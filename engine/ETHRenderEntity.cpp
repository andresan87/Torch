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

#include "ETHRenderEntity.h"
#include "ETHShaderManager.h"

ETHRenderEntity::ETHRenderEntity(const str_type::string& filePath, ETHResourceProviderPtr provider, const int nId) :
	ETHSpriteEntity(filePath, provider, nId)
{
}

ETHRenderEntity::ETHRenderEntity(TiXmlElement *pElement, ETHResourceProviderPtr provider, const int nId) :
	ETHSpriteEntity(pElement, provider, nId)
{
}

ETHRenderEntity::ETHRenderEntity(ETHResourceProviderPtr provider, const ETHEntityProperties& properties) :
	ETHSpriteEntity(provider, properties)
{
}

ETHRenderEntity::ETHRenderEntity(ETHResourceProviderPtr provider) :
	ETHSpriteEntity(provider)
{
}

bool ETHRenderEntity::DrawAmbientPass(const float maxHeight, const float minHeight,
									  const bool enableLightmaps, const ETHSceneProperties& sceneProps)
{
	if (!m_pSprite)
		return false;

	Vector4 v4FinalAmbient = Vector4(sceneProps.ambient, 1.0f);
	v4FinalAmbient.x = Min(1.0f, v4FinalAmbient.x + m_properties.emissiveColor.x);
	v4FinalAmbient.y = Min(1.0f, v4FinalAmbient.y + m_properties.emissiveColor.y);
	v4FinalAmbient.z = Min(1.0f, v4FinalAmbient.z + m_properties.emissiveColor.z);
	v4FinalAmbient = v4FinalAmbient * m_v4Color;

	SetDepth(maxHeight, minHeight);

	const bool applyLightmap = (m_pLightmap && enableLightmaps && IsStatic());
	if (applyLightmap)
	{
		m_pLightmap->SetAsTexture(1);
	}

	const VideoPtr& video = m_provider->GetVideo();

	// sets the alpha mode according to the entity's property
	video->SetAlphaMode((GetType() != ETH_OPAQUE_DECAL) ? GetBlendMode() : GSAM_ALPHA_TEST);
	m_pSprite->SetRect(m_spriteFrame);
	SetOrigin();

	const Vector2 pos = ETHGlobal::ToScreenPos(GetPosition(), sceneProps.zAxisDirection);
	m_pSprite->DrawOptimal(pos,	ConvertToDW(v4FinalAmbient), (m_properties.type == ETH_VERTICAL) ? 0.0f : GetAngle());

	if (applyLightmap)
	{
		video->UnsetTexture(1);
	}
	return true;
}

bool ETHRenderEntity::DrawLightPass(const Vector2 &zAxisDirection, const bool drawToTarget)
{
	if (!m_pSprite)
		return false;

	m_pSprite->SetRect(m_spriteFrame);
	SetOrigin();
	m_pSprite->Draw(ETHGlobal::ToScreenPos(GetPosition(), zAxisDirection),
		ConvertToDW(GetColorF()), (!IsRotatable() || drawToTarget) ? 0.0f : GetAngle());
	return true;
}

bool ETHRenderEntity::DrawShadow(const float maxHeight, const float minHeight, const ETHSceneProperties& sceneProps, const ETHLight& light, ETHSpriteEntity *pParent,
								 const bool maxOpacity, const bool drawToTarget, const float targetAngle, const Vector3& v3TargetPos)
{
	return DrawProjShadow(maxHeight, minHeight, sceneProps, light, pParent, maxOpacity, drawToTarget, targetAngle, v3TargetPos);
}

