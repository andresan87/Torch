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

#include "gs2dD3D9Sprite.h"
#include "../../unicode/utf8converter.h"

namespace gs2d {
using namespace math;

void D3D9Sprite::Init()
{
	m_pTexture = NULL;
	m_pDevice = NULL;
	m_nRects = m_nColumns = m_nRows = 0;
	m_flipX = m_flipY = false;
	m_multiply = Vector2(1.0f, 1.0f);
	m_scroll = Vector2(0.0f, 0.0f);
	m_origin = Vector2(0.0f, 0.0f);
	m_rect = Rect2Df(0,0,0,0);
	m_type = Sprite::T_NOT_LOADED;
	m_pBackupTexture = NULL;
	m_rectMode = GSRM_TWO_TRIANGLES;
	m_targetFormat = GSTF_NONE;
}

bool D3D9Sprite::GetInternalData()
{
	try
	{
		m_pTexture = boost::any_cast<IDirect3DTexture9*>(m_texture->GetTextureObject());
	}
	catch (const boost::bad_any_cast &)
	{
		std::wstringstream ss;
		ss << L"D3D9Sprite::GetInternalData Invalid texture pointer" << std::endl;
		ShowMessage(ss, GSMT_ERROR);
		return false;
	}

	Video *video = m_video.lock().get();
	try
	{
		m_pDevice = boost::any_cast<IDirect3DDevice9*>(video->GetGraphicContext());
	}
	catch (const boost::bad_any_cast &)
	{
		std::wstringstream ss;
		ss << L"D3D9Sprite::GetInternalData Invalid device" << std::endl;
		ShowMessage(ss, GSMT_ERROR);
		return false;
	}

	try
	{
		m_pVideoInfo = boost::any_cast<D3D9VideoInfoPtr>(video->GetVideoInfo());
	}
	catch (const boost::bad_any_cast &)
	{
		std::wstringstream ss;
		ss << L"D3D9Sprite::GetInternalData Video::GetVideoInfo returns an invalid D3D9VideoInfo object" << std::endl;
		ShowMessage(ss, GSMT_ERROR);
		return false;
	}

	return true;
}

D3D9Sprite::D3D9Sprite()
{
	Init();
}

D3D9Sprite::~D3D9Sprite()
{
	if (m_pBackupTexture)
	{
		m_pBackupTexture->Release();
		m_pBackupTexture = NULL;
	}
	D3D9Video::RemoveFromTargetList(this);
}

void D3D9Sprite::OnLostDevice()
{
	m_texture.reset();
	m_type = Sprite::T_RELOAD;
}

void D3D9Sprite::GenerateBackup()
{
	if (m_pBackupTexture)
	{
		m_pBackupTexture->Release();
		m_pBackupTexture = NULL;
	}
	if (m_type == Sprite::T_NOT_LOADED)
		return;

	IDirect3DTexture9* pBackup = NULL;
	IDirect3DSurface9* pBackupSurf = NULL;

	IDirect3DTexture9* pActualTexture;
	try
	{
		pActualTexture = boost::any_cast<IDirect3DTexture9*>(m_texture->GetTextureObject());
	}
	catch (const boost::bad_any_cast &)
	{
		ShowMessage(L"D3D9Sprite::GenerateBackup Invalid texture pointer");
		return;
	}
	IDirect3DSurface9* pActualSurface = NULL;
	D3DSURFACE_DESC desc;

	pActualTexture->GetSurfaceLevel(0, &pActualSurface);
	pActualSurface->GetDesc(&desc);

	m_pDevice->CreateTexture(desc.Width, desc.Height,
		1, 0, desc.Format, D3DPOOL_SYSTEMMEM, &pBackup, NULL);
	pBackup->GetSurfaceLevel(0, &pBackupSurf);

	m_pDevice->GetRenderTargetData(pActualSurface, pBackupSurf);

	pActualSurface->Release();
	pBackupSurf->Release();
	m_pBackupTexture = pBackup;
}

void D3D9Sprite::RecoverFromBackup()
{
	if (!m_pBackupTexture)
		return;

	m_texture = m_video.lock()->CreateRenderTargetTexture(m_size.x, m_size.y, m_targetFormat);

	IDirect3DTexture9* pBackup = m_pBackupTexture;

	IDirect3DTexture9* pActualTexture;
	try 
	{
		pActualTexture = boost::any_cast<IDirect3DTexture9*>(m_texture->GetTextureObject());
	}
	catch (const boost::bad_any_cast &)
	{
		std::wstringstream ss;
		ss << L"D3D9Sprite::RecoverFromBackup Invalid texture pointer" << std::endl;
		ShowMessage(ss, GSMT_ERROR);
		return;
	}
	IDirect3DSurface9* pActualSurface = NULL;
	IDirect3DSurface9* pBackupSurf = NULL;

	pActualTexture->GetSurfaceLevel(0, &pActualSurface);
	pBackup->GetSurfaceLevel(0, &pBackupSurf);
	m_pDevice->UpdateSurface(pBackupSurf, NULL, pActualSurface, NULL);
	
	pBackupSurf->Release();
	pActualSurface->Release();

	GetInternalData();
}

void D3D9Sprite::SetRectMode(const GS_RECT_MODE mode)
{
	m_rectMode = mode;
}

GS_RECT_MODE D3D9Sprite::GetRectMode() const
{
	return m_rectMode;
}

bool D3D9Sprite::SetupSpriteRects(const unsigned int columns, const unsigned int rows)
{
	m_rects.reset();

	if (columns <= 0 || rows <=0)
	{
		m_video.lock()->Message(L"The number of rows or columns set can't be 0 or less - D3D9Sprite::SetupSpriteRects");
		return false;
	}

	m_nColumns = columns;
	m_nRows = rows;
	m_nRects = columns*rows;
	m_rects = boost::shared_array<Rect2Df>(new Rect2Df [m_nRects]);

	const Texture::PROFILE prof = m_texture->GetProfile();
	const unsigned int strideX = prof.width/columns, strideY = prof.height/rows;
	unsigned int index=0;
	for (unsigned int y=0; y<rows; y++)
	{
		for (unsigned int x=0; x<columns; x++)
		{
			m_rects[index].pos.x = static_cast<float>(x*strideX);
			m_rects[index].pos.y = static_cast<float>(y*strideY);
			m_rects[index].size.x = static_cast<float>(strideX);
			m_rects[index].size.y = static_cast<float>(strideY);
			index++;
		}
	}

	SetRect(0);
	return true;
}

bool D3D9Sprite::DrawOptimal(const math::Vector2 &v2Pos, const GS_COLOR color, const float angle, const math::Vector2 &v2Size)
{
	Vector2 size;
	if (v2Size == Vector2(-1, -1))
		size = GetBitmapSizeF();
	else
		size = v2Size;
	return DrawShaped(v2Pos, size, color, color, color, color, angle);
}

bool D3D9Sprite::SetRect(const unsigned int column, const unsigned int row)
{
	if (column >= m_nColumns || row >= m_nRows)
	{
		m_video.lock()->Message(L"Invalid argument - D3D9Sprite::SetRect");
		return false;
	}
	m_currentRect = (row*m_nColumns)+column;
	m_rect = m_rects[m_currentRect];
	return true;
}

unsigned int D3D9Sprite::GetRectIndex() const
{
	return m_currentRect;
}

bool D3D9Sprite::SetRect(const unsigned int rect)
{
	if (rect >= m_nColumns*m_nRows)
	{
		m_video.lock()->Message(L"Invalid argument - D3D9Sprite::SetRect");
		return false;
	}
	m_currentRect = rect;
	m_rect = m_rects[m_currentRect];
	return true;
}

void D3D9Sprite::SetRect(const Rect2Df &rect)
{
	m_rect = rect;
}

void D3D9Sprite::UnsetRect()
{
	m_rect = Rect2Df(0,0,0,0);
}

Rect2Df D3D9Sprite::GetRect() const
{
	return m_rect;
}

int D3D9Sprite::GetNumRects() const
{
	return m_nRects;
}

unsigned int D3D9Sprite::GetNumRows() const
{
	return m_nRows;
}

unsigned int D3D9Sprite::GetNumColumns() const
{
	return m_nColumns;
}

void D3D9Sprite::FlipX(const bool flip)
{
	m_flipX = flip;
}

void D3D9Sprite::FlipY(const bool flip)
{
	m_flipY = flip;
}

void D3D9Sprite::FlipX()
{
	m_flipX = (m_flipX) ? false : true;
}

void D3D9Sprite::FlipY()
{
	m_flipY = (m_flipY) ? false : true;
}

bool D3D9Sprite::GetFlipX() const
{
	return m_flipX;
}

bool D3D9Sprite::GetFlipY() const
{
	return m_flipY;
}

void D3D9Sprite::SetMultiply(const Vector2 &v2Multiply)
{
	m_multiply = Vector2(Abs(v2Multiply.x), Abs(v2Multiply.y));
}

Vector2 D3D9Sprite::GetMultiply() const
{
	return m_multiply;
}

#define GS_MAX_SCROLL (1024.0f*4.0f)
void D3D9Sprite::SetScroll(const Vector2 &v2Scroll)
{
	m_scroll = v2Scroll;
	if (m_scroll.x > GS_MAX_SCROLL)
		m_scroll.x -= GS_MAX_SCROLL;
	else if (m_scroll.x < -GS_MAX_SCROLL)
		m_scroll.x += GS_MAX_SCROLL;

	if (m_scroll.y > GS_MAX_SCROLL)
		m_scroll.y -= GS_MAX_SCROLL;
	else if (m_scroll.y < -GS_MAX_SCROLL)
		m_scroll.y += GS_MAX_SCROLL;
}

Vector2 D3D9Sprite::GetScroll() const
{
	return m_scroll;
}

bool D3D9Sprite::SaveBitmap(const wchar_t *wcsName, const GS_BITMAP_FORMAT fmt, Rect2D *pRect)
{
	if (m_type == Sprite::T_NOT_LOADED)
	{
		m_video.lock()->Message(L"Bad sprite. Not loaded yet - D3D9Sprite::SaveBitmap");
		return false;
	}
	Rect2D rect;
	if (pRect)
	{
		rect = *pRect;
	}
	else
	{
		rect.pos = Vector2i(0,0);
		rect.size = GetBitmapSize();
	}

	IDirect3DSurface9 *pd3dsFront = NULL;

	if (SUCCEEDED(m_pTexture->GetSurfaceLevel(0, &pd3dsFront)))
	{
		RECT wRect = { 0,0,0,0 };
		wRect.left = rect.pos.x;
		wRect.top = rect.pos.y;
		wRect.right = rect.pos.x+rect.size.x;
		wRect.bottom = rect.pos.y+rect.size.y;

		std::wstring finalName;
		std::wstring ext = GetImageExtension(fmt);
		finalName = wcsName;
		if (!IsTheExtensionRight(finalName.c_str(), ext.c_str()))
		{
			finalName += ext;
		}
		if (
			FAILED(
				D3DXSaveSurfaceToFile( finalName.c_str(), GetD3DPF(fmt), pd3dsFront, NULL, &wRect)
			)
		)
		{
			m_video.lock()->Message(L"Couldn't save the sprite - D3D9Sprite::SaveBitmap");
			if (pd3dsFront)
				pd3dsFront->Release();
			return false;
		}
	}
	else
	{
		m_video.lock()->Message(L"Couldn't get surface data - D3D9Sprite::SaveBitmap");
		if (pd3dsFront)
			pd3dsFront->Release();
		return false;
	}

	if (pd3dsFront)
		pd3dsFront->Release();

	return true;
}

bool D3D9Sprite::LoadSprite(VideoWeakPtr video, GS_BYTE *pBuffer, const unsigned int bufferLength,
				GS_COLOR mask, const unsigned int width, const unsigned int height)
{
	m_video = video;
	if (m_type != Sprite::T_NOT_LOADED)
	{
		m_video.lock()->Message(L"The sprite can't be loaded twice - D3D9Sprite::LoadSprite");
		return false;
	}

	m_texture = m_video.lock()->CreateTextureFromFileInMemory(pBuffer, bufferLength, mask, width, height);
	if (!m_texture)
		 return false;

	m_type = Sprite::T_BITMAP;
	Texture::PROFILE profile = m_texture->GetProfile();
	m_size = Vector2i(profile.width, profile.height);

	SetupSpriteRects(1, 1);
	return GetInternalData();
}

bool D3D9Sprite::LoadSprite(VideoWeakPtr video, const std::wstring& fileName, GS_COLOR mask,
				const unsigned int width, const unsigned int height)
{
	m_video = video;
	if (m_type != Sprite::T_NOT_LOADED)
	{
		m_video.lock()->Message(L"The sprite can't be loaded twice - D3D9Sprite::LoadSprite");
		return false;
	}

	m_texture = m_video.lock()->LoadTextureFromFile(fileName, mask, width, height, 0);
	if (!m_texture)
		 return false;

	m_type = Sprite::T_BITMAP;
	Texture::PROFILE profile = m_texture->GetProfile();
	m_size = Vector2i(profile.width, profile.height);

	SetupSpriteRects(1, 1);
	return GetInternalData();
}

bool D3D9Sprite::CreateRenderTarget(VideoWeakPtr video,
							const unsigned int width, const unsigned int height,
							const GS_TARGET_FORMAT format)
{
	if (m_type != Sprite::T_NOT_LOADED && m_type != Sprite::T_RELOAD)
	{
		m_video.lock()->Message(L"The sprite can't be loaded twice - D3D9Sprite::CreateRenderTarget");
		return false;
	}
	m_video = video;
	Video* pVideo = m_video.lock().get();

	const unsigned int screenX = static_cast<unsigned int>(pVideo->GetScreenSize().x);
	const unsigned int screenY = static_cast<unsigned int>(pVideo->GetScreenSize().y);
	if (width > screenX || height > screenY)
	{
		pVideo->Message(L"The render target surface size can't be greater than the backbuffer - D3D9Sprite::CreateRenderTarget");
		return false;
	}

	m_texture = pVideo->CreateRenderTargetTexture(width, height, format);
	if (!m_texture)
		return false;

	m_size = Vector2i(width, height);
	D3D9Video::RENDER_TARGET target(this, width, height, format);
	D3D9Video::AddToTargetList(target);
	m_type = Sprite::T_TARGET;
	m_targetFormat = format;

	SetupSpriteRects(1, 1);
	return GetInternalData();
}

Sprite::TYPE D3D9Sprite::GetType() const
{
	return m_type;
}

Texture::PROFILE D3D9Sprite::GetProfile() const
{
	return m_texture->GetProfile();
}

Vector2i D3D9Sprite::GetBitmapSize() const
{
	return m_size;
}

Vector2 D3D9Sprite::GetBitmapSizeF() const
{
	return Vector2(static_cast<float>(m_size.x), static_cast<float>(m_size.y));
}

bool D3D9Sprite::Draw(const Vector2 &v2Pos, GS_COLOR color,
						   const float angle, const Vector2 &v2Scale)

{
	Vector2 v2Size;
	if (m_rect.size.x != 0)
	{
		v2Size = m_rect.size;
	}
	else
	{
		v2Size = Vector2(static_cast<float>(m_size.x), static_cast<float>(m_size.y));
	}
	v2Size = v2Size * v2Scale;
	return DrawShaped(v2Pos, v2Size, color, color, color, color, angle);
}

bool D3D9Sprite::DrawShaped(const Vector2 &v2Pos, const Vector2 &v2Size,
							 GS_COLOR color0, GS_COLOR color1,
							 GS_COLOR color2, GS_COLOR color3,
							 const float angle)
{
	if (v2Size == Vector2(0,0))
	{
		return true;
	}

	// do the flip (parameters that will be send to the VS)
	Vector2 flipMul(1,1), flipAdd(0,0);
	if (m_flipX)
	{
		flipMul.x =-1;
		flipAdd.x = 1;
	}
	if (m_flipY)
	{
		flipMul.y =-1;
		flipAdd.y = 1;
	}

	// centralizes the sprite according to the origin
	Vector2 v2Center = m_origin*v2Size;

	Video* video = m_video.lock().get();
	ShaderPtr pCurrentVS = video->GetVertexShader();

	Matrix4x4 mRot;
	if (angle != 0.0f)
		mRot = RotateZ(DegreeToRadian(angle));
	pCurrentVS->SetMatrixConstant(L"rotationMatrix", mRot);

	// rounds up the final position to avoid alpha distortion
	Vector2 v2FinalPos;
	if (video->IsRoundingUpPosition())
	{
		v2FinalPos.x = floor(v2Pos.x);
		v2FinalPos.y = floor(v2Pos.y);
	}
	else
	{
		v2FinalPos = v2Pos;
	}

	// subtract 0.5 to align pixel-texel
	v2FinalPos -= constant::HALF_VECTOR2;

	pCurrentVS->SetConstant(L"size", v2Size);
	pCurrentVS->SetConstant(L"entityPos", v2FinalPos);
	pCurrentVS->SetConstant(L"center", v2Center);
	pCurrentVS->SetConstant(L"flipMul", flipMul);
	pCurrentVS->SetConstant(L"flipAdd", flipAdd);
	pCurrentVS->SetConstant(L"bitmapSize", GetBitmapSizeF());
	pCurrentVS->SetConstant(L"scroll", GetScroll());
	pCurrentVS->SetConstant(L"multiply", GetMultiply());

	const bool setCameraPos = pCurrentVS->ConstantExist(L"cameraPos");
	if (setCameraPos)
		pCurrentVS->SetConstant(L"cameraPos", video->GetCameraPos());

	if (m_rect.size.x == 0 || m_rect.size.y == 0)
	{
		pCurrentVS->SetConstant(L"rectSize", GetBitmapSizeF());
		pCurrentVS->SetConstant(L"rectPos", 0, 0);
	}
	else
	{
		pCurrentVS->SetConstant(L"rectSize", m_rect.size);
		pCurrentVS->SetConstant(L"rectPos", m_rect.pos);
	}

	pCurrentVS->SetConstant(L"color0", color0);
	pCurrentVS->SetConstant(L"color1", color1);
	pCurrentVS->SetConstant(L"color2", color2);
	pCurrentVS->SetConstant(L"color3", color3);

	if (pCurrentVS->ConstantExist(L"depth"))
		pCurrentVS->SetConstant(L"depth", video->GetSpriteDepth());

	pCurrentVS->SetShader();

	// apply textures according to the rendering mode (pixel shaded or not)
	ShaderPtr pCurrentPS = video->GetPixelShader();
	if (!pCurrentPS)
	{
		m_pDevice->SetTexture(0, m_pTexture);
		//for (unsigned int t=1; t<GS_TEXTURE_CHANNELS; t++)
		//	pDevice->SetTexture(t, NULL);
	}
	else
	{
		pCurrentPS->SetShader();
		pCurrentPS->SetTexture(L"diffuse", GetTexture());
	}

	// draw the one-pixel-quad applying the vertex shader
	m_pVideoInfo->DrawSprite(m_pDevice, m_rectMode);

	m_pDevice->SetVertexShader(NULL);

	return true;
}

void D3D9Sprite::BeginFastRendering()
{
	Video* video = m_video.lock().get();
	video->SetVertexShader(video->GetFontShader());
	ShaderPtr pCurrentVS = video->GetVertexShader();
	pCurrentVS->SetConstant(L"bitmapSize", GetBitmapSizeF());

	// apply textures according to the rendering mode (pixel shaded or not)
	ShaderPtr pCurrentPS = video->GetPixelShader();
	if (!pCurrentPS)
	{
		m_pDevice->SetTexture(0, m_pTexture);
		//for (unsigned int t=1; t<GS_TEXTURE_CHANNELS; t++)
		//	pDevice->SetTexture(t, NULL);
	}
	else
	{
		pCurrentPS->SetShader();
		pCurrentPS->SetTexture(L"diffuse", GetTexture());
	}
	m_pVideoInfo->BeginFastDraw(m_pDevice, m_rectMode);
}

void D3D9Sprite::EndFastRendering()
{
	m_video.lock()->SetVertexShader(ShaderPtr());
	m_pDevice->SetVertexShader(NULL);
}

bool D3D9Sprite::DrawShapedFast(const Vector2 &v2Pos, const Vector2 &v2Size, const GS_COLOR color)
{
	if (v2Size == Vector2(0,0))
	{
		return true;
	}

	Video* video = m_video.lock().get();
	ShaderPtr pCurrentVS = video->GetVertexShader();

	// rounds up the final position to avoid alpha distortion
	Vector2 v2FinalPos;
	if (video->IsRoundingUpPosition())
	{
		v2FinalPos.x = floor(v2Pos.x);
		v2FinalPos.y = floor(v2Pos.y);
	}
	else
	{
		v2FinalPos = v2Pos;
	}

	// subtract 0.5 to align pixel-texel
	v2FinalPos -= constant::HALF_VECTOR2;

	pCurrentVS->SetConstant(L"size", v2Size);
	pCurrentVS->SetConstant(L"entityPos", v2FinalPos);
	pCurrentVS->SetConstant(L"color0", color);

	if (m_rect.size.x == 0 || m_rect.size.y == 0)
	{
		pCurrentVS->SetConstant(L"rectSize", GetBitmapSizeF());
		pCurrentVS->SetConstant(L"rectPos", 0, 0);
	}
	else
	{
		pCurrentVS->SetConstant(L"rectSize", m_rect.size);
		pCurrentVS->SetConstant(L"rectPos", m_rect.pos);
	}

	pCurrentVS->SetShader();

	// draw the one-pixel-quad applying the vertex shader
	m_pVideoInfo->DrawSpriteFast(m_pDevice, m_rectMode);

	return true;
}

bool D3D9Sprite::SetAsTexture(const unsigned int passIdx)
{
	if (passIdx == 0)
	{
		m_video.lock()->Message(L"The pass 0 is reserved - D3D9Sprite::SetAsTexture");
		return false;
	}
	m_pDevice->SetTexture(passIdx, m_pTexture);
	return true;
}

bool D3D9Sprite::Stretch(const Vector2 &a, const Vector2 &b, const float width,
							  GS_COLOR color0, GS_COLOR color1)
{
	if (a == b || width <= 0.0f)
	{
		return true;
	}

	const Vector2 v2Dir = a - b;
	const float len = Distance(a, b);
	const float angle = RadianToDegree(GetAngle(v2Dir));
	const float lineWidth = (m_rect.size.x <= 0) ? (float)GetProfile().width : (float)m_rect.size.x;

	Vector2 origin = GetOrigin();
	SetOrigin(GSEO_CENTER_BOTTOM);
	const bool r = DrawShaped(a, Vector2((width >= 0.0f) ? width : lineWidth, len),
							  color1, color1, color0, color0, angle);
	SetOrigin(origin);
	return r;
}

boost::any D3D9Sprite::GetTextureObject()
{
	return m_texture->GetTextureObject();
}

Rect2Df D3D9Sprite::GetRect(const unsigned int rect) const
{
	return m_rects[rect];
}

Vector2 D3D9Sprite::GetOrigin() const
{
	return m_origin;
}

void D3D9Sprite::SetOrigin(const Vector2 &origin)
{
	m_origin = origin;
}

void D3D9Sprite::SetOrigin(const GS_ENTITY_ORIGIN origin)
{
	switch (origin)
	{
	case GSEO_RECT_CENTER:
	case GSEO_CENTER:
		m_origin.x = 1.0f/2.0f;
		m_origin.y = 1.0f/2.0f;
		break;
	case GSEO_RECT_CENTER_BOTTOM:
	case GSEO_CENTER_BOTTOM:
		m_origin.x = 1.0f/2.0f;
		m_origin.y = 1.0f;
		break;
	case GSEO_RECT_CENTER_TOP:
	case GSEO_CENTER_TOP:
		m_origin.x = 1.0f/2.0f;
		m_origin.y = 0.0f;
		break;
	default:
		m_origin.x = 0.0f;
		m_origin.y = 0.0f;
		break;
	};
}

TextureWeakPtr D3D9Sprite::GetTexture()
{
	return m_texture;
}

} // namespace gs2d