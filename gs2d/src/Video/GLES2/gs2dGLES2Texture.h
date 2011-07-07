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

#ifndef GS2D_GLES2_TEXTURE_H_
#define GS2D_GLES2_TEXTURE_H_

#include "../../gs2d.h"
#include "../../Platform/FileLogger.h"
#include "../../Platform/android/Platform.android.h"
#include <GLES2/gl2.h>

namespace gs2d {

void CheckFrameBufferStatus(const Platform::FileLogger& logger, const GLuint fbo, const GLuint tex, const bool showSuccessMessage);

class GLES2Texture : public Texture
{
public:
	GLES2Texture(VideoWeakPtr video, const str_type::string& fileName, Platform::ZipFileManagerPtr fileManager);
	~GLES2Texture();
	bool SetTexture(const unsigned int passIdx = 0);
	PROFILE GetProfile() const;
	TYPE GetTextureType() const;
	boost::any GetTextureObject();
	GLuint GetTextureID() const;
	GLuint GetFrameBufferID() const;

	bool CreateRenderTarget(VideoWeakPtr video, const unsigned int width, const unsigned int height, const GS_TARGET_FORMAT fmt);

	bool LoadTexture(VideoWeakPtr video, const str_type::string& fileName, GS_COLOR mask,
			const unsigned int width = 0, const unsigned int height = 0, const unsigned int nMipMaps = 0);

	bool LoadTexture(VideoWeakPtr video, const void* pBuffer, GS_COLOR mask,
			const unsigned int width, const unsigned int height, const unsigned int nMipMaps,
			const unsigned int bufferLength);

	math::Vector2 GetBitmapSize() const;
	const str_type::string& GetFileName() const;

	static const str_type::string TEXTURE_LOG_FILE;

private:
	struct TEXTURE_INFO
	{
		TEXTURE_INFO();
		GLuint m_texture, m_frameBuffer;
	} m_textureInfo;

	TYPE m_type;
	PROFILE m_profile;
	str_type::string m_fileName;
	Platform::ZipFileManagerPtr m_fileManager;
	static Platform::FileLogger m_logger;
	static GLuint m_textureID;
};

typedef boost::shared_ptr<GLES2Texture> GLES2TexturePtr;

} // namespace gs2d

#endif