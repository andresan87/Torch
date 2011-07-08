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

#include "ETHPixelLightDiffuseSpecular.h"
#include "ETHShaders.h"

const GS_SHADER_PROFILE ETHPixelLightDiffuseSpecular::m_profile = GSSP_MODEL_2;

ETHPixelLightDiffuseSpecular::ETHPixelLightDiffuseSpecular(VideoPtr video, const str_type::string& shaderPath, ETHFakeEyePositionManagerPtr fakeEyeManager)
	: m_fakeEyeManager(fakeEyeManager)
{
	m_video = video;
	if (IsSupportedByHardware())
	{
		m_hPixelLightPS = m_video->LoadShaderFromFile(ETHGlobal::GetDataResourceFullPath(shaderPath, ETHShaders::PL_PS_Hor_Diff()).c_str(), GSSF_PIXEL, m_profile, "main");
		m_vPixelLightPS = m_video->LoadShaderFromFile(ETHGlobal::GetDataResourceFullPath(shaderPath, ETHShaders::PL_PS_Ver_Diff()).c_str(), GSSF_PIXEL, m_profile, "main");
		m_pixelLightVS         = m_video->LoadShaderFromFile(ETHGlobal::GetDataResourceFullPath(shaderPath, ETHShaders::PL_VS_Hor_Light()).c_str(), GSSF_VERTEX, GSSP_MODEL_2, "sprite_ppl");
		m_verticalPixelLightVS = m_video->LoadShaderFromFile(ETHGlobal::GetDataResourceFullPath(shaderPath, ETHShaders::PL_VS_Ver_Light()).c_str(), GSSF_VERTEX, GSSP_MODEL_2, "verticalSprite_ppl");
		m_hPixelLightSpecularPS = m_video->LoadShaderFromFile(ETHGlobal::GetDataResourceFullPath(shaderPath, ETHShaders::PL_PS_Hor_Spec()).c_str(), GSSF_PIXEL, m_profile, "mainSpecular");
		m_vPixelLightSpecularPS = m_video->LoadShaderFromFile(ETHGlobal::GetDataResourceFullPath(shaderPath, ETHShaders::PL_PS_Ver_Spec()).c_str(), GSSF_PIXEL, m_profile, "mainSpecular");
		m_defaultNM = m_video->CreateSprite(ETHGlobal::GetDataResourceFullPath(shaderPath, GS_L("default_nm.png")));
	}
}

bool ETHPixelLightDiffuseSpecular::BeginLightPass(ETHSpriteEntity *pRender, Vector3 &v3LightPos, const Vector2 &v2Size,
	const ETHLight* light, const float maxHeight, const float minHeight, const float lightIntensity,
	const bool drawToTarget)
{
	const Vector2 &v2Origin = pRender->ComputeOrigin(v2Size);
	const Vector3 &v3EntityPos = pRender->GetPosition();

	// set the correct light shader
	ShaderPtr pLightShader;
	const bool hasGloss = pRender->GetGloss();
	if (pRender->GetType() == ETH_VERTICAL)
	{
		if (hasGloss)
		{
			pLightShader = m_vPixelLightSpecularPS;
		}
		else
		{
			pLightShader = m_vPixelLightPS;
		}
	}
	else
	{
		if (hasGloss)
		{
			pLightShader = m_hPixelLightSpecularPS;
		}
		else
		{
			pLightShader = m_hPixelLightPS;
		}
	}

	// if it has a gloss map, send specular data to shader
	if (hasGloss)
	{
		pLightShader->SetConstant(GS_L("specularPower"), pRender->GetSpecularPower());
		pLightShader->SetConstant(GS_L("specularBrightness"), pRender->GetSpecularBrightness());
		pLightShader->SetTexture(GS_L("glossMap"), pRender->GetGloss()->GetTexture());
		pLightShader->SetConstant(GS_L("fakeEyePos"), m_fakeEyeManager->ComputeFakeEyePosition(m_video, pLightShader, drawToTarget, v3LightPos, pRender->GetAngle()));
	}

	// choose which normalmap to use
	m_video->SetPixelShader(pLightShader);
	if (pRender->GetNormal())
	{
		pLightShader->SetTexture(GS_L("normalMap"), pRender->GetNormal()->GetTexture());
	}
	else
	{
		pLightShader->SetTexture(GS_L("normalMap"), GetDefaultNormalMap()->GetTexture());
	}

	// sets spatial information to the shader
	if (pRender->GetType() == ETH_VERTICAL)
	{
		m_verticalPixelLightVS->SetConstant(GS_L("spaceLength"), (maxHeight-minHeight));
		m_verticalPixelLightVS->SetConstant(GS_L("topLeft3DPos"), v3EntityPos-(Vector3(v2Origin.x,0,-v2Origin.y)));
		m_video->SetVertexShader(m_verticalPixelLightVS);
	}
	else
	{
		m_pixelLightVS->SetConstant(GS_L("topLeft3DPos"), v3EntityPos-Vector3(v2Origin,0));
		m_video->SetVertexShader(m_pixelLightVS);
	}

	const Vector2 v2ScreenDim = (drawToTarget) ? pRender->GetLightmap()->GetBitmapSizeF() : m_video->GetScreenSizeF();
	pLightShader->SetConstant(GS_L("screenSize"), v2ScreenDim);

	// const ETH_ENTITY_DATA *pData = pRender->GetData();
	if (pRender->GetType() != ETH_VERTICAL)
	{
		if (pRender->GetAngle() != 0.0f)
		{
			Vector3 newPos = v3LightPos-v3EntityPos;
			Matrix4x4 matRot = RotateZ(-DegreeToRadian(pRender->GetAngle()));
			
			newPos = Multiply(newPos, matRot);
			v3LightPos = newPos + v3EntityPos;
		}
		else
		{
			Matrix4x4 matRot;
			pLightShader->SetMatrixConstant(GS_L("rotMatrix"), matRot);
		}
	}

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

bool ETHPixelLightDiffuseSpecular::EndLightPass()
{
	m_video->SetPixelShader(ShaderPtr());
	m_video->SetVertexShader(ShaderPtr());
	m_video->SetAlphaMode(m_lastAM);
	return true;
}

bool ETHPixelLightDiffuseSpecular::IsSupportedByHardware() const
{
	return (m_video->GetHighestPixelProfile() >= m_profile);
}

bool ETHPixelLightDiffuseSpecular::IsUsingPixelShader() const
{
	return true;
}

SpritePtr ETHPixelLightDiffuseSpecular::GetDefaultNormalMap()
{
	return m_defaultNM;
}

