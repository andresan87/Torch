/*-----------------------------------------------------------------------

 Ethanon Engine (C) Copyright 2009-2011 Andre Santee
 http://www.asantee.net/ethanon/

    This file is part of Ethanon Engine.

    Ethanon Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    Ethanon Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with Ethanon Engine. If not, see
    <http://www.gnu.org/licenses/>.

-----------------------------------------------------------------------*/

#include "ETHScene.h"
#include "ETHEntityArray.h"
#include "ETHLightmapGen.h"
#include "ETHShaderManager.h"
#include "ETHResourceProvider.h"

#ifdef GS2D_STR_TYPE_WCHAR
#include "../addons/utf16/scriptbuilder.h"
#else
#include "../addons/ansi/scriptbuilder.h"
#endif

#include <string.h>
#include <stdlib.h>
#include <sstream>
#include <map>
#include <assert.h>

ETHScene::ETHScene(const str_type::string& fileName, ETHResourceProviderPtr provider, const ETHSceneProperties& props, asIScriptModule *pModule,
				   asIScriptContext *pContext, const Vector2& v2BucketSize) :
	m_buckets(provider, v2BucketSize, true),
	m_tempEntities(provider)
{
	Init(provider, props, pModule, pContext);
	LoadFromFile(fileName);
}

ETHScene::ETHScene(ETHResourceProviderPtr provider, const ETHSceneProperties& props, asIScriptModule *pModule, asIScriptContext *pContext, const Vector2 &v2BucketSize) :
	m_buckets(provider, v2BucketSize, true),
	m_tempEntities(provider)
{
	Init(provider, props, pModule, pContext);
}

void ETHScene::Init(ETHResourceProviderPtr provider, const ETHSceneProperties& props, asIScriptModule *pModule, asIScriptContext *pContext)
{
	m_provider = provider;
	m_sceneProps = props;
	m_pModule = pModule;
	m_pContext = pContext;
	m_idCounter = 0;
	m_minSceneHeight = 0.0f;
	m_enableLightmaps = false;
	m_usingRTShadows = true;
	m_nCurrentLights = 0;
	m_nRenderedEntities = -1;
	m_showingLightmaps = true;
	m_rotatingHalos = false;
	m_maxSceneHeight = m_provider->GetVideo()->GetScreenSizeF().y;
}

void ETHScene::ClearResources()
{
	m_provider->GetGraphicResourceManager()->ReleaseResources();
	m_provider->GetAudioResourceManager()->ReleaseResources();
}

bool ETHScene::SaveToFile(const str_type::string& fileName)
{
	if (m_buckets.IsEmpty())
	{
		ETH_STREAM_DECL(ss) << GS_L("ETHScene::Save: there are no entities to save: ") << fileName;
		m_provider->Log(ss.str(), Platform::FileLogger::ERROR);
		return false;
	}

	// Write the header to the file
	TiXmlDocument doc;
	TiXmlDeclaration *pDecl = new TiXmlDeclaration(GS_L("1.0"), GS_L(""), GS_L(""));
	doc.LinkEndChild(pDecl);

	TiXmlElement *pElement = new TiXmlElement(GS_L("Ethanon"));
	doc.LinkEndChild(pElement);
	TiXmlElement *pRoot = doc.RootElement();

	// write the property header
	m_sceneProps.WriteToXMLFile(pRoot);

	// start writing entities
	TiXmlElement *pEntities = new TiXmlElement(GS_L("EntitiesInScene"));
	pRoot->LinkEndChild(pEntities);

	// Write every entity
	for (ETHBucketMap::iterator bucketIter = m_buckets.GetFirstBucket(); bucketIter != m_buckets.GetLastBucket(); bucketIter++)
	{
		ETHEntityList::const_iterator iEnd = bucketIter->second.end();
		for (ETHEntityList::iterator iter = bucketIter->second.begin(); iter != iEnd; iter++)
		{
			(*iter)->WriteToXMLFile(pEntities);
			#ifdef _DEBUG
			ETH_STREAM_DECL(ss) << GS_L("Entity written to file: ") << (*iter)->GetEntityName();
			m_provider->Log(ss.str(), Platform::FileLogger::INFO);
			#endif
		}
	}

	doc.SaveFile(fileName);
	return true;
}

