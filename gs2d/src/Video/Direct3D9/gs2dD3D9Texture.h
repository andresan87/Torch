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

#ifndef GS2D_D3D9_TEXTURE_H_
#define GS2D_D3D9_TEXTURE_H_

#include "../../gs2d.h"
#include <d3dx9.h>

namespace gs2d {

/**
 * \brief Implements a Direct3D 9 Texture object
 */
class D3D9Texture : public Texture
{
	IDirect3DTexture9 *m_pTexture;
	IDirect3DDevice9 *m_pDevice;
	VideoWeakPtr m_video;
	PROFILE m_profile;
	TYPE  m_texType;

	bool GetInternalData();

public:
	D3D9Texture();
	~D3D9Texture();

	bool SetTexture(const unsigned int passIdx = 0);
	PROFILE GetProfile() const;
	TYPE GetTextureType() const;
	boost::any GetTextureObject();
	math::Vector2 GetBitmapSize() const;

protected:
	bool CreateRenderTarget(
		VideoWeakPtr video, const unsigned int width, const unsigned int height, const GS_TARGET_FORMAT fmt
	);
	bool LoadTexture(
		VideoWeakPtr video,
		const std::wstring& fileName, GS_COLOR mask,
			const unsigned int width = 0, const unsigned int height = 0,
			const unsigned int nMipMaps = 0
	);
	virtual bool LoadTexture(
		VideoWeakPtr video,
		const void * pBuffer, GS_COLOR mask,
			const unsigned int width, const unsigned int height,
			const unsigned int nMipMaps,
			const unsigned int bufferLength
	);

};

} // namespace gs2d

#endif