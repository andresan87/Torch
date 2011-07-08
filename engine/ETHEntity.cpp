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

#include "ETHEntity.h"

ETHEntity::ETHEntity(const str_type::string& filePath, const int nId, const Platform::FileManagerPtr& fileManager) :
	ETHScriptEntity(),
	m_properties(filePath, fileManager),
	m_id(nId),
	m_controller(new ETHRawEntityController(Vector3(0, 0, 0), 0.0f))
{
	Zero();
}

ETHEntity::ETHEntity(TiXmlElement *pElement, const int nId) : 
	ETHScriptEntity(),
	m_id(nId),
	m_controller(new ETHRawEntityController(Vector3(0, 0, 0), 0.0f))
{
	Zero();
	ReadFromXMLFile(pElement);
}

ETHEntity::ETHEntity() : 
	ETHScriptEntity(),
	m_id(-1),
	m_controller(new ETHRawEntityController(Vector3(0, 0, 0), 0.0f))
{
	Zero();
}

void ETHEntity::Zero()
{
	m_v4Color = Vector4(1,1,1,1);
	m_spriteFrame = (0);
	m_shadowZ = (0.0f);
	m_hide = (ETH_FALSE);
}

bool ETHEntity::WriteToXMLFile(TiXmlElement *pHeadRoot) const
{
	TiXmlElement *pEntity = new TiXmlElement(GS_L("Entity"));
	pHeadRoot->LinkEndChild(pEntity);

	TiXmlElement *pElement;

	// if it has a specific name, save it
	if (m_properties.entityName != GS_L(""))
	{
		pElement = new TiXmlElement(GS_L("EntityName"));
		pElement->LinkEndChild(new TiXmlText(m_properties.entityName));
		pEntity->LinkEndChild(pElement);
	}

	pElement = new TiXmlElement(GS_L("Color"));
	pEntity->LinkEndChild(pElement); 
	pElement->SetDoubleAttribute(GS_L("r"), m_v4Color.x);
	pElement->SetDoubleAttribute(GS_L("g"), m_v4Color.y);
	pElement->SetDoubleAttribute(GS_L("b"), m_v4Color.z);
	pElement->SetDoubleAttribute(GS_L("a"), m_v4Color.w);

	pElement = new TiXmlElement(GS_L("Position"));
	pEntity->LinkEndChild(pElement); 
	pElement->SetDoubleAttribute(GS_L("x"), m_controller->GetPos().x);
	pElement->SetDoubleAttribute(GS_L("y"), m_controller->GetPos().y);
	pElement->SetDoubleAttribute(GS_L("z"), m_controller->GetPos().z);
	pElement->SetDoubleAttribute(GS_L("angle"), m_controller->GetAngle());

	pEntity->SetAttribute(GS_L("id"), m_id);
	pEntity->SetAttribute(GS_L("spriteFrame"), m_spriteFrame);
	pEntity->SetDoubleAttribute(GS_L("shadowZ"), m_shadowZ);
	pEntity->SetAttribute(GS_L("hide"), m_hide);

	m_properties.WriteToXMLFile(pEntity);
	return true;
}