bool ETHScene::LoadFromFile(const str_type::string& fileName)
{
	Platform::FileManagerPtr fileManager = m_provider->GetVideo()->GetFileManager();

	// load the scene from a file
	{
		Platform::FileBuffer file;
		fileManager->GetFileBuffer(fileName, file);
		if (!file)
		{
			ETH_STREAM_DECL(ss) << GS_L("ETHScene::Open: file not found (") << fileName << GS_L(")");
			m_provider->Log(ss.str(), Platform::FileLogger::ERROR);
			return false;
		}
	}
	m_minSceneHeight = 0.0f;
	m_maxSceneHeight = m_provider->GetVideo()->GetScreenSizeF().y;

	// Read the header and check if the file is valid
	TiXmlDocument doc(fileName);
	str_type::string content;
	fileManager->GetUTF16FileString(fileName, content);
	if (!doc.LoadFile(content, TIXML_ENCODING_LEGACY))
	{
		ETH_STREAM_DECL(ss) << GS_L("ETHScene::Open: file found, but parsing failed (") << fileName << GS_L(")");
		m_provider->Log(ss.str(), Platform::FileLogger::ERROR);
		return false;
	}

	TiXmlHandle hDoc(&doc);
	TiXmlHandle hRoot(0);

	TiXmlElement *pElement = hDoc.FirstChildElement().Element();
	if (!pElement)
	{
		ETH_STREAM_DECL(ss) << GS_L("ETHScene::Open: couldn't find root element (") << fileName << GS_L(")");
		m_provider->Log(ss.str(), Platform::FileLogger::ERROR);
		return false;
	}
	return ReadFromXMLFile(pElement);
}

bool ETHScene::ReadFromXMLFile(TiXmlElement *pRoot)
{
	m_sceneProps.ReadFromXMLFile(pRoot);
	TiXmlNode *pNode = pRoot->FirstChild(GS_L("EntitiesInScene"));
	if (pNode)
	{
		TiXmlElement *pEntities = pNode->ToElement();
		if (pEntities)
		{
			pNode = pEntities->FirstChild(GS_L("Entity"));
			if (pNode)
			{
				TiXmlElement *pEntityIter = pNode->ToElement();
				if (pEntityIter)
				{
					do
					{
						ETHRenderEntity* entity = new ETHRenderEntity(pEntityIter, m_provider, ++m_idCounter);
						AddEntity(entity);
						pEntityIter = pEntityIter->NextSiblingElement();
					} while (pEntityIter);
				}
			}
		}
	}
	return true;
}

int ETHScene::AddEntity(ETHRenderEntity* pEntity, const str_type::string& alternativeName)
{
	// sets an alternative name if there is any
	if (!alternativeName.empty())
	{
		pEntity->ChangeEntityName(alternativeName);
	}

	// generate an unique id
	if (pEntity->GetID() < 0)
	{
		pEntity->SetID(++m_idCounter);
	}

	m_buckets.Add(pEntity, (pEntity->GetType() == ETH_HORIZONTAL) ? ETHBucketManager::FRONT : ETHBucketManager::BACK);

	m_maxSceneHeight = Max(m_maxSceneHeight, pEntity->GetMaxHeight());
	m_minSceneHeight = Min(m_minSceneHeight, pEntity->GetMinHeight());

	// find a callback function for this one
	if (m_pContext && m_pModule)
	{
		AssignCallbackScript(pEntity);
	}

	// if it has a callback and is dynamic, or if it's temporary, add it to the "callback constant run list"
	m_tempEntities.AddEntityWhenEligible(pEntity);

	// restart all sound effects for this one
	// It's useful in case of explosion sfx's for example. It'll start all over again
	pEntity->StartSFX();

	return pEntity->GetID();
}

int ETHScene::AddEntity(ETHRenderEntity* pEntity)
{
	return AddEntity(pEntity, GS_L(""));	
}

const ETHSceneProperties* ETHScene::GetSceneProperties() const
{
	return &m_sceneProps;
}

ETHSceneProperties* ETHScene::GetEditableSceneProperties()
{
	return &m_sceneProps;
}

void ETHScene::SetSceneProperties(const ETHSceneProperties &prop)
{
	m_sceneProps = prop;
}

