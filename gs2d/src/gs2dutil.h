/*-----------------------------------------------------------------------

 gameSpace2d (C) Copyright 2008-2011 Andre Santee
 http://www.asantee.net/gamespace/
 http://groups.google.com/group/gamespacelib

    This file is part of gameSpace2d.

    gameSpace2d is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    gameSpace2d is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with gameSpace2d. If not, see
    <http://www.gnu.org/licenses/>.

-----------------------------------------------------------------------*/

#ifndef GS2D_UTIL_H_
#define GS2D_UTIL_H_

#include "gs2d.h"

namespace gs2d {
namespace math {

class Shape
{
public:
	inline Shape(const std::vector<Plane> &planes)
	{
		m_planes = planes;
	}

	inline Shape(const Vector2& rectPos, const Vector2& rectSize)
	{
		m_planes.resize(8);
		int idx = 0;

		const Vector2 halfRectSize = rectSize/2.0f;
		m_planes[idx++] = (ComputePlane(rectPos+Vector2(0.0f,-halfRectSize.y), Vector2(0.0f, 1.0f)));
		m_planes[idx++] = (ComputePlane(rectPos+Vector2(0.0f, halfRectSize.y), Vector2(0.0f,-1.0f)));
		m_planes[idx++] = (ComputePlane(rectPos+Vector2(-halfRectSize.x, 0.0f), Vector2( 1.0f, 0.0f)));
		m_planes[idx++] = (ComputePlane(rectPos+Vector2( halfRectSize.x, 0.0f), Vector2(-1.0f, 0.0f)));

		m_planes[idx++] = (ComputePlane(rectPos+Vector2(halfRectSize.x, halfRectSize.y), Normalize(Vector2(-1.0f, -1.0f))));
		m_planes[idx++] = (ComputePlane(rectPos+Vector2(-halfRectSize.x, halfRectSize.y), Normalize(Vector2(1.0f, -1.0f))));
		m_planes[idx++] = (ComputePlane(rectPos+Vector2(halfRectSize.x, -halfRectSize.y), Normalize(Vector2(-1.0f, 1.0f))));
		m_planes[idx++] = (ComputePlane(rectPos+Vector2(-halfRectSize.x, -halfRectSize.y), Normalize(Vector2(1.0f, 1.0f))));
	}

	inline bool IntersectSphere(const Vector2& pos, const float radius)
	{
		for (std::vector<Plane>::iterator iter = m_planes.begin(); iter != m_planes.end(); iter++)
		{
			if (Abs(PlaneDotVector(*iter, pos)) < radius)
			{
				return true;
			}
		}
		return false;
	}

	inline bool OverlapSphere(const Vector2& pos, const float radius)
	{
		for (std::vector<Plane>::iterator iter = m_planes.begin(); iter != m_planes.end(); iter++)
		{
			if (PlaneDotVector(*iter, pos) < -radius)
			{
				return false;
			}
		}
		return true;
	}

	inline void DrawPlanes(VideoPtr video, const GS_COLOR color = GS_WHITE)
	{
		for (std::vector<Plane>::iterator iter = m_planes.begin(); iter != m_planes.end(); iter++)
		{
			const Vector2 normal(iter->x, iter->y);
			const float angle = GetAngle(normal);
			video->DrawRectangle(normal *-iter->w, Vector2(2000, 4), color, RadianToDegree(angle), GSEO_CENTER);
		}
	}

private:
	std::vector<Plane> m_planes;
};

} // namespace math

} // namespace gs2d

#endif