bool ETHEntity::ReadFromXMLFile(TiXmlElement *pElement)
{
	pElement->QueryIntAttribute(GS_L("id"), &m_id);
	pElement->QueryFloatAttribute(GS_L("shadowZ"), &m_shadowZ);

	int nHide = 0;
	pElement->QueryIntAttribute(GS_L("hide"), &nHide);
	m_hide = (!nHide) ? ETH_FALSE : ETH_TRUE;

	int signedSpriteFrame = 0;
	pElement->QueryIntAttribute(GS_L("spriteFrame"), &signedSpriteFrame);
	m_spriteFrame = static_cast<unsigned int>(signedSpriteFrame);

	TiXmlNode *pNode;
	TiXmlElement *pStringElement;

	pNode = pElement->FirstChild(GS_L("EntityName"));
	if (pNode)
	{
		pStringElement = pNode->ToElement();
		if (pStringElement)
		{
			m_properties.entityName = pStringElement->GetText();
		}
	}

	TiXmlElement *pIter;
	pNode = pElement->FirstChild(GS_L("Color"));
	if (pNode)
	{
		pIter = pNode->ToElement();
		if (pIter)
		{
			pIter->QueryFloatAttribute(GS_L("r"), &m_v4Color.x);
			pIter->QueryFloatAttribute(GS_L("g"), &m_v4Color.y);
			pIter->QueryFloatAttribute(GS_L("b"), &m_v4Color.z);
			pIter->QueryFloatAttribute(GS_L("a"), &m_v4Color.w);
		}
	}

	pNode = pElement->FirstChild(GS_L("Position"));
	if (pNode)
	{
		pIter = pNode->ToElement();
		if (pIter)
		{
			Vector3 pos; float angle = 0.0f;
			pIter->QueryFloatAttribute(GS_L("x"), &pos.x);
			pIter->QueryFloatAttribute(GS_L("y"), &pos.y);
			pIter->QueryFloatAttribute(GS_L("z"), &pos.z);
			pIter->QueryFloatAttribute(GS_L("angle"), &angle);
			m_controller->SetPos(pos);
			m_controller->SetAngle(angle);
		}
	}

	pNode = pElement->FirstChild(GS_L("Entity"));
	if (pNode)
	{
		m_properties.ReadFromXMLFile(pNode->ToElement());
	}
	return true;
}

int ETHEntity::GetID() const
{
	return m_id;
}

ETHEntityControllerPtr ETHEntity::GetController()
{
	return m_controller;
}

void ETHEntity::SetController(const ETHEntityControllerPtr& controller)
{
	m_controller = controller;
}

Vector2 ETHEntity::ComputeOrigin(const Vector2 &v2Size) const
{
	Vector2 v2Center;
	switch (ETHGlobal::ConvertToGSSO(GetType()))
	{
	case GSEO_RECT_CENTER:
	case GSEO_CENTER:
		v2Center.x = v2Size.x/2.0f;
		v2Center.y = v2Size.y/2.0f;
		break;
	case GSEO_RECT_CENTER_BOTTOM:
	case GSEO_CENTER_BOTTOM:
		v2Center.x = v2Size.x/2.0f;
		v2Center.y = v2Size.y;
		break;
	case GSEO_RECT_CENTER_TOP:
	case GSEO_CENTER_TOP:
		v2Center.x = v2Size.x/2.0f;
		v2Center.y = 0.0f;
		break;
	default:
	case GSEO_DEFAULT:
		v2Center.x = 0.0f;
		v2Center.y = 0.0f;
		break;
	};	
	return v2Center + m_properties.pivotAdjust;
}

void ETHEntity::ChangeEntityName(const str_type::string& name)
{
	m_properties.entityName = name;
}

str_type::string ETHEntity::GetEntityName() const
{
	return m_properties.entityName;
}

ETH_COLLISION_BOX ETHEntity::GetCollisionBox() const
{
	if (IsCollidable())
	{
		return *m_properties.collision.get();
	}
	return ETH_COLLISION_BOX(Vector3(0,0,0), Vector3(0,0,0));
}

std::size_t ETHEntity::GetNumParticleSystems() const
{
	return m_properties.particleSystems.size();
}

ETH_ENTITY_TYPE ETHEntity::GetType() const
{
	return m_properties.type;
}

Vector3 ETHEntity::GetPosition() const
{
	return m_controller->GetPos();
}

Vector2 ETHEntity::GetPositionXY() const
{
	const Vector3& pos = m_controller->GetPos();
	return Vector2(pos.x, pos.y);
}

void ETHEntity::SetPosition(const Vector3& pos, ETHBucketManager& buckets)
{
	const Vector2 v2CurrentBucket = ETHGlobal::GetBucket(GetPositionXY(), buckets.GetBucketSize());
	m_controller->SetPos(pos);
	const Vector2 v2NextBucket = ETHGlobal::GetBucket(GetPositionXY(), buckets.GetBucketSize());
	buckets.MoveEntity(GetID(), v2CurrentBucket, v2NextBucket);
}

void ETHEntity::SetPositionXY(const Vector2& pos, ETHBucketManager& buckets)
{
	SetPosition(Vector3(pos, m_controller->GetPos().z), buckets);
}