void ETHScene::AddLight(const ETHLight &light)
{
	// if the light isn't in screen, don't add it
	if (!light.staticLight)
		if (!ETHGlobal::IsSphereInScreen(light.pos, light.range, m_provider->GetVideo()))
			return;

	m_lights.push_back(light);
}

void ETHScene::AddLight(const ETHLight &light, const Vector3& v3Pos)
{
	ETHLight childLight = light;
	childLight.pos += v3Pos;
	AddLight(childLight);
}

bool ETHScene::GenerateLightmaps(const int id)
{
	const ETHSpriteEntity *pRender = (id >= 0) ? m_buckets.SeekEntity(id) : 0;
	const Vector2 v2Bucket = (pRender) ? ETHGlobal::GetBucket(pRender->GetPositionXY(), GetBucketSize()) : Vector2(0,0);

	for (ETHBucketMap::iterator bucketIter = m_buckets.GetFirstBucket(); bucketIter != m_buckets.GetLastBucket(); bucketIter++)
	{
		// if we're lighting only one entity and it is not in this bucket, skip it.
		// I know we could have used the find method to go directly to that bucket
		// but this function os not that critical to make the effort worth it.
		if (id >= 0)
			if (v2Bucket != bucketIter->first)
				continue;

		// iterate over all entities in this bucket
		ETHEntityList::const_iterator iEnd = bucketIter->second.end();
		for (ETHEntityList::iterator iter = bucketIter->second.begin(); iter != iEnd; iter++)
		{
			// if nID is valid, let's try to generate the lightmap for this one and only entity
			if (id >= 0)
				if (id != (*iter)->GetID())
					continue;

			Vector2 v2Size(1,1);
			Vector2 v2Origin(0,0);
			if ((*iter)->GetSprite())
			{
				v2Size = (*iter)->GetCurrentSize();
				v2Origin = (*iter)->ComputeOrigin(v2Size);
			}

			// Place the current entity at the top-left corner to align
			// it to the render target
			const Vector3 oldPos = (*iter)->GetPosition();
			const Vector3 newPos = Vector3(v2Origin.x, v2Origin.y, 0);

			// fill the light list
			for (ETHBucketMap::iterator lbucketIter = m_buckets.GetFirstBucket(); lbucketIter != m_buckets.GetLastBucket(); lbucketIter++)
			{
				ETHEntityList::const_iterator liEnd = lbucketIter->second.end();
				for (ETHEntityList::iterator liter = lbucketIter->second.begin(); liter != liEnd; liter++)
				{
					if ((*liter)->IsStatic() && (*liter)->HasLightSource())
					{
						AddLight(*((*liter)->GetLight()), newPos-oldPos+(*liter)->GetPosition());
					}
				}
			}

			ETHLightmapGen((*iter), m_provider->GetShaderManager(), m_lights.begin(), m_lights.end(),
							m_buckets, oldPos, newPos, m_minSceneHeight, m_maxSceneHeight, m_sceneProps);

			(*iter)->SetOrphanPosition(oldPos);
			m_lights.clear();
		}
	}
	#ifdef _DEBUG
	ETH_STREAM_DECL(ss) << GS_L("Lightmaps created... ");
	m_provider->Log(ss.str(), Platform::FileLogger::INFO);
	#endif
	return true;
}

bool ETHScene::RenderScene(const bool roundUp, SpritePtr pOutline, SpritePtr pInvisibleEntSymbol)
{
	m_provider->GetShaderManager()->SetParallaxIntensity(m_sceneProps.parallaxIntensity);

	float minHeight, maxHeight;

	m_provider->GetVideo()->SetBlendMode(1, GSBM_ADD);

	m_tempEntities.CheckTemporaryEntities(GetZAxisDirection(), m_buckets);

	m_provider->GetVideo()->SetZWrite(true);
	m_provider->GetVideo()->SetZBuffer(true);

	// temporary lists that will tell what to render and what to do
	std::list<ETHRenderEntity*> particles;
	std::list<ETHRenderEntity*> halos;

	// draw ambient pass
	m_provider->GetVideo()->RoundUpPosition(roundUp);
	RenderList(minHeight, maxHeight, pOutline, pInvisibleEntSymbol,
				particles, halos, roundUp);
	m_provider->GetVideo()->RoundUpPosition(false);

	// draw lights!
	m_provider->GetVideo()->SetZWrite(false);
	//RenderLightList(roundUp);

	m_provider->GetVideo()->SetZWrite(true);
	RenderParticleList(particles);
	m_lights.clear();
	RenderTransparentLayer(halos, maxHeight, minHeight);

	m_provider->GetVideo()->SetZWrite(false);
	m_provider->GetVideo()->SetZBuffer(false);

	m_minSceneHeight = minHeight;
	m_maxSceneHeight = maxHeight;
	return true;
}