bool ETHRenderEntity::DrawProjShadow(const float maxHeight, const float minHeight, const ETHSceneProperties& sceneProps, const ETHLight& light, ETHSpriteEntity *pParent,
									 const bool maxOpacity, const bool drawToTarget, const float targetAngle, const Vector3& v3TargetPos)
{
	if (!m_pSprite)
		return false;

	VideoPtr video = m_provider->GetVideo();
	ETHShaderManagerPtr shaderManager = m_provider->GetShaderManager();
	SpritePtr pShadow = shaderManager->GetProjShadow();

	Vector3 v3LightPos;
	Vector3 v3ParentPos(0,0,0);

	const Vector3 v3EntityPos = GetPosition();

	if (pParent)
	{
		v3ParentPos = pParent->GetPosition();
		v3LightPos = Vector3(v3ParentPos.x, v3ParentPos.y, 0) + light.pos;
	}
	else
	{
		v3LightPos = light.pos;
	}

	// if the object is higher than the light, then the shadow shouldn't be cast on the floor
	if (v3LightPos.z < v3EntityPos.z)
	{
		return true;
	}

	const float scale = (m_properties.shadowScale <= 0.0f) ? 1.0f : m_properties.shadowScale;
	const float opacity = (m_properties.shadowOpacity <= 0.0f) ? 1.0f : m_properties.shadowOpacity;
	const Vector2 v2Size = GetCurrentSize();
	Vector2 v2ShadowSize(v2Size.x, v2Size.y);
	Vector2 v2ShadowPos(v3EntityPos.x, v3EntityPos.y);

	// if we are drawing to a target of a rotated entity
	if (drawToTarget && targetAngle != 0)
	{
		// rotate de shadow position according to que entity angle
		Matrix4x4 matRot = RotateZ(-DegreeToRadian(targetAngle));
		Vector3 newShadowPos(v2ShadowPos, 0);
		newShadowPos = newShadowPos - v3TargetPos;
		newShadowPos = Multiply(newShadowPos, matRot);
		newShadowPos = newShadowPos + v3TargetPos;
		v2ShadowPos.x = newShadowPos.x;
		v2ShadowPos.y = newShadowPos.y;

		// rotate the light source to cast it correctly
		Vector3 newPos = v3LightPos - v3TargetPos;
		newPos = Multiply(newPos, matRot);
		v3LightPos = newPos + v3TargetPos;
	}

	Vector3 diff = v3EntityPos - v3LightPos;
	const float squaredDist = DP3(diff, diff);
	float squaredRange = light.range * light.range;

	if (squaredDist > squaredRange)
	{
		return true;
	}

	v2ShadowSize.x *= _ETH_SHADOW_SCALEX * scale;

	// calculate the correct shadow lenght according to the light height
	if ((GetPosition().z+v2Size.y) < light.pos.z) // if the light is over the entity
	{
		const float planarDist = Distance(GetPositionXY(), ETHGlobal::ToVector2(v3LightPos));
		const float verticalDist = Abs((v3EntityPos.z+v2Size.y)-v3LightPos.z);
		const float totalDist = (planarDist/verticalDist)*Abs(v3LightPos.z);
		v2ShadowSize.y = totalDist-planarDist;

		// clamp shadow lenght to the object's height. This is not realistic
		// but it looks better for the real-time shadows.
		v2ShadowSize.y = Min(v2Size.y*_ETH_SHADOW_FAKE_STRETCH, v2ShadowSize.y);
	}
	else
	{
		v2ShadowSize.y *= ((drawToTarget) ? _ETH_SHADOW_SCALEY : _ETH_SHADOW_SCALEY/4);
	}

	// specify a minimum length for the shadow
	v2ShadowSize.y = Max(v2ShadowSize.y, v2Size.y);

	ShaderPtr pVS = video->GetVertexShader();
	pVS->SetConstant(GS_L("shadowLength"), v2ShadowSize.y * m_properties.shadowLengthScale);
	pVS->SetConstant(GS_L("entityZ"), Max(m_shadowZ, v3EntityPos.z));
	pVS->SetConstant(GS_L("shadowZ"), m_shadowZ);
	pVS->SetConstant(GS_L("lightPos"), v3LightPos);
	video->SetSpriteDepth(
		(GetType() == ETH_VERTICAL) ?
		ETHGlobal::ComputeDepth(m_shadowZ, maxHeight, minHeight)
		: Max(0.0f, ComputeDepth(maxHeight, minHeight) - m_layrableMinimumDepth));

	v2ShadowSize.y = 1.0f;

	Vector2 lightPos2(v3LightPos.x, v3LightPos.y);
	const float shadowAngle = ::GetAngle((lightPos2 - Vector2(v3EntityPos.x, v3EntityPos.y))) + DegreeToRadian(targetAngle);

	squaredRange = Max(squaredDist, squaredRange);
	float attenBias = 1;

	// adjust brightness according to ambient light
	if (!maxOpacity)
	{
		attenBias = (1-(squaredDist/squaredRange));
		//fade the color according to the light brightness
		const float colorLen = Max(Max(light.color.x, light.color.y), light.color.z);
		attenBias *= Min(colorLen, 1.0f);

		//fade the color according to the ambient light brightness
		const Vector3 &ambientColor = sceneProps.ambient;
		const float ambientColorLen = 1.0f-((ambientColor.x + ambientColor.y + ambientColor.z)/3.0f);
		attenBias = Min(attenBias*ambientColorLen, 1.0f);
		attenBias *= Max(Min((1-(GetPosition().z/Max(GetCurrentSize().y, 1.0f))), 1.0f), 0.0f);
	}

	GS_BYTE alpha = static_cast<GS_BYTE>(attenBias*255.0f*opacity);

	if (alpha < 8)
		return true;

	GS_COLOR dwShadowColor(alpha,255,255,255);

	pShadow->SetOrigin(Vector2(0.5f, 0.79f));
	pShadow->SetRectMode(GSRM_THREE_TRIANGLES);
	pShadow->DrawShaped(v2ShadowPos, v2ShadowSize,
		dwShadowColor, dwShadowColor, dwShadowColor, dwShadowColor,
		RadianToDegree(shadowAngle));

	return true;
}

