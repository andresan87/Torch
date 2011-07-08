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

#include "ETHLightmapGen.h"
#include "ETHBucketManager.h"
#include "ETHShaderManager.h"

ETHLightmapGen::ETHLightmapGen(ETHRenderEntity* entity,
			   boost::shared_ptr<ETHShaderManager> shaderManager, std::list<ETHLight>::iterator iBegin, std::list<ETHLight>::iterator iEnd,
			   ETHBucketManager& buckets, const Vector3& oldPos, const Vector3& newPos, const float minHeight, const float maxHeight,
			   const ETHSceneProperties &sceneProps)
{
	const SpritePtr& sprite = entity->m_pSprite;
	const ETHEntityProperties& props = entity->m_properties;
	if (!sprite || !props.staticEntity || !props.applyLight)
	{
		return;
	}

	const VideoPtr& video = entity->m_provider->GetVideo();
	const Platform::FileLogger* logger = entity->m_provider->GetLogger();

	if (video->Rendering())
	{
		ETH_STREAM_DECL(ss) << GS_L("Entity ID #") << entity->GetID() << GS_L(": lightmaps can't be generated during application render.");
		logger->Log(ss.str(), Platform::FileLogger::ERROR);
		return;
	}

	const Vector2 v2Size = (sprite->GetNumRects() <= 1) ? sprite->GetBitmapSizeF() : sprite->GetRect().size;
	if (!(entity->m_pLightmap = video->CreateRenderTarget(static_cast<unsigned int>(v2Size.x), static_cast<unsigned int>(v2Size.y))))
	{
		ETH_STREAM_DECL(ss) << GS_L("ETHRenderEntity::GenerateLightmap: coudn't create the render target.");
		logger->Log(ss.str(), Platform::FileLogger::ERROR);
		entity->m_pLightmap.reset();
		return;
	}

	// Paint it black
	video->SetRenderTarget(entity->m_pLightmap);
	if (!video->BeginTargetScene(GS_BLACK))
	{
		ETH_STREAM_DECL(ss) << GS_L("ETHRenderEntity::GenerateLightmap: coudn't render to target.");
		logger->Log(ss.str(), Platform::FileLogger::ERROR);
		entity->m_pLightmap.reset();
		return;
	}
	video->EndTargetScene();

	entity->m_controller->SetPos(newPos);
	Vector2 v2CamPos = video->GetCameraPos();
	video->SetCameraPos(Vector2(0,0));

	for (std::list<ETHLight>::iterator iter = iBegin; iter != iEnd; iter++)
	{
		if (!iter->staticLight)
			continue;

		SpritePtr tempTarget;
		if (!(tempTarget = video->CreateRenderTarget(static_cast<unsigned int>(v2Size.x), static_cast<unsigned int>(v2Size.y))))
		{
			ETH_STREAM_DECL(ss) << GS_L("ETHRenderEntity::GenerateLightmap: coudn't create temporary render target.");
			logger->Log(ss.str(), Platform::FileLogger::ERROR);
			entity->m_pLightmap.reset();
			return;
		}

		if (!video->SetRenderTarget(tempTarget))
		{
			ETH_STREAM_DECL(ss) << GS_L("ETHRenderEntity::GenerateLightmap: coudn't set render target.");
			logger->Log(ss.str(), Platform::FileLogger::ERROR);
			entity->m_pLightmap.reset();
			return;
		}

		if (!video->BeginTargetScene(GS_BLACK))
		{
			ETH_STREAM_DECL(ss) << GS_L("ETHRenderEntity::GenerateLightmap: coudn't render to temporary target.");
			logger->Log(ss.str(), Platform::FileLogger::ERROR);
			entity->m_pLightmap.reset();
			return;
		}

		// draw light
		if (shaderManager->BeginLightPass(entity, &(*iter), video->GetScreenSizeF().y, 0.0f, sceneProps.lightIntensity, 0, true))
		{
			entity->DrawLightPass(sceneProps.zAxisDirection, true);
			shaderManager->EndLightPass();
		}

		// draw shadows
		if (entity->GetType() != ETH_VERTICAL)
		{
			for (ETHBucketMap::iterator bucketIter = buckets.GetFirstBucket(); bucketIter != buckets.GetLastBucket(); bucketIter++)
			{
				for (ETHEntityList::iterator entityIter = bucketIter->second.begin();
					entityIter != bucketIter->second.end(); entityIter++)
				{
					if (!(*entityIter)->IsStatic())
						continue;
					Vector3 oldPos2 = (*entityIter)->GetPosition();
					Vector3 newPos2 = oldPos2-(oldPos-newPos);
					(*entityIter)->SetOrphanPosition(newPos2);
					if (shaderManager->BeginShadowPass((*entityIter), &(*iter), maxHeight, minHeight))
					{
						(*entityIter)->DrawShadow(maxHeight, minHeight, sceneProps, *iter, 0, true, true, entity->GetAngle(), entity->GetPosition());
						shaderManager->EndShadowPass();
					}
					(*entityIter)->SetOrphanPosition(oldPos2);
				}
			}
		}
		video->EndTargetScene();

		// draw the shadowed light by adding it to the final lightmap
		video->SetRenderTarget(entity->m_pLightmap);
		video->BeginTargetScene(GS_BLACK, false);
		const GS_ALPHA_MODE oldAM = video->GetAlphaMode();
		video->SetAlphaMode(GSAM_ADD);
		tempTarget->Draw(Vector2(0,0));
		video->SetAlphaMode(oldAM);
		video->EndTargetScene();
	}

	entity->m_pLightmap->GenerateBackup();
	video->SetRenderTarget(SpritePtr());
	video->SetCameraPos(v2CamPos);
}