void ETHScene::EnableLightmaps(const bool enable)
{
	m_enableLightmaps = enable;
}

bool ETHScene::AreLightmapsEnabled() const
{
	return m_enableLightmaps;
}

void ETHScene::EnableRealTimeShadows(const bool enable)
{
	m_usingRTShadows = enable;
}

bool ETHScene::AreRealTimeShadowsEnabled() const
{
	return m_usingRTShadows;
}

// number of lights last-time-drawn
int ETHScene::GetNumLights()
{
	return m_nCurrentLights;
}

// number of actual lights (slower)
int ETHScene::CountLights()
{
	m_nCurrentLights = 0;
	for (ETHBucketMap::iterator bucketIter = m_buckets.GetFirstBucket(); bucketIter != m_buckets.GetLastBucket(); bucketIter++)
	{
		ETHEntityList::const_iterator iEnd = bucketIter->second.end();
		for (ETHEntityList::iterator iter = bucketIter->second.begin(); iter != iEnd; iter++)
		{
			if ((*iter)->HasLightSource())
				m_nCurrentLights++;
		}
	}
	return m_nCurrentLights;
}

void ETHScene::ShowLightmaps(const bool show)
{
	m_showingLightmaps = show;
}

bool ETHScene::AreLightmapsShown()
{
	return m_showingLightmaps;
}

bool ETHScene::RenderTransparentLayer(std::list<ETHRenderEntity*> &halos, const float maxHeight, const float minHeight)
{
	GS2D_UNUSED_ARGUMENT(minHeight);
	GS2D_UNUSED_ARGUMENT(maxHeight);
	// Draw halos
	for (std::list<ETHRenderEntity*>::iterator iter = halos.begin(); iter != halos.end(); iter++)
	{
		if (m_provider->GetShaderManager()->BeginHaloPass(((*iter)->GetLight()), m_maxSceneHeight))
		{
			(*iter)->DrawHalo(m_maxSceneHeight, m_minSceneHeight, GetHaloRotation(), GetZAxisDirection());
			m_provider->GetShaderManager()->EndHaloPass();
		}
	}
	return true;
}

bool ETHScene::DrawBucketOutlines()
{
	// Gets the list of visible buckets
	std::list<Vector2> bucketList;
	ETHGlobal::GetIntersectingBuckets(bucketList, m_provider->GetVideo()->GetCameraPos(),
									m_provider->GetVideo()->GetScreenSizeF(), GetBucketSize(),
									IsDrawingBorderBuckets(), IsDrawingBorderBuckets());

	int nVisibleBuckets = 0;

	// Loop through all visible Buckets
	for (std::list<Vector2>::iterator bucketPositionIter = bucketList.begin(); bucketPositionIter != bucketList.end(); bucketPositionIter++)
	{
		nVisibleBuckets++;

		const float width = m_provider->GetVideo()->GetLineWidth();
		m_provider->GetVideo()->SetLineWidth(2.0f);
		const Vector2 v2BucketPos = *bucketPositionIter*GetBucketSize()-m_provider->GetVideo()->GetCameraPos();
		m_provider->GetVideo()->DrawLine(v2BucketPos, v2BucketPos+Vector2(GetBucketSize().x, 0.0f), GS_WHITE, GS_WHITE);
		m_provider->GetVideo()->DrawLine(v2BucketPos, v2BucketPos+Vector2(0.0f, GetBucketSize().y), GS_WHITE, GS_WHITE);
		m_provider->GetVideo()->DrawLine(v2BucketPos+GetBucketSize(), v2BucketPos+Vector2(0.0f, GetBucketSize().y), GS_WHITE, GS_WHITE);
		m_provider->GetVideo()->DrawLine(v2BucketPos+GetBucketSize(), v2BucketPos+Vector2(GetBucketSize().x, 0.0f), GS_WHITE, GS_WHITE);
		m_provider->GetVideo()->SetLineWidth(width);
		
		// draw bucket key
		str_type::stringstream ss;

		if (m_buckets.Find(*bucketPositionIter) != m_buckets.GetLastBucket())
		{
			ss << GS_L("(") << bucketPositionIter->x << GS_L(",") << bucketPositionIter->y << GS_L(")")
				<< GS_L(" - entities: ") << m_buckets.GetNumEntities(*bucketPositionIter);
		}
		else
		{
			ss << GS_L("(") << bucketPositionIter->x << GS_L(",") << bucketPositionIter->y << GS_L(")");
		}

		const Vector2 v2TextPos(*bucketPositionIter*GetBucketSize()-m_provider->GetVideo()->GetCameraPos());
		m_provider->GetVideo()->DrawBitmapText(v2TextPos, ss.str(), ETH_DEFAULT_BITMAP_FONT, GS_WHITE);
	}

	str_type::stringstream ss;
	ss << GS_L("Visible buckets: ") << nVisibleBuckets;

	m_provider->GetVideo()->DrawBitmapText(m_provider->GetVideo()->GetScreenSizeF()/2, ss.str(), ETH_DEFAULT_BITMAP_FONT, GS_WHITE);
	return true;
}

