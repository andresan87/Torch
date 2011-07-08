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

#ifndef ETH_SCRIPT_ENTITY_H_
#define ETH_SCRIPT_ENTITY_H_

#include "ETHCommon.h"
#include "ETHSceneProperties.h"
#include "ETHCustomDataManager.h"
#include "ETHBucketManager.h"

class ETHScriptEntity
{
	bool m_isAlive;

protected:
	ETHScriptEntity();
	mutable int m_ref;

public:
	virtual ETH_ENTITY_TYPE GetType() const = 0;
	virtual str_type::string GetEntityName() const = 0;
	virtual Vector3 GetPosition() const = 0;
	virtual Vector2 GetPositionXY() const = 0;
	virtual float GetAngle() const = 0;
	virtual void SetAngle(const float angle) = 0;
	virtual void SetPosition(const Vector3& pos, ETHBucketManager& buckets) = 0;
	virtual void SetPositionXY(const Vector2& pos, ETHBucketManager& buckets) = 0;
	virtual bool SetFrame(const unsigned int frame) = 0;
	virtual bool SetFrame(const unsigned int column, const unsigned int row) = 0;
	virtual unsigned int GetFrame() const = 0;
	virtual Vector2 GetCurrentSize() const = 0;
	virtual bool HasLightSource() const = 0;
	virtual bool HasHalo() const = 0;
	virtual bool IsCollidable() const = 0;
	virtual unsigned int GetNumFrames() const = 0;
	virtual bool IsStatic() const = 0;
	virtual bool IsTemporary() const = 0;
	virtual bool IsInvisible() const = 0;
	virtual Vector3 GetLightPosition() const = 0;
	virtual void Hide(const bool hide) = 0;
	virtual bool IsHidden() const = 0;
	virtual ETH_VIEW_RECT GetScreenRect(const ETHSceneProperties& sceneProps) const = 0;
	virtual Vector2 GetScreenRectMin(const ETHSceneProperties& sceneProps) const = 0;
	virtual Vector2 GetScreenRectMax(const ETHSceneProperties& sceneProps) const = 0;
	virtual int GetID() const = 0;
	virtual void TurnDynamic() = 0;
	virtual void TurnStatic() = 0;
	virtual void SilenceParticleSystems(const bool silence) = 0;
	virtual void KillParticleSystem(const unsigned int n) = 0;
	virtual bool ParticlesKilled(const unsigned int n) const = 0;
	virtual bool PlayParticleSystem(const unsigned int n, const Vector2& zAxisDirection) = 0;
	virtual bool AreParticlesOver() const = 0;
	virtual void SetSoundVolume(const float volume) = 0;
	virtual ETH_COLLISION_BOX GetCollisionBox() const = 0;
	virtual void SetCollision(const bool enable) = 0;
	virtual bool HasParticleSystems() const = 0;
	virtual bool HasParticleSystem(const unsigned int n) const = 0;
	virtual bool HasSoundEffect() const = 0;
	virtual void SetShadowZ(const float z) = 0;
	virtual float GetShadowZ() const = 0;
	virtual bool HasShadow() const = 0;
	virtual void AddToPosition(const Vector3& pos, ETHBucketManager& buckets) = 0;
	virtual void AddToPositionXY(const Vector2& pos, ETHBucketManager& buckets) = 0;
	virtual void AddToAngle(const float angle) = 0;
	virtual bool SetSpriteCut(const unsigned int col, const unsigned int row) = 0;
	virtual void SetParticlePosition(const unsigned int n, const Vector3 &v3Pos) = 0;
	virtual void ScaleParticleSystem(const unsigned int n, const float scale) = 0;
	virtual bool MirrorParticleSystemX(const unsigned int n, const bool mirrorGravity) = 0;
	virtual bool MirrorParticleSystemY(const unsigned int n, const bool mirrorGravity) = 0;
	virtual void ForceSFXStop() = 0;

	virtual void SetLightRange(const float range) = 0;
	virtual float GetLightRange() const = 0;
	virtual void SetLightColor(const Vector3 &color) = 0;
	virtual Vector3 GetLightColor() const = 0;
	virtual void SetEmissiveColor(const Vector3 &color) = 0;
	virtual Vector3 GetEmissiveColor() const = 0;
	virtual void SetColor(const Vector3 &color) = 0;
	virtual void SetAlpha(const float alpha) = 0;
	virtual Vector3 GetColor() const = 0;
	virtual Vector4 GetColorARGB() const = 0;
	virtual float GetAlpha() const = 0;
	virtual void SetLayerDepth(const float depth) = 0;
	virtual float GetLayerDepth() const = 0;
	virtual void RecoverResources() = 0;

	virtual bool SetSprite(const str_type::string &fileName) = 0;
	virtual bool SetNormal(const str_type::string &fileName) = 0;
	virtual bool SetGloss(const str_type::string &fileName) = 0;

	virtual void SetFloat(const str_type::string &name, const float &value) = 0;
	virtual void SetInt(const str_type::string &name, const int &value) = 0;
	virtual void SetUInt(const str_type::string &name, const unsigned int &value) = 0;
	virtual void SetString(const str_type::string &name, const str_type::string &value) = 0;
	virtual void SetVector2(const str_type::string &name, const Vector2 &value) = 0;
	virtual void SetVector3(const str_type::string &name, const Vector3 &value) = 0;

	virtual float GetFloat(const str_type::string &name) const = 0;
	virtual int GetInt(const str_type::string &name) const = 0;
	virtual unsigned int GetUInt(const str_type::string &name) const = 0;
	virtual str_type::string GetString(const str_type::string &name) const = 0;
	virtual Vector2 GetVector2(const str_type::string &name) const = 0;
	virtual Vector3 GetVector3(const str_type::string &name) const = 0;

	virtual void AddToFloat(const str_type::string &name, const float &value) = 0;
	virtual void AddToInt(const str_type::string &name, const int &value) = 0;
	virtual void AddToUInt(const str_type::string &name, const unsigned int &value) = 0;
	virtual void AddToVector2(const str_type::string &name, const Vector2 &v) = 0;
	virtual void AddToVector3(const str_type::string &name, const Vector3 &v) = 0;

	virtual void MultiplyFloat(const str_type::string &name, const float &value) = 0;
	virtual void MultiplyInt(const str_type::string &name, const int &value) = 0;
	virtual void MultiplyUInt(const str_type::string &name, const unsigned int &value) = 0;
	virtual void MultiplyVector2(const str_type::string &name, const float &value) = 0;
	virtual void MultiplyVector3(const str_type::string &name, const float &value) = 0;

	virtual bool EraseData(const str_type::string &name) = 0;
	virtual ETH_CUSTOM_DATA_TYPE CheckCustomData(const str_type::string &name) const = 0;
	virtual void DebugPrintCustomData() const = 0;
	virtual bool HasCustomData() const = 0;
	virtual void ClearCustomData() = 0;

	virtual void AddRef() = 0;
	virtual void Release() = 0;
	void Kill();
	bool IsAlive() const;
};

#endif