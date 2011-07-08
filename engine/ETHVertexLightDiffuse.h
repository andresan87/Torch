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

#ifndef ETH_VERTEX_LIGHT_DIFFUSE_H_
#define ETH_VERTEX_LIGHT_DIFFUSE_H_

#include "ETHLightingProfile.h"

class ETHVertexLightDiffuse : public ETHLightingProfile
{
public:
	ETHVertexLightDiffuse(VideoPtr video, const str_type::string& shaderPath);

	bool BeginLightPass(ETHSpriteEntity *pRender, Vector3 &v3LightPos, const Vector2 &v2Size,
		const ETHLight* light, const float maxHeight, const float minHeight, const float lightIntensity,
		const bool drawToTarget);
	bool EndLightPass();

	bool IsSupportedByHardware() const;
	bool IsUsingPixelShader() const;
	SpritePtr GetDefaultNormalMap();

private:
	static const GS_SHADER_PROFILE m_profile;

	VideoPtr m_video;
	GS_ALPHA_MODE m_lastAM;
	ShaderPtr m_hVertexLightVS;
	ShaderPtr m_vVertexLightVS;
};

#endif