// TODO-TO-DO: this method is too large...
bool ETHScene::RenderList(float &minHeight, float &maxHeight, SpritePtr pOutline, SpritePtr pInvisibleEntSymbol,
						  std::list<ETHRenderEntity*> &outParticles, std::list<ETHRenderEntity*> &outHalos, const bool roundUp)
{
	// This multimap will store all entities contained in the visible buckets
	// It will automatically sort entities to draw them in an "alpha friendly" order
	std::multimap<float, ETHRenderEntity*> mmEntities;

	// store the max and min height to assign when everything is drawn
	maxHeight = m_maxSceneHeight;
	minHeight = m_minSceneHeight;

	m_nRenderedEntities = 0;

	// don't let bucket size be equal to 0
	assert(GetBucketSize().x != 0 || GetBucketSize().y != 0);

	// Gets the list of visible buckets
	std::list<Vector2> bucketList;
	const Vector2& v2CamPos = m_provider->GetVideo()->GetCameraPos(); //for debugging pourposes
	ETHGlobal::GetIntersectingBuckets(bucketList, v2CamPos,
									m_provider->GetVideo()->GetScreenSizeF(), GetBucketSize(),
									IsDrawingBorderBuckets(), IsDrawingBorderBuckets());

	// Loop through all visible Buckets
	for (std::list<Vector2>::iterator bucketPositionIter = bucketList.begin(); bucketPositionIter != bucketList.end(); bucketPositionIter++)
	{
		ETHBucketMap::iterator bucketIter = m_buckets.Find(*bucketPositionIter);

		if (bucketIter == m_buckets.GetLastBucket())
			continue;

		if (bucketIter->second.empty())
			continue;

		ETHEntityList::const_iterator iEnd = bucketIter->second.end();
		for (ETHEntityList::iterator iter = bucketIter->second.begin(); iter != iEnd; iter++)
		{
			ETHSpriteEntity *pRenderEntity = (*iter);

			// update scene bounding for depth buffer
			maxHeight = Max(maxHeight, pRenderEntity->GetMaxHeight());
			minHeight = Min(minHeight, pRenderEntity->GetMinHeight());

			if (pRenderEntity->IsHidden())
				continue;

			// fill the light list for this frame
			// const ETHEntityFile &entity = pRenderEntity->GetData()->entity;
			if (pRenderEntity->HasLightSource())
			{
				ETHLight light = *(pRenderEntity->GetLight());
				// if it has a particle system in the first slot, adjust the light
				// brightness according to the number os active particles
				if (pRenderEntity->GetParticleManager(0) && !pRenderEntity->IsStatic())
				{
					boost::shared_ptr<ETHParticleManager> paticleManager = pRenderEntity->GetParticleManager(0);
					light.color *= 
						static_cast<float>(paticleManager->GetNumActiveParticles()) /
						static_cast<float>(paticleManager->GetNumParticles());
				}
				AddLight(light, pRenderEntity->GetPosition());
			}

			// add this entity to the multimap to sort it for an alpha-friendly rendering list
			const Vector3& v3Pos = pRenderEntity->GetPosition();
			const ETH_ENTITY_TYPE type = pRenderEntity->GetType();
			const float depth = pRenderEntity->ComputeDepth(maxHeight, minHeight);
			const float drawHash = ComputeDrawHash(depth, v2CamPos, v3Pos, type);

			// add the entity to the render map
			mmEntities.insert(std::pair<float, ETHRenderEntity*>(drawHash, *iter));
			m_nRenderedEntities++;
		}
	}

	// Draw visible entities ordered in an alpha-friendly map
	for (std::multimap<float, ETHRenderEntity*>::iterator iter = mmEntities.begin(); iter != mmEntities.end(); iter++)
	{
		ETHRenderEntity *pRenderEntity = (iter->second);

		m_provider->GetShaderManager()->BeginAmbientPass(pRenderEntity, maxHeight, minHeight);

		// draws the ambient pass and if we're at the editor, draw the collision box if it's an invisible entity
		#ifdef _ETHANON_EDITOR
		if (pOutline && pRenderEntity->IsInvisible() && pRenderEntity->Collidable())
		{
			pRenderEntity->DrawCollisionBox(true, pOutline, GS_WHITE, maxHeight, minHeight, m_sceneProps.zAxisDirection);
		}
		#endif

		m_provider->GetVideo()->RoundUpPosition(roundUp);
		pRenderEntity->DrawAmbientPass(m_maxSceneHeight, m_minSceneHeight, (m_enableLightmaps && m_showingLightmaps), m_sceneProps);

		// draw "invisible entity symbol" if we're in the editor
		#ifdef _ETHANON_EDITOR
		if (pOutline)
		{
			if (pRenderEntity->IsInvisible() && pRenderEntity->Collidable())
			{
				pRenderEntity->DrawCollisionBox(false, pOutline, GS_WHITE, maxHeight, minHeight, m_sceneProps.zAxisDirection);
			}
			if (pRenderEntity->IsInvisible() && !pRenderEntity->Collidable())
			{
				const float depth = m_provider->GetVideo()->GetSpriteDepth();
				m_provider->GetVideo()->SetSpriteDepth(1.0f);
				pInvisibleEntSymbol->Draw(pRenderEntity->GetPositionXY());
				m_provider->GetVideo()->SetSpriteDepth(depth);
			}
		}
		#endif

		// fill the halo list
		// const ETHEntityFile &entity = pRenderEntity->GetData()->entity;
		if (pRenderEntity->HasLightSource() && pRenderEntity->GetHalo())
		{
			outHalos.push_back(pRenderEntity);
		}

		// fill the particle list for this frame
		if (pRenderEntity->HasParticleSystems())
		{
			outParticles.push_back(pRenderEntity);
		}

		// fill the callback list
		m_tempEntities.AddCallbackWhenEligible(pRenderEntity);

		m_provider->GetShaderManager()->EndAmbientPass();

		//draw light pass
		for (std::list<ETHLight>::iterator iter = m_lights.begin(); iter != m_lights.end(); iter++)
		{
			if (!pRenderEntity->IsHidden())
			{
				if (!(pRenderEntity->IsStatic() && iter->staticLight && m_enableLightmaps))
				{
					m_provider->GetVideo()->RoundUpPosition(roundUp);
					if (m_provider->GetShaderManager()->BeginLightPass(pRenderEntity, &(*iter), m_maxSceneHeight, m_minSceneHeight, GetLightIntensity()))
					{
						pRenderEntity->DrawLightPass(GetZAxisDirection());
						m_provider->GetShaderManager()->EndLightPass();

						m_provider->GetVideo()->RoundUpPosition(false);
						if (AreRealTimeShadowsEnabled())
						{
							if (m_provider->GetShaderManager()->BeginShadowPass(pRenderEntity, &(*iter), m_maxSceneHeight, m_minSceneHeight))
							{
								pRenderEntity->DrawShadow(m_maxSceneHeight, m_minSceneHeight, m_sceneProps, *iter, 0);
								m_provider->GetShaderManager()->EndShadowPass();
							}
						}
						m_provider->GetVideo()->RoundUpPosition(roundUp);
					}
				}
			}
		}
	}

	mmEntities.clear();
	m_nCurrentLights = m_lights.size();

	// Show the buckets outline in debug mode
	#if defined _DEBUG || defined _ETHANON_EDITOR
	if (m_provider->GetInput()->IsKeyDown(GSK_PAUSE))
	{
		DrawBucketOutlines();
	}
	#endif

	return true;
}

