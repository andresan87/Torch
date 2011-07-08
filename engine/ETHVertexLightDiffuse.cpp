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

#include "ETHVertexLightDiffuse.h"
#include "ETHShaders.h"

const GS_SHADER_PROFILE ETHVertexLightDiffuse::m_profile = GSSP_MODEL_2;

ETHVertexLightDiffuse::ETHVertexLightDiffuse(VideoPtr video, const str_type::string& shaderPath)
{
	m_video = video;
	m_hVertexLightVS = m_video->LoadShaderFromFile(ETHGlobal::GetDataResourceFullPath(shaderPath, ETHShaders::VL_VS_Hor_Diff()).c_str(), GSSF_VERTEX, GSSP_MODEL_2, "sprite_pvl");
	m_vVertexLightVS = m_video->LoadShaderFromFile(ETHGlobal::GetDataResourceFullPath(shaderPath, ETHShaders::VL_VS_Ver_Diff()).c_str(), GSSF_VERTEX, GSSP_MODEL_2, "sprite_pvl");
}

bool ETHVertexLightDiffuse::BeginLightPass(ETHSpriteEntity *pRender, Vector3 &v3LightPos, const Vector2 &v2Size,
	const ETHLight* light, const float maxHeight, const float minHeight, const float lightIntensity,
	const bool drawToTarget)
{
	GS2D_UNUSED_ARGUMENT(drawToTarget);
	const Vector2 &v2Origin = pRender->ComputeOrigin(v2Size);
	const Vector3 &v3EntityPos = pRender->GetPosition();

	m_video->SetPixelShader(ShaderPtr());

	ShaderPtr pLightShader;

	if (pRender->GetType() == ETH_VERTICAL)
	{
		m_vVertexLightVS->SetConstant(GS_L("spaceLength"), (maxHeight-minHeight));
		m_vVertexLightVS->SetConstant(GS_L("topLeft3DPos"), v3EntityPos-(Vector3(v2Origin.x, 0, -v2Origin.y)));
		pLightShader = m_vVertexLightVS;
	}
	else
	{
		m_hVertexLightVS->SetConstant(GS_L("topLeft3DPos"), v3EntityPos-Vector3(v2Origin, 0));
		pLightShader = m_hVertexLightVS;
	}
	m_video->SetVertexShader(pLightShader);

	m_lastAM = m_video->GetAlphaMode();
	m_video->SetAlphaMode(GSAM_ADD);

	// Set a depth value depending on the entity type
	pRender->SetDepth(maxHeight, minHeight);
 
	pLightShader->SetConstant(GS_L("pivotAdjust"), pRender->GetProperties()->pivotAdjust);
	pLightShader->SetConstant(GS_L("lightPos"), v3LightPos);
	pLightShader->SetConstant(GS_L("lightRange"), light->range);
	pLightShader->SetConstant(GS_L("lightColor"), light->color);
	pLightShader->SetConstant(GS_L("lightIntensity"), lightIntensity);
	return true;
}

bool ETHVertexLightDiffuse::EndLightPass()
{
	m_video->SetPixelShader(ShaderPtr());
	m_video->SetVertexShader(ShaderPtr());
	m_video->SetAlphaMode(m_lastAM);
	return true;
}

bool ETHVertexLightDiffuse::IsSupportedByHardware() const
{
	return (m_video->GetHighestVertexProfile() >= m_profile);
}

bool ETHVertexLightDiffuse::IsUsingPixelShader() const
{
	return false;
}

SpritePtr ETHVertexLightDiffuse::GetDefaultNormalMap()
{
	return SpritePtr();
}