void ETHEntity::AddToPosition(const Vector3& pos, ETHBucketManager& buckets)
{
	const Vector2 v2CurrentBucket = ETHGlobal::GetBucket(GetPositionXY(), buckets.GetBucketSize());
	m_controller->AddToPos(pos);
	const Vector2 v2NextBucket = ETHGlobal::GetBucket(GetPositionXY(), buckets.GetBucketSize());
	buckets.MoveEntity(GetID(), v2CurrentBucket, v2NextBucket);
}

void ETHEntity::AddToPositionXY(const Vector2& pos, ETHBucketManager& buckets)
{
	AddToPosition(Vector3(pos, 0.0f), buckets);
}

void ETHEntity::SetOrphanPosition(const Vector3& pos)
{
	m_controller->SetPos(pos);
}

void ETHEntity::SetOrphanPositionXY(const Vector2& pos)
{
	m_controller->SetPos(Vector3(pos, m_controller->GetPos().z));
}

void ETHEntity::SetAngle(const float angle)
{
	m_controller->SetAngle(angle);
}

void ETHEntity::AddToAngle(const float angle)
{
	m_controller->AddToAngle(angle);
}

float ETHEntity::GetAngle() const
{
	return m_controller->GetAngle();
}

void ETHEntity::SetLightRange(const float range)
{
	if (HasLightSource())
		m_properties.light->range = range;
}

float ETHEntity::GetLightRange() const
{
	if (HasLightSource())
		return m_properties.light->range;
	else
		return 0.0f;
}

void ETHEntity::SetLightColor(const Vector3 &color)
{
	if (HasLightSource())
		m_properties.light->color = color;
}

Vector3 ETHEntity::GetLightColor() const
{
	if (HasLightSource())
		return m_properties.light->color;
	else
		return Vector3(0, 0, 0);
}

void ETHEntity::SetEmissiveColor(const Vector3 &color)
{
	m_properties.emissiveColor = Vector4(color, 1);
}

Vector3 ETHEntity::GetEmissiveColor() const
{
	const Vector4 *pColor = &m_properties.emissiveColor;
	return Vector3(pColor->x, pColor->y, pColor->z);
}

void ETHEntity::SetColor(const Vector3& color)
{
	m_v4Color.x = color.x;
	m_v4Color.y = color.y;
	m_v4Color.z = color.z;
}

void ETHEntity::SetColor(const Vector4& color)
{
	m_v4Color = color;
}

void ETHEntity::SetAlpha(const float alpha)
{
	m_v4Color.w = alpha;
}

Vector3 ETHEntity::GetColor() const
{
	const Vector4 *p = &m_v4Color;
	return Vector3(p->x, p->y, p->z);
}

Vector4 ETHEntity::GetColorARGB() const
{
	return m_v4Color;
}

float ETHEntity::GetAlpha() const
{
	return m_v4Color.w;
}

void ETHEntity::SetShadowZ(const float z)
{
	m_shadowZ = z;
}

float ETHEntity::GetShadowZ() const
{
	return m_shadowZ;
}

bool ETHEntity::HasShadow() const
{
	return (m_properties.castShadow == ETH_TRUE);
}

void ETHEntity::SetLayerDepth(const float depth)
{
	m_properties.type = ETH_LAYERABLE;
	m_properties.layerDepth = Max(0.0f, Min(depth, 1.0f));
}

float ETHEntity::GetLayerDepth() const
{
	return (m_properties.type == ETH_LAYERABLE) ? m_properties.layerDepth : -1.0f;
}

bool ETHEntity::IsRotatable() const
{
	return GetType() != ETH_VERTICAL;
}

Vector4 ETHEntity::GetColorF() const
{
	return m_v4Color;
}

bool ETHEntity::HasLightSource() const
{
	return (m_properties.light);
}

bool ETHEntity::HasParticleSystems() const
{
	return (m_properties.particleSystems.size() > 0);
}

bool ETHEntity::HasSoundEffect() const
{
	for (std::size_t t = 0; t < m_properties.particleSystems.size(); t++)
	{
		if (m_properties.particleSystems[t]->soundFXFile.length() > 0)
			return true;
	}
	return false;
}

Vector3 ETHEntity::GetLightPosition() const
{
	if (HasLightSource())
		return GetPosition() + m_properties.light->pos;
	else
		return Vector3(0,0,0);
}