float ETHScene::ComputeDrawHash(const float depth, const Vector2& camPos, const Vector3& entityPos, const ETH_ENTITY_TYPE& type) const
{
	float drawHash;
	switch (type)
	{
	case ETH_HORIZONTAL:
		drawHash = depth / 2.0f;
		break;
	case ETH_VERTICAL:
		drawHash = (0.5f + depth) + (entityPos.y - camPos.y);
		break;
	case ETH_GROUND_DECAL:
	case ETH_OPAQUE_DECAL:
		drawHash = depth / 2.0f + 0.01f;
		break;
	default:
		drawHash = depth;
	}
	return drawHash;
}

int ETHScene::GetNumRenderedEntities()
{
	return m_nRenderedEntities;
}

void ETHScene::RunCallbacksFromList()
{
	m_tempEntities.RunCallbacksFromList();
}

bool ETHScene::RenderParticleList(std::list<ETHRenderEntity*> &particles)
{
	if (m_provider->GetShaderManager()->BeginParticlePass())
	{
		for (std::list<ETHRenderEntity*>::iterator iter = particles.begin(); iter != particles.end();)
		{
			ETHRenderEntity *pRenderEntity = (*iter);

			if (!m_tempEntities.IsTempEntityEligible(pRenderEntity))
			{
				pRenderEntity->UpdateParticleSystems(GetZAxisDirection());
			}
			for (std::size_t t=0; t<pRenderEntity->GetNumParticleSystems(); t++)
			{
				pRenderEntity->DrawParticles(t, m_maxSceneHeight, m_minSceneHeight, m_sceneProps);
			}
			iter++;
		}
		m_provider->GetShaderManager()->EndParticlePass();
	}
	// seed the random number generator
	Randomizer::Seed(m_provider->GetVideo()->GetElapsedTime());
	return true;
}

