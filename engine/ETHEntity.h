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

#ifndef ETH_ENTITY_H_
#define ETH_ENTITY_H_

#include "ETHEntityProperties.h"
#include "ETHScriptEntity.h"
#include "ETHEntityController.h"

class ETHEntity : public ETHScriptEntity
{
	friend class ETHScene;
public:
	ETHEntity(const str_type::string& filePath, const int nId, const Platform::FileManagerPtr& fileManager);
	ETHEntity(TiXmlElement *pElement, const int nId);
	ETHEntity();

	virtual void Refresh(const ETHEntityProperties& properties) = 0;

	bool WriteToXMLFile(TiXmlElement *pHeadRoot) const;

	ETH_INLINE const ETHEntityProperties* GetProperties() const { return &m_properties; };
	ETHEntityControllerPtr GetController();
	void SetController(const ETHEntityControllerPtr& controller);

	// General methods
	int GetID() const;
	Vector2 ComputeOrigin(const Vector2 &v2Size) const;
	Vector3 GetLightPosition() const;
	Vector3 GetLightRelativePosition() const;
	ETH_COLLISION_BOX GetCollisionBox() const;
	void SetCollision(const bool enable);
	void ChangeEntityName(const str_type::string& name);
	str_type::string GetEntityName() const;
	std::size_t GetNumParticleSystems() const;
	ETH_ENTITY_TYPE GetType() const;
	Vector3 GetPosition() const;
	Vector2 GetPositionXY() const;
	void SetPosition(const Vector3& pos, ETHBucketManager& buckets);
	void SetPositionXY(const Vector2& pos, ETHBucketManager& buckets);
	void AddToPosition(const Vector3& pos, ETHBucketManager& buckets);
	void AddToPositionXY(const Vector2& pos, ETHBucketManager& buckets);
	void AddToAngle(const float angle);
	void SetOrphanPosition(const Vector3& pos);
	void SetOrphanPositionXY(const Vector2& pos);
	float GetAngle() const;
	void SetAngle(const float angle);
	Vector4 GetColorF() const;
	bool SetFrame(const unsigned int frame);
	bool SetFrame(const unsigned int column, const unsigned int row);
	unsigned int GetFrame() const;
	bool IsCollidable() const;
	GS_ALPHA_MODE GetBlendMode() const;
	unsigned int GetNumFrames() const;
	bool HasCallback() const;
	const ETHLight* GetLight() const;
	void TurnDynamic();
	void TurnStatic();
	bool HasSoundEffect() const;
	void Hide(const bool hide);
	Vector2 GetCurrentBucket(const ETHBucketManager& buckets) const;

	void SetEmissiveColor(const Vector3 &color);
	Vector3 GetEmissiveColor() const;
	void SetColor(const Vector3& color);
	void SetColor(const Vector4& color);
	void SetAlpha(const float alpha);
	Vector3 GetColor() const;
	Vector4 GetColorARGB() const;
	float GetAlpha() const;
	void SetShadowZ(const float z);
	float GetShadowZ() const;
	bool HasShadow() const;
	void SetLayerDepth(const float depth);
	float GetLayerDepth() const;

	bool IsRotatable() const;
	bool IsStatic() const;
	bool IsApplyLight() const;
	bool IsHidden() const;
	bool IsTemporary() const;
	bool IsCastShadow() const;
	bool IsInvisible() const;

	bool HasLightSource() const;
	bool HasParticleSystems() const;
	bool HasHalo() const;

	bool RunCallbackScript();

	float GetSpecularPower() const;
	float GetSpecularBrightness() const;
	void SetLightRange(const float range);
	float GetLightRange() const;
	void SetLightColor(const Vector3 &color);
	Vector3 GetLightColor() const;

	// Custom data related methods:
	const ETHCustomDataManager *GetCustomDataManager() const;
	void SetFloat(const str_type::string &name, const float &value);
	void SetInt(const str_type::string &name, const int &value);
	void SetUInt(const str_type::string &name, const unsigned int &value);
	void SetString(const str_type::string &name, const str_type::string &value);
	void SetVector2(const str_type::string &name, const Vector2 &value);
	void SetVector3(const str_type::string &name, const Vector3 &value);
	void AddData(const str_type::string &name, const ETHCustomDataConstPtr &dataIn);

	float GetFloat(const str_type::string &name) const;
	int GetInt(const str_type::string &name) const;
	unsigned int GetUInt(const str_type::string &name) const;
	str_type::string GetString(const str_type::string &name) const;
	Vector2 GetVector2(const str_type::string &name) const;
	Vector3 GetVector3(const str_type::string &name) const;

	void AddToFloat(const str_type::string &name, const float &value);
	void AddToInt(const str_type::string &name, const int &value);
	void AddToUInt(const str_type::string &name, const unsigned int &value);
	void AddToVector2(const str_type::string &name, const Vector2 &v);
	void AddToVector3(const str_type::string &name, const Vector3 &v);

	void MultiplyFloat(const str_type::string &name, const float &value);
	void MultiplyInt(const str_type::string &name, const int &value);
	void MultiplyUInt(const str_type::string &name, const unsigned int &value);
	void MultiplyVector2(const str_type::string &name, const float &value);
	void MultiplyVector3(const str_type::string &name, const float &value);

	bool EraseData(const str_type::string &name);
	ETH_CUSTOM_DATA_TYPE CheckCustomData(const str_type::string &name) const;
	bool HasCustomData() const;
	void DebugPrintCustomData() const;
	void ClearCustomData();
	void InsertData(const ETHCustomDataManager &dataIn);
	void MoveData(ETHCustomDataManager &dataOut) const;

	#ifdef _ETHANON_EDITOR
	inline ETHEntityProperties* GetEditableProperties() { return &m_properties; }
	#endif

private:
	void Zero();
	void SetID(const int id) { m_id = id; }
	bool ReadFromXMLFile(TiXmlElement *pElement);
	int m_id;
	
protected:
	ETHEntityControllerPtr m_controller;
	ETHEntityProperties m_properties;
	unsigned int m_spriteFrame;
	Vector4 m_v4Color;
	ETH_BOOL m_hide;
	float m_shadowZ;
};

typedef ETHEntity* ETHEntityRawPtr;

#endif