Vector3 ETHEntity::GetLightRelativePosition() const
{
	if (HasLightSource())
		return m_properties.light->pos;
	else
		return Vector3(0,0,0);
}

bool ETHEntity::HasHalo() const
{
	return (!HasLightSource()) ? false : (!m_properties.light->haloBitmap.empty());
}

void ETHEntity::SetCollision(const bool enable)
{
	// TODO/TO-DO deprecate it
	if (!enable)
	{
		m_properties.collision.reset();
	}
}

bool ETHEntity::IsCollidable() const
{
	return (m_properties.collision);
}

GS_ALPHA_MODE ETHEntity::GetBlendMode() const
{
	return m_properties.blendMode;
}

unsigned int ETHEntity::GetNumFrames() const
{
	return static_cast<unsigned int>(m_properties.spriteCut.x * m_properties.spriteCut.y);
}

bool ETHEntity::HasCallback() const
{
	return m_controller->HasCallback();
}

bool ETHEntity::IsStatic() const
{
	return ETHGlobal::ToBool(m_properties.staticEntity);
}

bool ETHEntity::IsApplyLight() const
{
	return ETHGlobal::ToBool(m_properties.applyLight);
}

void ETHEntity::Hide(const bool hide)
{
	m_hide = hide;
}

bool ETHEntity::IsHidden() const
{
	return (m_hide == ETH_FALSE) ? false : true;
}

bool ETHEntity::IsTemporary() const
{
	unsigned int temporary = 0, existent = 0;
	for (std::size_t t=0; t<m_properties.particleSystems.size(); t++)
	{
		if (m_properties.particleSystems[t]->nParticles > 0)
		{
			if (m_properties.particleSystems[t]->repeat > 0)
				temporary++;
			existent++;
		}
	}
	if (existent && temporary == existent && m_properties.spriteFile == GS_L(""))
	{
		return true;
	}
	return false;
}

bool ETHEntity::IsCastShadow() const
{
	return (m_properties.castShadow == ETH_FALSE) ? false : true;
}

bool ETHEntity::IsInvisible() const
{
	return (m_properties.spriteFile == GS_L("") /*&& !HasLightSource()*/ && !HasParticleSystems());
}

Vector2 ETHEntity::GetCurrentBucket(const ETHBucketManager& buckets) const
{
	return ETHGlobal::GetBucket(GetPositionXY(), buckets.GetBucketSize());
}

const ETHLight* ETHEntity::GetLight() const
{
	return m_properties.light.get();
}

bool ETHEntity::SetFrame(const unsigned int frame)
{
	const Vector2i *pv2Cut = &m_properties.spriteCut;
	if (frame > static_cast<unsigned int>(pv2Cut->x * pv2Cut->y))
	{
		m_spriteFrame = (0);
		return false;
	}
	else
	{
		m_spriteFrame = (frame);
		return true;
	}
}

unsigned int ETHEntity::GetFrame() const
{
	return m_spriteFrame;
}

bool ETHEntity::SetFrame(const unsigned int column, const unsigned int row)
{
	const Vector2i *pv2Cut = &m_properties.spriteCut;
	const unsigned int cutX = static_cast<unsigned int>(pv2Cut->x);
	const unsigned int cutY = static_cast<unsigned int>(pv2Cut->y);
	if (column >= cutX || row >= cutY)
	{
		m_spriteFrame = (0);
		return false;
	}
	else
	{
		m_spriteFrame = ((row*cutX)+column);
		return true;
	}
}

bool ETHEntity::RunCallbackScript()
{
	return m_controller->RunCallback(this);
}

float ETHEntity::GetSpecularPower() const
{
	return m_properties.specularPower;
}

float ETHEntity::GetSpecularBrightness() const
{
	return m_properties.specularBrightness;
}

void ETHEntity::TurnDynamic()
{
	m_properties.staticEntity = false;
	if (m_properties.light)
		m_properties.light->staticLight = false;
}

void ETHEntity::TurnStatic()
{
	m_properties.staticEntity = true;
	if (m_properties.light)
		m_properties.light->staticLight = true;
}

const ETHCustomDataManager *ETHEntity::GetCustomDataManager() const
{
	return &m_properties;
}