void ETHScene::ForceAllSFXStop()
{
	for (ETHBucketMap::iterator bucketIter = m_buckets.GetFirstBucket(); bucketIter != m_buckets.GetLastBucket(); bucketIter++)
	{
		ETHEntityList::const_iterator iEnd = bucketIter->second.end();
		for (ETHEntityList::iterator iter = bucketIter->second.begin(); iter != iEnd; iter++)
		{
			(*iter)->ForceSFXStop();
		}
	}
}

bool ETHScene::AssignCallbackScript(ETHSpriteEntity* entity)
{
	// removes extension from file name (to make a function name with it)
	const str_type::string entityName = ETHGlobal::RemoveExtension(entity->GetEntityName().c_str());

	// assembles a function name based on it's ID. If we don't find any,
	// we'll try to find a function based on entity original file name
	str_type::stringstream funcName;
	funcName << ETH_CALLBACK_PREFIX << entity->GetID();

	int id=-1;
	if ((id = CScriptBuilder::GetFunctionIdByName(m_pModule, funcName.str())) < 0)
	{
		// let's try to find a function with name based on the entity original file name
		funcName.str(GS_L(""));
		funcName << ETH_CALLBACK_PREFIX << entityName;
		id = CScriptBuilder::GetFunctionIdByName(m_pModule, funcName.str());
	}

	// if we found anything...
	if (id >= 0)
	{
		entity->SetController(ETHEntityControllerPtr(new ETHRawEntityController(entity->GetController(), m_pContext, id)));
		#ifdef _DEBUG
			ETH_STREAM_DECL(ss) << GS_L("ETHScene::AssignCallbackScripts: Callback assigned (") << funcName.str() << GS_L(").");
			m_provider->Log(ss.str(), Platform::FileLogger::INFO);
		#endif
	}
	else
	{
		str_type::stringstream ss;
		switch (id)
		{
		case asERROR:
			ss << GS_L("ETHScene::AssignCallbackScripts: invalid module (") << funcName.str() << GS_L(").");
			m_provider->Log(ss.str(), Platform::FileLogger::ERROR);
			return false;
		case asMULTIPLE_FUNCTIONS:
			ss << GS_L("\n*Script error:\nETHScene::AssignCallbackScripts: there are multiple functions with this name (") << funcName.str() << GS_L(").");
			m_provider->Log(ss.str(), Platform::FileLogger::ERROR);
			return false;
		};
	}
	return true;
}

