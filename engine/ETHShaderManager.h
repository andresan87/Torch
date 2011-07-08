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

#ifndef ETH_SHADER_MANAGER_H_
#define ETH_SHADER_MANAGER_H_

#include "ETHCommon.h"
#include "ETHSpriteEntity.h"
#include "ETHParallaxManager.h"
#include "ETHLightingProfile.h"
#include "ETHFakeEyePositionManager.h"

class ETHShaderManager
{
public:
	enum PROFILE
	{
		VERTEX_LIGHTING_DIFFUSE = 0,
		PIXEL_LIGHTING_DIFFUSE_SPECULAR = 1
	};

	ETHShaderManager(VideoPtr video, const str_type::string& shaderPath);

	bool BeginAmbientPass(const ETHSpriteEntity *pRender, const float maxHeight, const float minHeight);
	bool EndAmbientPass();

	bool BeginLightPass(ETHSpriteEntity *pRender, const ETHLight* light,
		const float maxHeight, const float minHeight, const float lightIntensity,
		const ETHSpriteEntity *pParent = 0, const bool drawToTarget = false);
	bool EndLightPass();

	bool BeginShadowPass(const ETHSpriteEntity *pRender, const ETHLight* light, const float maxHeight, const float minHeight);
	bool EndShadowPass();

	bool BeginHaloPass(const ETHLight* light, const float maxHeight, const ETHSpriteEntity *pParent = 0);
	bool EndHaloPass();

	bool BeginParticlePass();
	bool EndParticlePass();

	void UsePS(const bool usePS);
	bool IsUsingPixelShader();
	bool IsPixelLightingSupported();

	SpritePtr GetProjShadow();

	inline void SetParallaxNormalizedOrigin(const Vector2 &normalizedOrigin) { m_parallaxManager.SetNormalizedOrigin(normalizedOrigin); }
	inline Vector2 GetParallaxNormalizedOrigin() const { return m_parallaxManager.GetNormalizedOrigin(); }
	inline void SetParallaxIntensity(const float intensity) { m_parallaxManager.SetIntensity(intensity); }
	inline float GetParallaxIntensity() const { return m_parallaxManager.GetIntensity(); }
	inline Vector2 ComputeParallaxOffset(const VideoPtr video, const Vector3 &pos) const { return m_parallaxManager.ComputeOffset(video, pos); }

private:

	ETHLightingProfilePtr FindHighestLightingProfile();

	ETHParallaxManager m_parallaxManager;
	ETHFakeEyePositionManagerPtr m_fakeEyeManager;

	std::map<PROFILE, ETHLightingProfilePtr> m_lightingProfiles;
	ETHLightingProfilePtr m_currentProfile;
	VideoPtr m_video;
	GS_ALPHA_MODE m_lastAM;
	SpritePtr m_projShadow;
	ShaderPtr m_shadowVS;
	ShaderPtr m_defaultVS;
	ShaderPtr m_particle;
	ShaderPtr m_defaultStaticAmbientVS;
	ShaderPtr m_verticalStaticAmbientVS;
};

typedef boost::shared_ptr<ETHShaderManager> ETHShaderManagerPtr;

#endif