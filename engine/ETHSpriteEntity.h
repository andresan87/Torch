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

#ifndef ETH_SPRITE_ENTITY_H_
#define ETH_SPRITE_ENTITY_H_

#include "ETHEntity.h"
#include "ETHResourceProvider.h"
#include "ETHSceneProperties.h"

class ETHSpriteEntity : public ETHEntity
{
public:
	ETHSpriteEntity(const str_type::string& filePath, ETHResourceProviderPtr provider, const int nId =-1);
	ETHSpriteEntity(TiXmlElement *pElement, ETHResourceProviderPtr provider, const int nId =-1);
	ETHSpriteEntity(ETHResourceProviderPtr provider, const ETHEntityProperties& properties);
	ETHSpriteEntity(ETHResourceProviderPtr provider);

	void Refresh(const ETHEntityProperties& properties);

	void AddRef();
	void Release();

	bool SetDepth(const float maxHeight, const float minHeight);
	void SetOrigin();

	SpritePtr GetSprite();
	SpritePtr GetGloss();
	SpritePtr GetNormal();
	SpritePtr GetLightmap();
	SpritePtr GetHalo();
	SpritePtr GetParticleBMP(const unsigned int n);
	AudioSamplePtr GetParticleSFX(const unsigned int n);

	float GetMaxHeight();
	float GetMinHeight();

	void DestroyParticleSystem(const unsigned int n);
	void SetParticleBMP(const int unsigned n, SpritePtr pBMP);
	void SetParticleSFX(const int unsigned n, AudioSamplePtr pSample);
	void SetParticlePosition(const unsigned int n, const Vector3 &v3Pos);
	void ScaleParticleSystem(const unsigned int n, const float scale);
	bool MirrorParticleSystemX(const unsigned int n, const bool mirrorGravity);
	bool MirrorParticleSystemY(const unsigned int n, const bool mirrorGravity);
	bool HasParticleSystem(const unsigned int n) const;

	ETHParticleManagerPtr GetParticleManager(const std::size_t n);
	bool AreParticlesOver() const;
	void UpdateParticleSystems(const Vector2& zAxisDirection);
	void SetStopSFXWhenDestroyed(const bool enable);
	void SilenceParticleSystems(const bool silence);
	bool PlayParticleSystem(const unsigned int n, const Vector2& zAxisDirection);
	void KillParticleSystem(const unsigned int n);
	bool ParticlesKilled(const unsigned int n) const;
	void SetSoundVolume(const float volume);
	bool SetSpriteCut(const unsigned int col, const unsigned int row);

	Vector2 GetCurrentSize() const;
	Vector2 ComputeParallaxOffset() const;
	float ComputeHaloAngle() const;
	float ComputeDepth(const float maxHeight, const float minHeight);

	ETH_VIEW_RECT GetScreenRect(const ETHSceneProperties& sceneProps) const;
	Vector2 GetScreenRectMin(const ETHSceneProperties& sceneProps) const;
	Vector2 GetScreenRectMax(const ETHSceneProperties& sceneProps) const;
	Vector2 GetScreenPosition(const ETHSceneProperties& sceneProps) const;

	bool SetSprite(const str_type::string &fileName);
	bool SetNormal(const str_type::string &fileName);
	bool SetGloss(const str_type::string &fileName);
	bool SetHalo(const str_type::string &fileName);

	void ForceSFXStop();
	void StartSFX();

	void LoadParticleSystem();

	void RecoverResources();

protected:
	void Create();
	void Zero();

	std::vector<ETHParticleManagerPtr> m_particles;

	bool m_stopSFXWhenDestroyed;
	ETHResourceProviderPtr m_provider;
	SpritePtr m_pSprite;
	SpritePtr m_pGloss;
	SpritePtr m_pNormal;
	SpritePtr m_pHalo;
	SpritePtr m_pLightmap;

	static const float m_layrableMinimumDepth;
	static const float m_haloRotation;
};

#endif