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


#ifndef ETH_PARALLAX_MANAGER_H_
#define ETH_PARALLAX_MANAGER_H_

#include "../gs2d/src/gs2d.h"
#include "../gs2d/src/gs2dshader.h"

using namespace gs2d;
using namespace math;

class ETHParallaxManager
{
	Vector2 m_normalizedOrigin;
	float m_intensity;

public:
	ETHParallaxManager();
	ETHParallaxManager(const Vector2 &normalizedOrigin, const float intensity);
	void SetNormalizedOrigin(const Vector2 &normalizedOrigin);
	Vector2 GetNormalizedOrigin() const;
	Vector2 GetInScreenOrigin(VideoConstPtr video) const;

	inline Vector2 ComputeOffset(const VideoPtr video, const Vector3 &pos) const
	{
		const Vector2 screenSpacePos = Vector2(pos.x, pos.y) - video->GetCameraPos();
		return ((screenSpacePos-GetInScreenOrigin(video))/video->GetScreenSizeF().x)*pos.z*m_intensity;
	}

	void SetIntensity(const float intensity);
	float GetIntensity() const;

	void SetShaderParameters(VideoConstPtr video, ShaderPtr shader, const Vector3 &entityPos, const bool drawToTarget) const;
};

#endif