bool ETHScene::AddFloatData(const str_type::string &entity, const str_type::string &name, const float value)
{
	return AddCustomData(entity, name, ETHCustomDataPtr(new ETHFloatData(value)));
}

bool ETHScene::AddIntData(const str_type::string &entity, const str_type::string &name, const int value)
{
	return AddCustomData(entity, name, ETHCustomDataPtr(new ETHIntData(value)));
}

bool ETHScene::AddUIntData(const str_type::string &entity, const str_type::string &name, const unsigned int value)
{
	return AddCustomData(entity, name, ETHCustomDataPtr(new ETHUIntData(value)));
}

bool ETHScene::AddStringData(const str_type::string &entity, const str_type::string &name, const str_type::string &value)
{
	return AddCustomData(entity, name, ETHCustomDataPtr(new ETHStringData(value)));
}

bool ETHScene::AddVector2Data(const str_type::string &entity, const str_type::string &name, const Vector2 &value)
{
	return AddCustomData(entity, name, ETHCustomDataPtr(new ETHVector2Data(value)));
}

bool ETHScene::AddVector3Data(const str_type::string &entity, const str_type::string &name, const Vector3 &value)
{
	return AddCustomData(entity, name, ETHCustomDataPtr(new ETHVector3Data(value)));
}

bool ETHScene::AddCustomData(const str_type::string &entity, const str_type::string &name, const ETHCustomDataConstPtr &inData)
{
	unsigned int count = 0;
	for (ETHBucketMap::iterator bucketIter = m_buckets.GetFirstBucket(); bucketIter != m_buckets.GetLastBucket(); bucketIter++)
	{
		ETHEntityList::const_iterator iEnd = bucketIter->second.end();
		for (ETHEntityList::iterator iter = bucketIter->second.begin(); iter != iEnd; iter++)
		{
			ETHSpriteEntity *pEntity = (*iter);
			if (entity == pEntity->GetEntityName())
			{
				pEntity->AddData(name, inData);
				count++;
			}
		}
	}
	return (count > 0);
}

void ETHScene::GetIntersectingEntities(const Vector2 &v2Here, ETHEntityArray &outVector, const bool screenSpace)
{
	m_buckets.GetIntersectingEntities(v2Here, outVector, screenSpace, m_sceneProps);
}

void ETHScene::SetLightIntensity(const float intensity)
{
	m_sceneProps.lightIntensity = intensity;
}
float ETHScene::GetLightIntensity() const
{
	return m_sceneProps.lightIntensity;
}

void ETHScene::SetAmbientLight(const Vector3 &color)
{
	m_sceneProps.ambient = color;
}

Vector3 ETHScene::GetAmbientLight() const
{
	return m_sceneProps.ambient;
}

void ETHScene::SetBorderBucketsDrawing(const bool enable)
{
	m_buckets.SetBorderBucketsDrawing(enable);
}

bool ETHScene::IsDrawingBorderBuckets() const
{
	return m_buckets.IsDrawingBorderBuckets();
}

void ETHScene::SetZAxisDirection(const Vector2 &v2)
{
	m_sceneProps.zAxisDirection = v2;
}

Vector2 ETHScene::GetZAxisDirection() const
{
	return m_sceneProps.zAxisDirection;
}

int ETHScene::GetLastID() const
{
	return m_idCounter;
}

float ETHScene::GetMaxHeight() const
{
	return m_maxSceneHeight;
}

float ETHScene::GetMinHeight() const
{
	return m_minSceneHeight;
}

Vector2 ETHScene::GetBucketSize() const
{
	return m_buckets.GetBucketSize();
}

void ETHScene::SetHaloRotation(const bool enable)
{
	m_rotatingHalos = enable;
}

bool ETHScene::GetHaloRotation() const
{
	return m_rotatingHalos;
}

ETHBucketManager& ETHScene::GetBucketManager()
{
	return m_buckets;
}

unsigned int ETHScene::GetNumEntities() const
{
	return m_buckets.GetNumEntities();
}

void ETHScene::RecoverResources()
{
	ETHEntityArray entities;
	m_buckets.GetEntityArray(entities);
	for (std::size_t t = 0; t < entities.size(); t++)
	{
		entities[t]->RecoverResources();
	}
}