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

#include "gs2dGLES2Sprite.h"

namespace gs2d {

using namespace math;

const str_type::string GLES2Sprite::SPRITE_LOG_FILE("GLES2Sprite.log.txt");
Platform::FileLogger GLES2Sprite::m_logger(Platform::FileLogger::GetLogPath() + GLES2Sprite::SPRITE_LOG_FILE);

GLES2Sprite::GLES2Sprite(GLES2ShaderContextPtr shaderContext) :
		m_normalizedOrigin(0.0f, 0.0f),
		m_type(T_NOT_LOADED),
		m_currentRect(0),
		m_rect(Vector2(0, 0), Vector2(0, 0)),
		m_nRects(1),
		m_nColumns(1),
		m_nRows(1)
{
	m_shaderContext = shaderContext.get();
}

bool GLES2Sprite::LoadSprite(VideoWeakPtr video, GS_BYTE *pBuffer, const unsigned int bufferLength,
					GS_COLOR mask, const unsigned int width, const unsigned int height)
{
	m_video = video.lock().get();
	// TODO 
	// SetupSpriteRects(1, 1);
	return false;
}

bool GLES2Sprite::LoadSprite(VideoWeakPtr video, const str_type::string& fileName,
				GS_COLOR mask, const unsigned int width, const unsigned int height)
{
	m_video = video.lock().get();
	if (width == 0 || height == 0)
	{
		m_texture = boost::dynamic_pointer_cast<GLES2Texture>(m_video->LoadTextureFromFile(fileName, 0, width, height, 1));
		if (!m_texture)
		{
			m_logger.Log(fileName + " could not load sprite", Platform::FileLogger::ERROR);
			m_video->Message(fileName + " could not load sprite", GSMT_ERROR);
			return false;
		}
		else
		{
			const Texture::PROFILE profile = m_texture->GetProfile();
			m_bitmapSize.x = static_cast<float>(profile.width);
			m_bitmapSize.y = static_cast<float>(profile.height);
		}
	}
	else
	{
		m_bitmapSize.x = static_cast<float>(width);
		m_bitmapSize.y = static_cast<float>(height);
	}
	m_type = T_BITMAP;

	SetupSpriteRects(1, 1);
	return true;
}

bool GLES2Sprite::CreateRenderTarget(VideoWeakPtr video, const unsigned int width, const unsigned int height, const GS_TARGET_FORMAT format)
{
	m_video = video.lock().get();
	m_texture = boost::dynamic_pointer_cast<GLES2Texture>(m_video->CreateRenderTargetTexture(width, height, format));
	m_bitmapSize = Vector2(static_cast<float>(width), static_cast<float>(height));
	m_type = T_TARGET;
	SetupSpriteRects(1, 1);
	return true;
}

bool GLES2Sprite::Draw(const Vector2 &v2Pos,
				const GS_COLOR color, const float angle,
				const Vector2 &v2Scale)
{
	Vector2 v2Size;
	if (m_rect.size.x != 0)
	{
		v2Size = m_rect.size;
	}
	else
	{
		v2Size = m_bitmapSize;
	}
	v2Size = v2Size * v2Scale;

	ShaderPtr
		optimal = m_video->GetOptimalVS(),
		current = m_shaderContext->GetCurrentVS(),
		defaultS = m_video->GetDefaultVS();
	if (current == optimal || current == defaultS)
	{
		if (current != optimal)
			m_video->SetVertexShader(optimal);
		DrawOptimal(v2Pos, color, angle, v2Size);
		return true;
	}
	else
	{
		return DrawShaped(v2Pos, v2Size, color, color, color, color, angle);
	}
}

bool GLES2Sprite::DrawShaped(const Vector2 &v2Pos, const Vector2 &v2Size,
							const GS_COLOR color0, const GS_COLOR color1,
							const GS_COLOR color2, const GS_COLOR color3,
							const float angle)
{
	ShaderPtr
		optimal = m_video->GetOptimalVS(),
		current = m_shaderContext->GetCurrentVS(),
		defaultS = m_video->GetDefaultVS();
	if (current == optimal || current == defaultS)
	{
		if (color0 == color1 && color0 == color2 && color0 == color3)
		{
			if (current != optimal)
				m_video->SetVertexShader(optimal);
			DrawOptimal(v2Pos, color0, angle, v2Size);
			return true;
		}
	}

	#ifdef GS2D_WARN_SLOW_DRAWING
	if (current == optimal)
		m_logger.Log(m_texture->GetFileName() + ": slow drawing!", Platform::FileLogger::ERROR);
	#endif

	GLES2Shader* vs = m_shaderContext->GetCurrentVS().get();
	GLES2Shader* ps = m_shaderContext->GetCurrentPS().get();

	Vector2 pos(v2Pos), camPos(m_video->GetCameraPos()), center(m_normalizedOrigin*v2Size);
	if (m_video->IsRoundingUpPosition())
	{
		pos.x = floor(pos.x);
		pos.y = floor(pos.y);
		camPos.x = floor(camPos.x);
		camPos.y = floor(camPos.y);
		center.x = floor(center.x);
		center.y = floor(center.y);
	}

	static const std::size_t ROTATION_MATRIX_HASH = fastHash("rotationMatrix");
	static const std::size_t RECT_SIZE_HASH = fastHash("rectSize");
	static const std::size_t RECT_POS_HASH = fastHash("rectPos");
	static const std::size_t BITMAP_SIZE_HASH = fastHash("bitmapSize");
	static const std::size_t ENTITY_POS_HASH = fastHash("entityPos");
	static const std::size_t CENTER_HASH = fastHash("center");
	static const std::size_t SIZE_HASH = fastHash("size");
	static const std::size_t COLOR0_HASH = fastHash("color0");
	static const std::size_t COLOR1_HASH = fastHash("color1");
	static const std::size_t COLOR2_HASH = fastHash("color2");
	static const std::size_t COLOR3_HASH = fastHash("color3");
	static const std::size_t CAMERA_POS_HASH = fastHash("cameraPos");

	Matrix4x4 mRot;
	if (angle != 0.0f)
	{
		mRot = RotateZ(DegreeToRadian(-angle)); 
	}
	vs->SetMatrixConstant(ROTATION_MATRIX_HASH, "rotationMatrix", mRot);

	ps->SetTexture("diffuse", m_texture);
	
	if (m_rect.size.x == 0 || m_rect.size.y == 0)
	{
		vs->SetConstant(RECT_SIZE_HASH, "rectSize", GetBitmapSizeF());
		vs->SetConstant(RECT_POS_HASH, "rectPos", Vector2(0, 0));
	}
	else
	{
		vs->SetConstant(RECT_SIZE_HASH, "rectSize", m_rect.size);
		vs->SetConstant(RECT_POS_HASH, "rectPos", m_rect.pos);
	}

	vs->SetConstant(BITMAP_SIZE_HASH, "bitmapSize", m_bitmapSize);
	vs->SetConstant(ENTITY_POS_HASH, "entityPos", pos);
	vs->SetConstant(CENTER_HASH, "center", center);
	vs->SetConstant(SIZE_HASH, "size", v2Size);
	vs->SetConstant(COLOR0_HASH, "color0", color0);
	vs->SetConstant(COLOR1_HASH, "color1", color1);
	vs->SetConstant(COLOR2_HASH, "color2", color2);
	vs->SetConstant(COLOR3_HASH, "color3", color3);
	vs->SetConstant(CAMERA_POS_HASH, "cameraPos", camPos);
	m_shaderContext->DrawRect();
	return true;
}

bool GLES2Sprite::DrawOptimal(const math::Vector2 &v2Pos, const GS_COLOR color, const float angle, const Vector2 &v2Size)
{
	GLES2Shader* vs = m_shaderContext->GetCurrentVS().get();
	GLES2Shader* ps = m_shaderContext->GetCurrentPS().get();

	Vector2 size((v2Size != Vector2(-1, -1)) ? v2Size : m_bitmapSize);
	Vector2 pos(v2Pos), camPos(m_video->GetCameraPos()), center(m_normalizedOrigin*size);

	if (m_video->IsRoundingUpPosition())
	{
		// do it as fast as possible...
		pos.x = floor(pos.x);
		pos.y = floor(pos.y);
		camPos.x = floor(camPos.x);
		camPos.y = floor(camPos.y);
		center.x = floor(center.x);
		center.y = floor(center.y);
	}

	static const std::size_t ROTATION_MATRIX_HASH = fastHash("rotationMatrix");
	static const std::size_t COLOR_HASH = fastHash("color");
	static const std::size_t PARAMS_HASH = fastHash("params");

	Matrix4x4 mRot;
	if (angle != 0.0f)
	{
		mRot = RotateZ(DegreeToRadian(-angle)); 
	}
	vs->SetMatrixConstant(ROTATION_MATRIX_HASH, "rotationMatrix", mRot);

	ps->SetTexture("diffuse", m_texture);

	Vector2 rectPos, rectSize;
	if (m_rect.size.x == 0 || m_rect.size.y == 0)
	{
		rectPos = Vector2(0, 0);
		rectSize = GetBitmapSizeF();
	}
	else
	{
		rectPos = m_rect.pos;
		rectSize = m_rect.size;
	}

	static const unsigned int numParams = 7;
	Vector2 *params = new Vector2 [numParams];
	params[0] = rectPos;
	params[1] = rectSize;
	params[2] = center;
	params[3] = size;
	params[4] = pos;
	params[5] = camPos;
	params[6] = m_bitmapSize;

	vs->SetConstantArray(PARAMS_HASH, "params", numParams, boost::shared_array<const math::Vector2>(params));
	vs->SetConstant(COLOR_HASH, "color", color);
	m_shaderContext->DrawRect();
	return true;
}

bool GLES2Sprite::Stretch(const Vector2 &a, const Vector2 &b, const float width,
				   const GS_COLOR color0, const GS_COLOR color1)
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

bool GLES2Sprite::SaveBitmap(const wchar_t *wcsName, const GS_BITMAP_FORMAT fmt, Rect2D *pRect)
{
	// TODO 
	return false;
}

bool GLES2Sprite::DrawShapedFast(const Vector2 &v2Pos, const Vector2 &v2Size, const GS_COLOR color)
{
	GLES2Shader* vs = m_shaderContext->GetCurrentVS().get();

	static const std::size_t COLOR_HASH = fastHash("color");
	static const std::size_t PARAMS_HASH = fastHash("params");

	Vector2 rectSize, rectPos;
	if (m_rect.size.x == 0 || m_rect.size.y == 0)
	{
		rectSize = GetBitmapSizeF();
		rectPos = Vector2(0, 0);
	}
	else
	{
		rectSize = m_rect.size;
		rectPos = m_rect.pos;
	}	

	static const unsigned int numParams = 5;
	Vector2 *params = new Vector2 [numParams];
	params[0] = rectPos;
	params[1] = rectSize;
	params[2] = v2Size;
	params[3] = v2Pos;
	params[4] = m_bitmapSize;

	vs->SetConstantArray(PARAMS_HASH, "params", numParams, boost::shared_array<const math::Vector2>(params));
	vs->SetConstant(COLOR_HASH, "color", color);
	m_shaderContext->FastDraw();
	return true;
}

void GLES2Sprite::BeginFastRendering()
{
	GLES2ShaderPtr ps = static_cast<GLES2Video*>(m_video)->GetDefaultPS();
	ps->SetTexture("diffuse", m_texture);
	m_video->SetVertexShader(m_video->GetFontShader());
	m_video->SetPixelShader(ps);
	m_shaderContext->BeginFastDraw();
}

void GLES2Sprite::EndFastRendering()
{
	m_shaderContext->EndFastDraw();
	m_video->SetVertexShader(ShaderPtr());
}

TextureWeakPtr GLES2Sprite::GetTexture()
{
	return m_texture;
}

void GLES2Sprite::SetOrigin(const GS_ENTITY_ORIGIN origin)
{
	switch (origin)
	{
	case GSEO_RECT_CENTER:
	case GSEO_CENTER:
		m_normalizedOrigin.x = 1.0f/2.0f;
		m_normalizedOrigin.y = 1.0f/2.0f;
		break;
	case GSEO_RECT_CENTER_BOTTOM:
	case GSEO_CENTER_BOTTOM:
		m_normalizedOrigin.x = 1.0f/2.0f;
		m_normalizedOrigin.y = 1.0f;
		break;
	case GSEO_RECT_CENTER_TOP:
	case GSEO_CENTER_TOP:
		m_normalizedOrigin.x = 1.0f/2.0f;
		m_normalizedOrigin.y = 0.0f;
		break;
	default:
		m_normalizedOrigin.x = 0.0f;
		m_normalizedOrigin.y = 0.0f;
		break;
	};
}

void GLES2Sprite::SetOrigin(const Vector2 &v2Custom)
{
	m_normalizedOrigin = v2Custom; 
}

Vector2 GLES2Sprite::GetOrigin() const
{
	return m_normalizedOrigin;
}

bool GLES2Sprite::SetupSpriteRects(const unsigned int columns, const unsigned int rows)
{
	m_rects.reset();

	if (columns <= 0 || rows <=0)
	{
		m_logger.Log(m_texture->GetFileName() + ": number of rows or columns set can't be 0 or less - ::SetupSpriteRects", Platform::FileLogger::ERROR);
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

bool GLES2Sprite::SetRect(const unsigned int column, const unsigned int row)
{
	if (column >= m_nColumns || row >= m_nRows)
	{
		m_logger.Log(m_texture->GetFileName() + " invalid argument - ::SetRect", Platform::FileLogger::ERROR);
		return false;
	}
	m_currentRect = (row*m_nColumns)+column;
	m_rect = m_rects[m_currentRect];
	return true;
}

bool GLES2Sprite::SetRect(const unsigned int rect)
{
	if (rect >= m_nColumns*m_nRows)
	{
		m_logger.Log(m_texture->GetFileName() + " invalid argument - ::SetRect", Platform::FileLogger::ERROR);
		return false;
	}
	m_currentRect = rect;
	m_rect = m_rects[m_currentRect];
	return true;
}

void GLES2Sprite::SetRect(const Rect2Df &rect)
{
	m_rect = rect;
}

void GLES2Sprite::UnsetRect()
{
	m_rect = Rect2Df(0,0,0,0);
}

int GLES2Sprite::GetNumRects() const
{
	return m_nRects;
}

Rect2Df GLES2Sprite::GetRect() const
{
	return m_rect;
}

Rect2Df GLES2Sprite::GetRect(const unsigned int rect) const
{
	return m_rects[rect];
}

unsigned int GLES2Sprite::GetRectIndex() const
{
	return m_currentRect;
}

Texture::PROFILE GLES2Sprite::GetProfile() const
{
	return m_texture->GetProfile();
}

Vector2i GLES2Sprite::GetBitmapSize() const
{
	return Vector2i(static_cast<int>(m_bitmapSize.x), static_cast<int>(m_bitmapSize.y));
}

Vector2 GLES2Sprite::GetBitmapSizeF() const
{
	return m_bitmapSize;
}

unsigned int GLES2Sprite::GetNumRows() const
{
	return m_nRows;
}

unsigned int GLES2Sprite::GetNumColumns() const
{
	return m_nColumns;
}

void GLES2Sprite::FlipX(const bool flip)
{
	// TODO 
}

void GLES2Sprite::FlipY(const bool flip)
{
	// TODO 
}

void GLES2Sprite::FlipX()
{
	// TODO 
}

void GLES2Sprite::FlipY()
{
	// TODO 
}

bool GLES2Sprite::GetFlipX() const
{
	// TODO 
	return false;
}

bool GLES2Sprite::GetFlipY() const
{
	// TODO 
	return false;
}

void GLES2Sprite::SetScroll(const Vector2 &v2Scroll)
{
	// TODO 
}

Vector2 GLES2Sprite::GetScroll() const
{
	// TODO 
	return Vector2();
}

void GLES2Sprite::SetMultiply(const Vector2 &v2Multiply)
{
	// TODO 
}

Vector2 GLES2Sprite::GetMultiply() const
{
	// TODO 
	return Vector2();
}

Sprite::TYPE GLES2Sprite::GetType() const
{
	return m_type;;
}

boost::any GLES2Sprite::GetTextureObject()
{
	return m_texture->GetTextureID();
}

void GLES2Sprite::GenerateBackup()
{
	// TODO 
}

bool GLES2Sprite::SetAsTexture(const unsigned int passIdx)
{
	// TODO 
	return false;
}

void GLES2Sprite::SetRectMode(const GS_RECT_MODE mode)
{
	// TODO 
}

GS_RECT_MODE GLES2Sprite::GetRectMode() const
{
	// TODO 
	return GSRM_TWO_TRIANGLES;
}

void GLES2Sprite::OnLostDevice()
{
	// TODO 
}

void GLES2Sprite::RecoverFromBackup()
{
	// TODO 
}

} // namespace gs2d