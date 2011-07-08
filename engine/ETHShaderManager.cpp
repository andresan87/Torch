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

#include "ETHShaderManager.h"
#include "ETHScene.h"
#include "ETHPixelLightDiffuseSpecular.h"
#include "ETHVertexLightDiffuse.h"
#include "ETHShaders.h"

ETHShaderManager::ETHShaderManager(VideoPtr video, const str_type::string& shaderPath) :
	m_lastAM(GSAM_PIXEL), m_fakeEyeManager(new ETHFakeEyePositionManager)
{
	m_video = video;

	m_defaultVS = m_video->LoadShaderFromFile(ETHGlobal::GetDataResourceFullPath(shaderPath, ETHShaders::DefaultVS()).c_str(), GSSF_VERTEX, GSSP_MODEL_2);
	m_particle  = m_video->LoadShaderFromFile(ETHGlobal::GetDataResourceFullPath(shaderPath, ETHShaders::Particle_VS()).c_str(), GSSF_VERTEX, GSSP_MODEL_2, "particle");
	m_defaultStaticAmbientVS  = m_video->LoadShaderFromFile(ETHGlobal::GetDataResourceFullPath(shaderPath, ETHShaders::Ambient_VS_Hor()).c_str(), GSSF_VERTEX, GSSP_MODEL_2);
	m_verticalStaticAmbientVS = m_video->LoadShaderFromFile(ETHGlobal::GetDataResourceFullPath(shaderPath, ETHShaders::Ambient_VS_Ver()).c_str(), GSSF_VERTEX, GSSP_MODEL_2, "vertical");
	m_shadowVS = m_video->LoadShaderFromFile(ETHGlobal::GetDataResourceFullPath(shaderPath, ETHShaders::Shadow_VS_Ver()).c_str(), GSSF_VERTEX, GSSP_MODEL_2);

	// TODO/TO-DO: use a wider macro here
	#ifndef ANDROID
		m_projShadow = m_video->CreateSprite(ETHGlobal::GetDataResourceFullPath(shaderPath, GS_L("shadow.dds")));
	#else
		m_projShadow = m_video->CreateSprite(ETHGlobal::GetDataResourceFullPath(shaderPath, GS_L("shadow.png")));
	#endif

	{
		ETHLightingProfilePtr profile(new ETHVertexLightDiffuse(m_video, shaderPath));
		if (profile->IsSupportedByHardware())
		{
			m_lightingProfiles[VERTEX_LIGHTING_DIFFUSE] = profile;
		}
	}
	{
		ETHLightingProfilePtr profile(new ETHPixelLightDiffuseSpecular(m_video, shaderPath, m_fakeEyeManager));
		if (profile->IsSupportedByHardware())
		{
			m_lightingProfiles[PIXEL_LIGHTING_DIFFUSE_SPECULAR] = profile;
		}
	}

	if (m_lightingProfiles.empty())
	{
		video->Message(GS_L("ETHShaderManager::ETHShaderManager: no lighting profile"), GSMT_WARNING);
	}
	else
	{
		m_currentProfile = FindHighestLightingProfile();
	}
}

ETHLightingProfilePtr ETHShaderManager::FindHighestLightingProfile()
{
	if (m_lightingProfiles.empty())
	{
		m_video->Message(GS_L("ETHShaderManager::FindHighestLightingProfile: no lighting profile"), GSMT_WARNING);
		return ETHLightingProfilePtr();
	}
	return m_lightingProfiles.rbegin()->second;
}

SpritePtr ETHShaderManager::GetProjShadow()
{
	return m_projShadow;
}

bool ETHShaderManager::BeginAmbientPass(const ETHSpriteEntity *pRender, const float maxHeight, const float minHeight)
{
	m_video->SetPixelShader(ShaderPtr());

	if (pRender->GetType() == ETH_VERTICAL)
	{
		m_verticalStaticAmbientVS->SetConstant(GS_L("spaceLength"), (maxHeight-minHeight));
		m_video->SetVertexShader(m_verticalStaticAmbientVS);
	}
	else
	{
		m_video->SetVertexShader(m_defaultStaticAmbientVS);
	}
	m_parallaxManager.SetShaderParameters(m_video, m_video->GetVertexShader(), pRender->GetPosition(), false);

	m_lastAM = m_video->GetAlphaMode();
	return true;
}

bool ETHShaderManager::EndAmbientPass()
{
	m_video->SetPixelShader(ShaderPtr());
	m_video->SetVertexShader(ShaderPtr());
	m_video->SetAlphaMode(m_lastAM);
	return true;
}

