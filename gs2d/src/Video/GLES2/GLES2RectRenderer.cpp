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

#include "GLES2RectRenderer.h"

#include "gs2dGLES2.h"

namespace gs2d {

const GLfloat g_quad2Vertices[] =
{
	0.0f, 1.0f,
	0.0f, 0.0f,
	1.0f, 0.0f,
	1.0f, 1.0f
};

const GLfloat g_quad2TexCoords[] =
{
	0.0f, 1.0f,
	0.0f, 0.0f,
	1.0f, 0.0f,
	1.0f, 1.0f
};

const GLfloat g_quad2Sprite[] =
{
	0.0f, 1.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 1.0f, 1.0f
};

const GLubyte g_indices[] = { 0, 1, 2, 3 };

GLES2RectRenderer::GLES2RectRenderer(const Platform::FileLogger& logger)
{
	glGenBuffers(1, &m_vertexBuffer);
	GLES2Video::CheckGLError("GLES2RectRenderer::GLES2RectRenderer - glGenBuffers", logger);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	GLES2Video::CheckGLError("GLES2RectRenderer::GLES2RectRenderer - glBindBuffer", logger);

	glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad2Sprite), g_quad2Sprite, GL_STATIC_DRAW);
	GLES2Video::CheckGLError("GLES2RectRenderer::GLES2RectRenderer - glBufferData", logger);
	
	glGenBuffers(1, &m_indexBuffer);
	GLES2Video::CheckGLError("GLES2RectRenderer::GLES2RectRenderer - glGenBuffers (index)", logger);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
	GLES2Video::CheckGLError("GLES2RectRenderer::GLES2RectRenderer - glBindBuffer (index)", logger);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(g_indices), g_indices, GL_STATIC_DRAW);
	GLES2Video::CheckGLError("GLES2RectRenderer::GLES2RectRenderer - glBufferData (index)", logger);
}

void GLES2RectRenderer::BeginFastDraw(const int positionLocation, const int texCoordLocation, const Platform::FileLogger& logger) const
{
	SetPositionLocations(positionLocation, texCoordLocation, logger);
}

void GLES2RectRenderer::FastDraw(const Platform::FileLogger& logger) const
{
	glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_BYTE, (void*)0);
}

void GLES2RectRenderer::EndFastDraw(const Platform::FileLogger& logger) const
{
}

void GLES2RectRenderer::SetPositionLocations(const int positionLocation, const int texCoordLocation, const Platform::FileLogger& logger) const
{
	if (m_latestLocations.positionLocation != positionLocation || m_latestLocations.texCoordLocation != texCoordLocation)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
		glVertexAttribPointer(positionLocation, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)0);
		glEnableVertexAttribArray(positionLocation);

		glVertexAttribPointer(texCoordLocation, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)(sizeof(float) * 2));
		glEnableVertexAttribArray(texCoordLocation);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);

		m_latestLocations.positionLocation = positionLocation;
		m_latestLocations.texCoordLocation = texCoordLocation;
		logger.Log("GLES2RectRenderer::SetPositionLocations: new vertex attribute array set", Platform::FileLogger::INFO);
	}
}

void GLES2RectRenderer::Draw(const int positionLocation, const int texCoordLocation, const Platform::FileLogger& logger) const
{
	SetPositionLocations(positionLocation, texCoordLocation, logger);
	glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_BYTE, (void*)0);
}

//
// Client mem drawing
//

void GLES2RectRenderer::BeginFastDrawFromClientMem(const int positionLocation, const int texCoordLocation, const Platform::FileLogger& logger) const
{
	glVertexAttribPointer(positionLocation, 2, GL_FLOAT, GL_FALSE, 0, g_quad2Vertices);
	GLES2Video::CheckGLError("GLES2RectRenderer::BeginFastDraw - glVertexAttribPointer - position", logger);

	glVertexAttribPointer(texCoordLocation, 2, GL_FLOAT, GL_FALSE, 0, g_quad2TexCoords);
	GLES2Video::CheckGLError("GLES2RectRenderer::BeginFastDraw - glVertexAttribPointer - texCoord", logger);

	glEnableVertexAttribArray(positionLocation);
	GLES2Video::CheckGLError("glEnableVertexAttribArray - position", logger);

	glEnableVertexAttribArray(texCoordLocation);
	GLES2Video::CheckGLError("glEnableVertexAttribArray - texCoord", logger);
}

void GLES2RectRenderer::FastDrawFromClientMem(const Platform::FileLogger& logger) const
{
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	GLES2Video::CheckGLError("glDrawArrays - fast drawing", logger);
}

void GLES2RectRenderer::EndFastDrawFromClientMem(const Platform::FileLogger& logger) const
{
}

void GLES2RectRenderer::DrawFromClientMem(const int positionLocation, const int texCoordLocation, const Platform::FileLogger& logger) const
{
	glVertexAttribPointer(positionLocation, 2, GL_FLOAT, GL_FALSE, 0, g_quad2Vertices);
	GLES2Video::CheckGLError("GLES2RectRenderer::Draw - glVertexAttribPointer - position", logger);

	glVertexAttribPointer(texCoordLocation, 2, GL_FLOAT, GL_FALSE, 0, g_quad2TexCoords);
	GLES2Video::CheckGLError("GLES2RectRenderer::Draw - glVertexAttribPointer - texCoord", logger);

	glEnableVertexAttribArray(positionLocation);
	GLES2Video::CheckGLError("glEnableVertexAttribArray - position", logger);

	glEnableVertexAttribArray(texCoordLocation);
	GLES2Video::CheckGLError("glEnableVertexAttribArray - texCoord", logger);

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	GLES2Video::CheckGLError("glDrawArrays", logger);
}

} // namespace gs2d