bool ETHRenderEntity::DrawHalo(const float maxHeight, const float minHeight, const bool rotateHalo, const Vector2 &zAxisDirection)
{
	GS2D_UNUSED_ARGUMENT(minHeight);
	GS2D_UNUSED_ARGUMENT(maxHeight);

	if (!GetHalo() || !HasLightSource())
		return false;

	m_pHalo->SetOrigin(GSEO_CENTER);
	const ETHLight* light = m_properties.light.get();

	const Vector3 v3EntityPos = GetPosition();

	// will remain as 1.0 for a while
	m_provider->GetVideo()->SetSpriteDepth(1.0f/*ETHGlobal::ComputeDepth(m_data.entity.light.pos.z+v3EntityPos.z+ETH_HALO_DEPTH_SHIFT, maxHeight, minHeight)*/);

	// if it has a particle system in the first slot, adjust the light
	// brightness according to the number of active particles
	float brightness = 1.0f;
	const ETHParticleManagerPtr paticleManager = GetParticleManager(0);
	if (paticleManager)
	{
		brightness = static_cast<float>(paticleManager->GetNumActiveParticles())/static_cast<float>(paticleManager->GetNumParticles());
	}

	Vector3 v3HaloPos = light->pos + v3EntityPos;

	GS_COLOR dwColor = ConvertToDW(light->color * light->haloBrightness * brightness);
	Vector2 v2Size(light->haloSize, light->haloSize);

	m_pHalo->DrawShaped(
		ETHGlobal::ToScreenPos(v3HaloPos, zAxisDirection) + ComputeParallaxOffset(),
		v2Size, dwColor, dwColor, dwColor, dwColor,
		(rotateHalo) ? ComputeHaloAngle() : 0.0f);
	return true;
}

bool ETHRenderEntity::DrawParticles(const unsigned int n, const float maxHeight, const float minHeight, const ETHSceneProperties& sceneProps)
{
	if (n >= m_particles.size())
	{
		return false;
	}
	else
	{
		m_particles[n]->DrawParticleSystem(sceneProps.ambient, maxHeight, minHeight, GetType(),
										   sceneProps.zAxisDirection, ComputeParallaxOffset(),
										   ComputeDepth(maxHeight, minHeight));
		return true;
	}
}

void ETHRenderEntity::DrawCollisionBox(const bool drawBase, SpritePtr pOutline, const GS_COLOR dwColor, const Vector2 &zAxisDirection)
{
	VideoPtr video = m_provider->GetVideo();
	const bool collidable = (m_properties.collision);
	const Vector3 v3Size = (collidable) ? m_properties.collision->size : Vector3(32,32,32);
	const Vector3 v3Pos = (collidable) ? (m_properties.collision->pos + GetPosition()) : GetPosition();

	const Vector2 v2Pos = ETHGlobal::ToScreenPos(v3Pos, zAxisDirection) + Vector2(0, v3Size.y/2);
	pOutline->SetOrigin(GSEO_CENTER_BOTTOM);

	const GS_ALPHA_MODE alphaMode = video->GetAlphaMode();
	video->SetAlphaMode(GSAM_PIXEL);

	ShaderPtr pVS = video->GetVertexShader();
	video->SetVertexShader(ShaderPtr());

	const GS_COLOR dwH = ARGB(150,dwColor.r,dwColor.g,dwColor.b);
	const float depth = video->GetSpriteDepth();
	if (drawBase)
	{
		// base
		video->SetSpriteDepth(1.0f);
		pOutline->DrawShaped(v2Pos, Vector2(v3Size.x, v3Size.y), dwH, dwH, dwH, dwH);
	}
	else
	{
		video->SetSpriteDepth(0.0f);
		const GS_COLOR dwV = ARGB(50,dwColor.r,dwColor.g,dwColor.b);
		// front face
		pOutline->DrawShaped(v2Pos, Vector2(v3Size.x, v3Size.z), dwV, dwV, dwV, dwV);

		// upper face
		pOutline->DrawShaped(v2Pos-Vector2(0,v3Size.z), Vector2(v3Size.x, v3Size.y), dwH, dwH, dwH, dwH);
	}

	video->SetAlphaMode(alphaMode);
	video->SetVertexShader(pVS);
	video->SetSpriteDepth(depth);
}