bool ETHShaderManager::BeginLightPass(ETHSpriteEntity *pRender, const ETHLight* light,
									  const float maxHeight, const float minHeight, const float lightIntensity,
									  const ETHSpriteEntity *pParent, const bool drawToTarget)
{
	if (!light || !pRender->IsApplyLight())
		return false;

	Vector3 v3LightPos;
	if (pParent)
		v3LightPos = pParent->GetPosition() + light->pos;
	else
		v3LightPos = light->pos;

	const Vector2 &v2Size = pRender->GetCurrentSize();
	const float size = Max(v2Size.x, v2Size.y);
	const float distance = SquaredDistance(pRender->GetPosition(), v3LightPos);
	const float radius = (light->range + size);
	if (distance > radius * radius)
		return false;

	m_currentProfile->BeginLightPass(pRender, v3LightPos, v2Size, light, maxHeight, minHeight, lightIntensity, drawToTarget);
	m_parallaxManager.SetShaderParameters(m_video, m_video->GetVertexShader(), pRender->GetPosition(), drawToTarget);
	return true;
}

bool ETHShaderManager::EndLightPass()
{
	m_video->SetPixelShader(ShaderPtr());
	m_video->SetVertexShader(ShaderPtr());
	m_video->SetAlphaMode(m_lastAM);
	return true;
}

bool ETHShaderManager::BeginShadowPass(const ETHSpriteEntity *pRender, const ETHLight* light, const float maxHeight, const float minHeight)
{
	if (!light || !light->castShadows || !pRender->IsCastShadow()/* || pRender->GetType() != ETH_VERTICAL*/)
		return false;

	m_lastAM = m_video->GetAlphaMode();
	m_video->SetAlphaMode(GSAM_PIXEL);
	m_video->SetVertexShader(m_shadowVS);

	m_shadowVS->SetConstant(GS_L("lightRange"), light->range);
	m_video->SetSpriteDepth(((pRender->GetPosition().z + _ETH_SMALL_NUMBER-minHeight)/(maxHeight-minHeight)));
	m_video->SetPixelShader(ShaderPtr());
	return true;
}

bool ETHShaderManager::EndShadowPass()
{
	m_video->SetPixelShader(ShaderPtr());
	m_video->SetVertexShader(ShaderPtr());
	m_video->SetAlphaMode(m_lastAM);
	return true;
}

bool ETHShaderManager::BeginHaloPass(const ETHLight* light, const float maxHeight, const ETHSpriteEntity *pParent)
{
	GS2D_UNUSED_ARGUMENT(maxHeight);
	GS2D_UNUSED_ARGUMENT(pParent);
	if (!light || light->haloBitmap == GS_L(""))
		return false;

	m_lastAM = m_video->GetAlphaMode();
	m_video->SetAlphaMode(GSAM_ADD);
	m_video->SetVertexShader(m_defaultVS);
	m_video->SetPixelShader(ShaderPtr());
	return true;
}

bool ETHShaderManager::EndHaloPass()
{
	m_video->SetPixelShader(ShaderPtr());
	m_video->SetVertexShader(ShaderPtr());
	m_video->SetAlphaMode(m_lastAM);
	return true;
}

bool ETHShaderManager::BeginParticlePass()
{
	m_video->SetVertexShader(m_particle);
	m_video->SetPixelShader(ShaderPtr());
	return true;
}

bool ETHShaderManager::EndParticlePass()
{
	m_video->SetPixelShader(ShaderPtr());
	m_video->SetVertexShader(ShaderPtr());
	return true;
}


void ETHShaderManager::UsePS(const bool usePS)
{
	if (!m_lightingProfiles.empty())
	{
		if (usePS)
			m_currentProfile = FindHighestLightingProfile();
		else
			m_currentProfile = m_lightingProfiles.find(VERTEX_LIGHTING_DIFFUSE)->second;
	}
	else
	{
		m_video->Message(GS_L("ETHShaderManager::UsePS: no lighting profile"), GSMT_WARNING);
	}
}

bool ETHShaderManager::IsUsingPixelShader()
{
	if (m_lightingProfiles.empty())
	{
		m_video->Message(GS_L("ETHShaderManager::IsUsingPixelShader: no lighting profile"), GSMT_WARNING);
		return false;
	}
	return m_currentProfile->IsUsingPixelShader();
}

bool ETHShaderManager::IsPixelLightingSupported()
{
	if (m_lightingProfiles.empty())
	{
		m_video->Message(GS_L("ETHShaderManager::IsPixelLightingSupported: no lighting profile"), GSMT_WARNING);
		return false;
	}
	return (m_lightingProfiles.find(PIXEL_LIGHTING_DIFFUSE_SPECULAR) != m_lightingProfiles.end());
}