void ETHEntity::AddData(const str_type::string &name, const ETHCustomDataConstPtr &dataIn)
{
	m_properties.AddData(name, dataIn);
}

void ETHEntity::SetFloat(const str_type::string &name, const float &value)
{
	m_properties.SetFloat(name, value);
}

void ETHEntity::SetInt(const str_type::string &name, const int &value)
{
	m_properties.SetInt(name, value);
}

void ETHEntity::SetUInt(const str_type::string &name, const unsigned int &value)
{
	m_properties.SetUInt(name, value);
}

void ETHEntity::SetString(const str_type::string &name, const str_type::string &value)
{
	m_properties.SetString(name, value);
}

void ETHEntity::SetVector2(const str_type::string &name, const Vector2 &value)
{
	m_properties.SetVector2(name, value);
}

void ETHEntity::SetVector3(const str_type::string &name, const Vector3 &value)
{
	m_properties.SetVector3(name, value);
}

float ETHEntity::GetFloat(const str_type::string &name) const
{
	float fOut = 0.0f;
	m_properties.GetFloat(name, fOut);
	return fOut;
}

int ETHEntity::GetInt(const str_type::string &name) const
{
	int nOut = 0;
	m_properties.GetInt(name, nOut);
	return nOut;
}

unsigned int ETHEntity::GetUInt(const str_type::string &name) const
{
	unsigned int nOut = 0;
	m_properties.GetUInt(name, nOut);
	return nOut;
}

str_type::string ETHEntity::GetString(const str_type::string &name) const
{
	str_type::string sOut = GS_L("");
	m_properties.GetString(name, sOut);
	return sOut;
}

Vector2 ETHEntity::GetVector2(const str_type::string &name) const
{
	Vector2 vOut(0,0);
	m_properties.GetVector2(name, vOut);
	return vOut;
}

Vector3 ETHEntity::GetVector3(const str_type::string &name) const
{
	Vector3 vOut(0,0,0);
	m_properties.GetVector3(name, vOut);
	return vOut;
}

void ETHEntity::AddToFloat(const str_type::string &name, const float &value)
{
	m_properties.AddToFloat(name, value);
}

void ETHEntity::AddToInt(const str_type::string &name, const int &value)
{
	m_properties.AddToInt(name, value);
}

void ETHEntity::AddToUInt(const str_type::string &name, const unsigned int &value)
{
	m_properties.AddToUInt(name, value);
}

void ETHEntity::AddToVector2(const str_type::string &name, const Vector2 &v)
{
	m_properties.AddToVector2(name, v);
}

void ETHEntity::AddToVector3(const str_type::string &name, const Vector3 &v)
{
	m_properties.AddToVector3(name, v);
}

void ETHEntity::MultiplyFloat(const str_type::string &name, const float &value)
{
	m_properties.MultiplyFloat(name, value);
}

void ETHEntity::MultiplyInt(const str_type::string &name, const int &value)
{
	m_properties.MultiplyInt(name, value);
}

void ETHEntity::MultiplyUInt(const str_type::string &name, const unsigned int &value)
{
	m_properties.MultiplyUInt(name, value);
}

void ETHEntity::MultiplyVector2(const str_type::string &name, const float &value)
{
	m_properties.MultiplyVector2(name, value);
}

void ETHEntity::MultiplyVector3(const str_type::string &name, const float &value)
{
	m_properties.MultiplyVector3(name, value);
}

void ETHEntity::InsertData(const ETHCustomDataManager &dataIn)
{
	m_properties.InsertData(dataIn);
}

void ETHEntity::MoveData(ETHCustomDataManager &dataOut) const
{
	m_properties.MoveData(dataOut);
}

bool ETHEntity::EraseData(const str_type::string &name)
{
	return m_properties.EraseData(name);
}

ETH_CUSTOM_DATA_TYPE ETHEntity::CheckCustomData(const str_type::string &name) const
{
	return m_properties.Check(name);
}

bool ETHEntity::HasCustomData() const
{
	return m_properties.HasData();
}

void ETHEntity::DebugPrintCustomData() const
{
	#ifdef GS2D_STR_TYPE_WCHAR
	std::wcout
	#else
	std::cout
	#endif
		<< m_properties.GetDebugStringData();
}

void ETHEntity::ClearCustomData()
{
	m_properties.Clear();
}