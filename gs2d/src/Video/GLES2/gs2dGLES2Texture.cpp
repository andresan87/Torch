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

#include "gs2dGLES2Texture.h"
#include "gs2dGLES2.h"
#include <SOIL.h>
#include <sstream>

namespace gs2d {

const str_type::string GLES2Texture::TEXTURE_LOG_FILE("GLES2Texture.log.txt");
GLuint GLES2Texture::m_textureID(1000);
Platform::FileLogger GLES2Texture::m_logger(Platform::FileLogger::GetLogPath() + GLES2Texture::TEXTURE_LOG_FILE);

void CheckFrameBufferStatus(const Platform::FileLogger& logger, const GLuint fbo, const GLuint tex, const bool showSuccessMessage)
{
	const GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	str_type::stringstream ss;
	ss << GS_L("fboID ") << fbo << GS_L(" (") << tex << GS_L("): ");
	switch (status)
	{
	case GL_FRAMEBUFFER_COMPLETE:
		if (showSuccessMessage)
		{
			ss << GS_L(" render target texture created successfully");
			logger.Log(ss.str(), Platform::FileLogger::INFO);
		}
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
		ss << GS_L(" incomplete attachment");
		logger.Log(ss.str(), Platform::FileLogger::ERROR);
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS:
		ss << GS_L(" incomplete dimensions");
		logger.Log(ss.str(), Platform::FileLogger::ERROR);
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
		ss << GS_L(" incomplete missing attachment");
		logger.Log(ss.str(), Platform::FileLogger::ERROR);
		break;
	case GL_FRAMEBUFFER_UNSUPPORTED:
		ss << GS_L(" unsupported");
		logger.Log(ss.str(), Platform::FileLogger::ERROR);
		break;
	default:
		ss << GS_L(" unknown status");
		logger.Log(ss.str(), Platform::FileLogger::ERROR);
	}
}

void ApplyPixelMask(unsigned char *ht_map, const GS_COLOR mask, const int channels, const int width, const int height)
{
	if (channels == 4)
	{
		const int numBytes = width * height * channels;
		for (int i = 0; i < numBytes; i += channels)
		{
			unsigned char& r = ht_map[i+0];
			unsigned char& g = ht_map[i+1];
			unsigned char& b = ht_map[i+2];
			unsigned char& a = ht_map[i+3];

			if ((r == mask.r && g == mask.g && b == mask.b && mask.a == 0xFF)
				|| (a == 0x0))
			{
				r = g = b = a = 0x0;
			}
		}
	}
}

GLES2Texture::TEXTURE_INFO::TEXTURE_INFO() :
	m_texture(0),
	m_frameBuffer(0)
{
}

GLES2Texture::GLES2Texture(VideoWeakPtr video, const str_type::string& fileName, Platform::ZipFileManagerPtr fileManager) :
		m_fileManager(fileManager),
		m_fileName(fileName),
		m_type(TT_NONE)
{
}

GLES2Texture::~GLES2Texture()
{
	if (m_textureInfo.m_texture != 0)
	{
		GLuint textures[1] = { m_textureInfo.m_texture };
		glDeleteTextures(1, textures);
		std::stringstream ss;
		ss << m_fileName << ": texture deleted ID " << m_textureInfo.m_texture; 
		m_logger.Log(ss.str(), Platform::FileLogger::INFO);
		m_textureInfo.m_texture = 0;
	}
	/*if (m_textureInfo.m_frameBuffer != 0)
	{
		GLuint buffers[1] = { m_textureInfo.m_frameBuffer };
		glDeleteFramebuffers(1, buffers);
		std::stringstream ss;
		ss << "Frame buffer deleted ID " << m_textureInfo.m_frameBuffer; 
		m_logger.Log(ss.str());
		m_textureInfo.m_frameBuffer = 0;
	}*/
}

bool GLES2Texture::SetTexture(const unsigned int passIdx)
{
	glBindTexture(GL_TEXTURE_2D, m_textureInfo.m_texture);
	return true;
}

Texture::PROFILE GLES2Texture::GetProfile() const
{
	return m_profile;
}

Texture::TYPE GLES2Texture::GetTextureType() const
{
	return m_type;
}

boost::any GLES2Texture::GetTextureObject()
{
	return m_textureInfo.m_texture;
}

bool GLES2Texture::CreateRenderTarget(VideoWeakPtr video, const unsigned int width, const unsigned int height, const GS_TARGET_FORMAT fmt)
{
	m_textureInfo.m_texture = m_textureID++;

	//glGenTextures(1, &m_textureInfo.m_texture);
	glBindTexture(GL_TEXTURE_2D, m_textureInfo.m_texture);

	const GLint glfmt = (fmt == GSTF_ARGB) ? GL_RGBA : GL_RGB;
	const unsigned int channels = (fmt == GSTF_ARGB) ? 4 : 3;
	const unsigned int bufferLength = width * height * channels;
	unsigned char *buffer = new unsigned char [bufferLength];
	
	for (unsigned int t=0; t<bufferLength; t++)
	{
		buffer[t] = 0x0;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, glfmt,
		static_cast<GLsizei>(width), static_cast<GLsizei>(height),
		0, static_cast<GLenum>(glfmt), GL_UNSIGNED_BYTE, buffer);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);

