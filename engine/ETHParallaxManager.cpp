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

#include "ETHParallaxManager.h"

ETHParallaxManager::ETHParallaxManager()
 : m_normalizedOrigin(0.5f, 0.5f), m_intensity(0)
{
}

ETHParallaxManager::ETHParallaxManager(const Vector2 &normalizedOrigin, const float intensity)
{
	SetNormalizedOrigin(normalizedOrigin);
	SetIntensity(intensity);
}

void ETHParallaxManager::SetNormalizedOrigin(const Vector2 &normalizedOrigin)
{
	m_normalizedOrigin = normalizedOrigin;
}

Vector2 ETHParallaxManager::GetNormalizedOrigin() const
{
	return m_normalizedOrigin;
}

Vector2 ETHParallaxManager::GetInScreenOrigin(VideoConstPtr video) const
{
	return GetNormalizedOrigin()*video->GetScreenSizeF();
}

void ETHParallaxManager::SetIntensity(const float intensity)
{
	m_intensity = intensity;
}

float ETHParallaxManager::GetIntensity() const
{
	return m_intensity;
}

void ETHParallaxManager::SetShaderParameters(VideoConstPtr video, ShaderPtr shader, const Vector3 &entityPos, const bool drawToTarget) const
{
	if (!drawToTarget)
	{
		shader->SetConstant(GS_L("entityPos3D"), entityPos);
		shader->SetConstant(GS_L("parallaxOrigin"), GetInScreenOrigin(video));
		shader->SetConstant(GS_L("parallaxIntensity"), GetIntensity());
	}
	else
	{
		shader->SetConstant(GS_L("parallaxIntensity"), 0.0f);
	}
}
