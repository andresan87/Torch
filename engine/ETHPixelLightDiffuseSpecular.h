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

#ifndef ETH_PIXEL_LIGHT_DIFFUSE_SPECULAR_H_
#define ETH_PIXEL_LIGHT_DIFFUSE_SPECULAR_H_

#include "ETHLightingProfile.h"
#include "ETHFakeEyePositionManager.h"

class ETHPixelLightDiffuseSpecular : public ETHLightingProfile
{
public:
	ETHPixelLightDiffuseSpecular(VideoPtr video, const str_type::string& shaderPath, ETHFakeEyePositionManagerPtr fakeEyeManager);

	bool BeginLightPass(ETHSpriteEntity *pRender, Vector3 &v3LightPos, const Vector2 &v2Size,
		const ETHLight* light, const float maxHeight, const float minHeight, const float lightIntensity,
		const bool drawToTarget = false);
	bool EndLightPass();

	bool IsSupportedByHardware() const;
	bool IsUsingPixelShader() const;
	SpritePtr GetDefaultNormalMap();

private:
	VideoPtr m_video;
	SpritePtr m_defaultNM;
	static const GS_SHADER_PROFILE m_profile;
	ETHFakeEyePositionManagerPtr m_fakeEyeManager;
	GS_ALPHA_MODE m_lastAM;
	ShaderPtr m_vPixelLightPS;
	ShaderPtr m_hPixelLightPS;
	ShaderPtr m_hPixelLightSpecularPS;
	ShaderPtr m_vPixelLightSpecularPS;
	ShaderPtr m_pixelLightVS;
	ShaderPtr m_verticalPixelLightVS;
};

#endif