	// attach 2D texture
	glGenFramebuffers(1, &m_textureInfo.m_frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_textureInfo.m_frameBuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_textureInfo.m_texture, 0);

	// create depth buffer
	GLuint depthRenderbuffer;
	glGenRenderbuffers(1, &depthRenderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, static_cast<GLsizei>(width), static_cast<GLsizei>(height));
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer);

	CheckFrameBufferStatus(m_logger, m_textureInfo.m_frameBuffer, m_textureInfo.m_texture, true);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	m_type = TT_RENDER_TARGET;
	m_profile.width = width;
	m_profile.height = height;
	m_profile.originalWidth = m_profile.width;
	m_profile.originalHeight = m_profile.height;

	delete [] buffer;
	return true;
}

bool GLES2Texture::LoadTexture(VideoWeakPtr video, const str_type::string& fileName, GS_COLOR mask,
		const unsigned int width, const unsigned int height, const unsigned int nMipMaps)
{
	m_logger.Log(str_type::string("\n___________________\n") + fileName + " loading texture...", Platform::FileLogger::INFO);

	m_fileName = fileName;
	Platform::FileBuffer out;
	m_fileManager->GetFileBuffer(fileName, out);
	if (!out)
	{
		m_logger.Log(fileName + " could not load buffer", Platform::FileLogger::ERROR);
		return false;
	}
	return LoadTexture(video, out->GetAddress(), mask, width, height, nMipMaps, out->GetBufferSize());
}

bool GLES2Texture::LoadTexture(VideoWeakPtr video, const void * pBuffer, GS_COLOR mask,
		const unsigned int width, const unsigned int height, const unsigned int nMipMaps,
		const unsigned int bufferLength)
{
	int iWidth, iHeight, channels;
	unsigned char *ht_map = SOIL_load_image_from_memory((unsigned char*)pBuffer, bufferLength, &iWidth, &iHeight, &channels, SOIL_LOAD_AUTO);

	if (ht_map)
	{
		ApplyPixelMask(ht_map, mask, channels, iWidth, iHeight);
		m_textureInfo.m_texture = SOIL_create_OGL_texture(ht_map, iWidth, iHeight, channels, m_textureID++, SOIL_FLAG_POWER_OF_TWO);
	}

	std::stringstream ss;
	ss << m_fileName << " file ID " << m_textureInfo.m_texture;
	m_logger.Log(ss.str(), Platform::FileLogger::INFO);

	if (!m_textureInfo.m_texture)
	{
		m_logger.Log(m_fileName + " couldn't load texture", Platform::FileLogger::ERROR);
		video.lock()->Message(m_fileName + " couldn't load texture", GSMT_ERROR);
		SOIL_free_image_data(ht_map);
		return false;
	}
	else
	{
		m_type = TT_STATIC;
		m_profile.width = static_cast<unsigned int>(iWidth);
		m_profile.height = static_cast<unsigned int>(iHeight);
		m_profile.originalWidth = m_profile.width;
		m_profile.originalHeight = m_profile.height;
		m_logger.Log(m_fileName + " texture loaded", Platform::FileLogger::INFO);
		SOIL_free_image_data(ht_map);
		return true;
	}
}

math::Vector2 GLES2Texture::GetBitmapSize() const
{
	return math::Vector2(static_cast<float>(m_profile.width), static_cast<float>(m_profile.height));
}

const str_type::string& GLES2Texture::GetFileName() const
{
	return m_fileName;
}

GLuint GLES2Texture::GetTextureID() const
{
	return m_textureInfo.m_texture;
}

GLuint GLES2Texture::GetFrameBufferID() const
{
	return m_textureInfo.m_frameBuffer;
}

} // namespace gs2d