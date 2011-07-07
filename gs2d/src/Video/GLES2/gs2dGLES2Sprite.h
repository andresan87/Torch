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

#ifndef GS2D_GLES2_SPRITE_H_
#define GS2D_GLES2_SPRITE_H_

#include "../../gs2d.h"
#include "gs2dGLES2Shader.h"
#include "gs2dGLES2Texture.h"
#include <boost/shared_array.hpp>

namespace gs2d {

class GLES2Sprite : public Sprite
{
private:
	GLES2ShaderContext* m_shaderContext;

	static const str_type::string SPRITE_LOG_FILE;
	static Platform::FileLogger m_logger;
	
	TYPE m_type;
	math::Vector2 m_bitmapSize;
	math::Vector2 m_normalizedOrigin;
	GLES2TexturePtr m_texture;

	unsigned int  m_currentRect;
	boost::shared_array<math::Rect2Df> m_rects;
	math::Rect2Df m_rect;
	unsigned int m_nRects, m_nColumns, m_nRows;
	
	Video* m_video;
	
public:
	GLES2Sprite(GLES2ShaderContextPtr shaderContext);

	bool LoadSprite(VideoWeakPtr video, GS_BYTE *pBuffer, const unsigned int bufferLength,
						GS_COLOR mask = GS_ZERO, const unsigned int width = 0, const unsigned int height = 0);
	bool LoadSprite(VideoWeakPtr video, const str_type::string& fileName, GS_COLOR mask = GS_ZERO,
					const unsigned int width = 0, const unsigned int height = 0);
	bool CreateRenderTarget(VideoWeakPtr video, const unsigned int width, const unsigned int height,
							const GS_TARGET_FORMAT format = GSTF_DEFAULT);

	bool Draw(const math::Vector2 &v2Pos,
					const GS_COLOR color = GS_WHITE,
					const float angle = 0.0f,
					const math::Vector2 &v2Scale = math::Vector2(1.0f,1.0f));
	bool DrawShaped(const math::Vector2 &v2Pos, const math::Vector2 &v2Size,
						  const GS_COLOR color0, const GS_COLOR color1,
						  const GS_COLOR color2, const GS_COLOR color3,
						  const float angle = 0.0f);
	bool DrawOptimal(const math::Vector2 &v2Pos, const GS_COLOR color = GS_WHITE, const float angle = 0.0f,
					 const math::Vector2 &v2Size = math::Vector2(-1,-1));
	bool Stretch(const math::Vector2 &a, const math::Vector2 &b, const float width,
					   const GS_COLOR color0 = GS_WHITE, const GS_COLOR color1 = GS_WHITE);
	bool SaveBitmap(const wchar_t *wcsName, const GS_BITMAP_FORMAT fmt, math::Rect2D *pRect = 0);

	bool DrawShapedFast(const math::Vector2 &v2Pos, const math::Vector2 &v2Size, const GS_COLOR color);
	void BeginFastRendering();
	void EndFastRendering();

	TextureWeakPtr GetTexture();
	void SetOrigin(const GS_ENTITY_ORIGIN origin);
	void SetOrigin(const math::Vector2 &v2Custom);
	math::Vector2 GetOrigin() const;

	bool SetupSpriteRects(const unsigned int columns, const unsigned int rows);
	bool SetRect(const unsigned int column, const unsigned int row);
	bool SetRect(const unsigned int rect);
	void SetRect(const math::Rect2Df &rect);
	void UnsetRect();
	int GetNumRects() const;
	math::Rect2Df GetRect() const;
	math::Rect2Df GetRect(const unsigned int rect) const;
	unsigned int GetRectIndex() const;

	Texture::PROFILE GetProfile() const;
	math::Vector2i GetBitmapSize() const;
	math::Vector2 GetBitmapSizeF() const;

	unsigned int GetNumRows() const;
	unsigned int GetNumColumns() const;

	void FlipX(const bool flip);
	void FlipY(const bool flip);
	void FlipX();
	void FlipY();
	bool GetFlipX() const;
	bool GetFlipY() const;

	void SetScroll(const math::Vector2 &v2Scroll);
	math::Vector2 GetScroll() const;

	void SetMultiply(const math::Vector2 &v2Multiply);
	math::Vector2 GetMultiply() const;

	TYPE GetType() const;
	boost::any GetTextureObject();

	void GenerateBackup();
	bool SetAsTexture(const unsigned int passIdx);
	void SetRectMode(const GS_RECT_MODE mode);
	GS_RECT_MODE GetRectMode() const;

	void OnLostDevice();

	void RecoverFromBackup();
};

} // namespace gs2d

#endif