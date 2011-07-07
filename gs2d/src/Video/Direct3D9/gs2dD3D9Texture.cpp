/*-----------------------------------------------------------------------

 GameSpace2d (C) Copyright 2008-2011 Andre Santee
 http://www.asantee.net/gamespace/
 http://groups.google.com/group/gamespacelib

    This file is part of GameSpace2d.

    GameSpace2d is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    GameSpace2d is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with GameSpace2d. If not, see
    <http://www.gnu.org/licenses/>.

-----------------------------------------------------------------------*/

#include "gs2dD3D9Texture.h"
#include "../../unicode/utf8converter.h"

namespace gs2d {
using namespace math;

D3D9Texture::D3D9Texture()
{
	m_pTexture = NULL;
	m_pDevice = NULL;
	m_texType = Texture::TT_NONE;
}

D3D9Texture::~D3D9Texture()
{
	if (m_pTexture)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

bool D3D9Texture::CreateRenderTarget(VideoWeakPtr video, const unsigned int width, const unsigned int height,
									 const GS_TARGET_FORMAT fmt)
{
	m_video = video;
	GetInternalData();
	IDirect3DTexture9 *pTexture = NULL;

	m_profile.mask = 0x0;
	m_profile.nMipMaps = 1;
	m_texType = TT_RENDER_TARGET;

	const DWORD dwUsage = D3DUSAGE_RENDERTARGET;
	if (width <= 0 || height <= 0)
	{
		std::wstringstream ss;
		ss << L"Couldn't create the render surface - D3D9Texture::LoadTexture" << std::endl;
		ShowMessage(ss, GSMT_ERROR);
		return false;
	}

	IDirect3DSurface9 *pBackBuffer = NULL;
	D3DSURFACE_DESC surfDesc;
	m_pDevice->GetRenderTarget(0, &pBackBuffer);
	pBackBuffer->GetDesc(&surfDesc);
	pBackBuffer->Release();

	m_pDevice->CreateTexture(width, height, 1,
						   dwUsage, (fmt == GSTF_ARGB) ? D3DFMT_A8R8G8B8 : surfDesc.Format,
						   D3DPOOL_DEFAULT, &pTexture, NULL);

	m_profile.width = width;
	m_profile.height = height;
	m_profile.originalWidth = m_profile.width;
	m_profile.originalHeight = m_profile.height;
	m_pTexture = pTexture;
	return true;
}

bool D3D9Texture::LoadTexture(
	VideoWeakPtr video,
	const std::wstring& fileName, GS_COLOR mask,
		const unsigned int width, const unsigned int height,
		const unsigned int nMipMaps
)
{
	m_video = video;
	GetInternalData();
	IDirect3DTexture9 *pTexture = NULL;

	m_profile.mask = mask;
	m_profile.nMipMaps = nMipMaps;
	m_texType = TT_STATIC;

	if (m_texType == TT_STATIC)
	{
		D3DXIMAGE_INFO imageInfo;
		HRESULT hr;
		{
			hr = D3DXCreateTextureFromFileEx(
				m_pDevice,
				fileName.c_str(),
				(width) ? width : D3DX_DEFAULT_NONPOW2,
				(height) ? height : D3DX_DEFAULT_NONPOW2,
				(nMipMaps == FULL_MIPMAP_CHAIN) ? D3DX_DEFAULT : Max(nMipMaps, static_cast<unsigned int>(1)),
				0,
				D3DFMT_UNKNOWN,
				D3DPOOL_MANAGED,
				D3DX_DEFAULT,
				D3DX_DEFAULT,
				mask.color,
				&imageInfo,
				NULL,
				&pTexture
			);
		}
		if (FAILED(hr))
		{
			std::wstringstream ss;
			ss << L"D3D9Texture::LoadTextureFromFile:\nCannot find and/or create the texture from a bitmap.\n";
			{
				ss << fileName << std::endl;
			}
			ShowMessage(ss, GSMT_ERROR);
			return false;
		}
		if (width == 0 && height == 0)
		{
			m_profile.width = imageInfo.Width;
			m_profile.height = imageInfo.Height;
		}
		else
		{
			m_profile.width = width;
			m_profile.height = height;
		}
		m_profile.originalWidth = imageInfo.Width;
		m_profile.originalHeight = imageInfo.Height;
	}
	m_pTexture = pTexture;

	return true;
}

bool D3D9Texture::LoadTexture(
	VideoWeakPtr video,
	const void * pBuffer, GS_COLOR mask,
		const unsigned int width, const unsigned int height,
		const unsigned int nMipMaps,
		const unsigned int bufferLength
)
{
	m_video = video;
	GetInternalData();
	IDirect3DTexture9 *pTexture = NULL;

	m_profile.mask = mask;
	m_profile.nMipMaps = nMipMaps;
	m_texType = TT_STATIC;

	if (m_texType == TT_STATIC)
	{
		D3DXIMAGE_INFO imageInfo;
		HRESULT hr;
		{
			hr = D3DXCreateTextureFromFileInMemoryEx(
				m_pDevice,
				pBuffer,
				bufferLength,
				(width) ? width : D3DX_DEFAULT_NONPOW2,
				(height) ? height : D3DX_DEFAULT_NONPOW2,
				(nMipMaps == FULL_MIPMAP_CHAIN) ? D3DX_DEFAULT : Max(nMipMaps, static_cast<unsigned int>(1)),
				0,
				D3DFMT_UNKNOWN,
				D3DPOOL_MANAGED,
				D3DX_DEFAULT,
				D3DX_DEFAULT,
				mask.color,
				&imageInfo,
				NULL,
				&pTexture
			);
		}
		if (FAILED(hr))
		{
			std::wstringstream ss;
			ss << L"D3D9Texture::LoadTextureFromFile:\nCannot find and/or create the texture from a bitmap.\n";
			ShowMessage(ss, GSMT_ERROR);
			return false;
		}
		if (width == 0 && height == 0)
		{
			m_profile.width = imageInfo.Width;
			m_profile.height = imageInfo.Height;
		}
		else
		{
			m_profile.width = width;
			m_profile.height = height;
		}
		m_profile.originalWidth = imageInfo.Width;
		m_profile.originalHeight = imageInfo.Height;
	}
	m_pTexture = pTexture;

	return true;
}

bool D3D9Texture::GetInternalData()
{
	try
	{
		Video *pVideo = m_video.lock().get();
		m_pDevice = boost::any_cast<IDirect3DDevice9*>(pVideo->GetGraphicContext());
	}
	catch (const boost::bad_any_cast &)
	{
		std::wstringstream ss;
		ss << L"D3D9Texture::GetInternalData: Invalid device";
		ShowMessage(ss, GSMT_ERROR);
		return false;
	}
	return true;
}

Vector2 D3D9Texture::GetBitmapSize() const
{
	return Vector2(static_cast<float>(m_profile.width), static_cast<float>(m_profile.height));
}

bool D3D9Texture::SetTexture(const unsigned int passIdx)
{
	m_pDevice->SetTexture(passIdx, m_pTexture);
	return true;
}

Texture::PROFILE D3D9Texture::GetProfile() const
{
	return m_profile;
}

Texture::TYPE D3D9Texture::GetTextureType() const
{
	return m_texType;
}

boost::any D3D9Texture::GetTextureObject()
{
	return m_pTexture;
}

} // namespace gs2d