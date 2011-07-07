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

#ifndef GLES2_RECT_RENDERER_H_
#define GLES2_RECT_RENDERER_H_

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "../../Platform/Platform.h"
#include "../../Platform/FileLogger.h"

namespace gs2d {

class GLES2RectRenderer
{
	GLuint m_vertexBuffer;
	GLuint m_indexBuffer;
	
	mutable struct LatestLocations
	{
		LatestLocations() :
			positionLocation(-1),
			texCoordLocation(-1)
		{
		}
		int positionLocation;
		int texCoordLocation;
	} m_latestLocations;

public:
	GLES2RectRenderer(const Platform::FileLogger& logger);
	void BeginFastDraw(const int positionLocation, const int texCoordLocation, const Platform::FileLogger& logger) const;
	void FastDraw(const Platform::FileLogger& logger) const;
	void EndFastDraw(const Platform::FileLogger& logger) const;
	void SetPositionLocations(const int positionLocation, const int texCoordLocation, const Platform::FileLogger& logger) const;
	void Draw(const int positionLocation, const int texCoordLocation, const Platform::FileLogger& logger) const;

	void BeginFastDrawFromClientMem(const int positionLocation, const int texCoordLocation, const Platform::FileLogger& logger) const;
	void FastDrawFromClientMem(const Platform::FileLogger& logger) const;
	void EndFastDrawFromClientMem(const Platform::FileLogger& logger) const;
	void DrawFromClientMem(const int positionLocation, const int texCoordLocation, const Platform::FileLogger& logger) const;
};

} // namespace gs2d

#endif