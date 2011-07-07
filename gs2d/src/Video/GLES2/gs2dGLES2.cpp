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

#include "gs2dGLES2.h"

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include "../../Platform/android/Platform.android.h"
#include "../../gs2dshader.h"
#include "gs2dGLES2Shader.h"
#include "gs2dGLES2Sprite.h"

#include <sstream>

namespace gs2d {

using namespace math;

const float GLES2Video::ZFAR = 5.0f;
const float GLES2Video::ZNEAR = 0.0f;
const str_type::string GLES2Video::VIDEO_LOG_FILE("GLES2Video.log.txt");
const unsigned long GLES2Video::ALPHAREF = (0x01);

str_type::string GetFileName(const str_type::string& source)
{
	str_type::string::size_type length = source.length();
	str_type::string::size_type t = length-1;
	for (; t >= 0; --t)
	{
		if (source[t] == '\\' || source[t] == '/')
			break;
	}
	return source.substr(++t);
}

GLES2ShaderPtr LoadInternalShader(GLES2Video* video, const str_type::string& str, const GS_SHADER_FOCUS focus)
{
	GLES2ShaderPtr shader = video->LoadGLES2ShaderFromFile(str, focus);
	if (shader)
	{
		video->Message(str + " default shader created successfully", GSMT_INFO);
	}
	else
	{
		video->Message(str + " couldn't create default shader", GSMT_ERROR);
	}
	return shader;
}

GS2D_API VideoPtr CreateVideo(const unsigned int width, const unsigned int height,
				const str_type::string& bitmapFontDefaultPath, Platform::FileManagerPtr fileManager)
{
	return CreateVideo(width, height, "GS2D", false, true, bitmapFontDefaultPath, GSPF_DEFAULT, false, fileManager);
}

GS2D_API VideoPtr CreateVideo(const unsigned int width, const unsigned int height,
				const str_type::string& winTitle, const bool windowed, const bool sync,
				const str_type::string& bitmapFontDefaultPath,
				const GS_PIXEL_FORMAT pfBB, const bool maximizable, Platform::FileManagerPtr fileManager)
{
	return GLES2Video::Create(width, height, winTitle, bitmapFontDefaultPath, fileManager);
}

GLES2Video::GLES2Video(const unsigned int width, const unsigned int height,
		const str_type::string& winTitle, const str_type::string& bitmapFontDefaultPath,
		Platform::FileManagerPtr fileManager) :
	m_backgroundColor(GS_BLACK),
	m_screenSize(width, height),
	m_windowTitle(winTitle),
	m_quit(false),
	m_rendering(false),
	m_logger(Platform::FileLogger::GetLogPath() + VIDEO_LOG_FILE),
	m_fileManager(fileManager),
	m_defaultBitmapFontPath(bitmapFontDefaultPath),
	m_fpsRate(30.0f),
	m_roundUpPosition(false),
	m_scissor(Vector2i(0, 0), Vector2i(0, 0)),
	m_textureFilterMode(GSTM_IFNEEDED)
{
	m_startTimeMS = clock()/1000;
	m_logger.Log("Creating shader context...", Platform::FileLogger::INFO);
	m_shaderContext = GLES2ShaderContextPtr(new GLES2ShaderContext(this));
	m_logger.Log("StartApplication...", Platform::FileLogger::INFO);
	StartApplication(width, height, winTitle, false, false, bitmapFontDefaultPath, GSPF_DEFAULT, false);
}

boost::shared_ptr<GLES2Video> GLES2Video::Create(const unsigned int width, const unsigned int height,
		const str_type::string& winTitle, const str_type::string& bitmapFontDefaultPath, Platform::FileManagerPtr fileManager)
{
	boost::shared_ptr<GLES2Video> p(new GLES2Video(width, height, winTitle, bitmapFontDefaultPath, fileManager));
	p->weak_this = p;
	return p;
}

bool GLES2Video::StartApplication(const unsigned int width, const unsigned int height,
		const str_type::string& winTitle, const bool windowed,
		const bool sync, const str_type::string& bitmapFontDefaultPath,
		const GS_PIXEL_FORMAT pfBB,	const bool maximizable)
{
	glDisable(GL_DITHER);
	glHint(GL_GENERATE_MIPMAP_HINT, GL_FASTEST);

	m_defaultVS = LoadInternalShader(this, "assets/shaders/default/default.vs", GSSF_VERTEX);
	m_defaultPS = LoadInternalShader(this, "assets/shaders/default/default.ps", GSSF_PIXEL);
	m_fastRenderVS = LoadInternalShader(this, "assets/shaders/default/fastRender.vs", GSSF_VERTEX);
	m_optimalVS = LoadInternalShader(this, "assets/shaders/default/optimal.vs", GSSF_VERTEX);

	m_shaderContext->SetShader(m_defaultVS, m_defaultPS, m_orthoMatrix, GetScreenSizeF());

	SetZBuffer(false);
	SetFilterMode(GSTM_IFNEEDED);
	UnsetScissor();

	ResetVideoMode(width, height, pfBB, false);
	m_logger.Log("Application started...", Platform::FileLogger::INFO);
}

void GLES2Video::Enable2D(const int width, const int height, const bool flipY)
{
	glViewport(0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(height));
	glDisable(GL_CULL_FACE);
	Orthogonal(m_orthoMatrix, static_cast<float>(width), static_cast<float>(height) * ((flipY) ? -1.0f : 1.0f), ZNEAR, ZFAR);

	glDepthFunc(GL_LEQUAL);
	glDepthRangef(0.0f, 1.0f);
}

void GLES2Video::CheckGLError(const str_type::string& op, const Platform::FileLogger& logger)
{
	for (GLint error = glGetError(); error; error = glGetError())
	{
		std::stringstream ss;
		ss << "ERROR: after " << op << ". Error code " << error;
		logger.Log(ss.str(), Platform::FileLogger::ERROR);
	}
}

TexturePtr GLES2Video::CreateTextureFromFileInMemory(const void *pBuffer,
		const unsigned int bufferLength, GS_COLOR mask,
		const unsigned int width, const unsigned int height,
		const unsigned int nMipMaps)
{
	TexturePtr texture(new GLES2Texture(weak_this, GS_L("from_memory"), boost::dynamic_pointer_cast<Platform::ZipFileManager>(m_fileManager)));
	if (texture->LoadTexture(weak_this, pBuffer, bufferLength, mask, width, height, nMipMaps))
	{
		return texture;
	}
	return TexturePtr();
}

TexturePtr GLES2Video::LoadTextureFromFile(const str_type::string& fileName,
		GS_COLOR mask, const unsigned int width,
		const unsigned int height, const unsigned int nMipMaps)
{
	TexturePtr texture(new GLES2Texture(weak_this, fileName, boost::dynamic_pointer_cast<Platform::ZipFileManager>(m_fileManager)));
	if (texture->LoadTexture(weak_this, fileName, mask, width, height, nMipMaps))
	{
		return texture;
	}
	return TexturePtr();
}

TexturePtr GLES2Video::CreateRenderTargetTexture(const unsigned int width, const unsigned int height, const GS_TARGET_FORMAT fmt)
{
	TexturePtr texture(new GLES2Texture(weak_this, GS_L("render_target"), boost::dynamic_pointer_cast<Platform::ZipFileManager>(m_fileManager)));
	if (texture->CreateRenderTarget(weak_this, width, height, fmt))
	{
		return texture;
	}
	return TexturePtr();
}

SpritePtr GLES2Video::CreateSprite(GS_BYTE *pBuffer, const unsigned int bufferLength,
		GS_COLOR mask, const unsigned int width, const unsigned int height)
{
	// TODO
	return SpritePtr();
}

SpritePtr GLES2Video::CreateSprite(const str_type::string& fileName,
		GS_COLOR mask, const unsigned int width, const unsigned int height)
{
	SpritePtr sprite(new GLES2Sprite(m_shaderContext));
	if (sprite->LoadSprite(weak_this, fileName, mask, width, height))
	{
		return sprite;
	}
	return SpritePtr();
}

SpritePtr GLES2Video::CreateRenderTarget(const unsigned int width,
		const unsigned int height, const GS_TARGET_FORMAT format)
{
	SpritePtr sprite(new GLES2Sprite(m_shaderContext));
	if (sprite->CreateRenderTarget(weak_this, width, height, format))
	{
		return sprite;
	}
	return SpritePtr();
}

ShaderPtr GLES2Video::LoadShaderFromFile(const str_type::string& fileName,
		const GS_SHADER_FOCUS focus, const GS_SHADER_PROFILE profile,
		const char *entry)
{
	GLES2ShaderPtr shader(new GLES2Shader(m_fileManager, m_shaderContext));
	if (shader->LoadShaderFromFile(m_shaderContext, fileName, focus, profile, entry))
	{
		return shader;
	}
	return ShaderPtr();
}

ShaderPtr GLES2Video::LoadShaderFromString(const str_type::string& shaderName,
		const std::string& codeAsciiString, const GS_SHADER_FOCUS focus, const GS_SHADER_PROFILE profile,
		const char *entry)
{
	GLES2ShaderPtr shader(new GLES2Shader(m_fileManager, m_shaderContext));
	if (shader->LoadShaderFromString(m_shaderContext, shaderName, codeAsciiString, focus, profile, entry))
	{
		return shader;
	}
	return ShaderPtr();
}

GLES2ShaderPtr GLES2Video::LoadGLES2ShaderFromFile(const str_type::string& fileName, const GS_SHADER_FOCUS focus)
{
	GLES2ShaderPtr shader(new GLES2Shader(m_fileManager, m_shaderContext));
	if (shader->LoadShaderFromFile(m_shaderContext, fileName, focus))
	{
		return shader;
	}
	return GLES2ShaderPtr();
}

GLES2ShaderPtr GLES2Video::LoadGLES2ShaderFromString(const str_type::string& shaderName,
		const std::string& codeAsciiString, const GS_SHADER_FOCUS focus)
{
	GLES2ShaderPtr shader(new GLES2Shader(m_fileManager, m_shaderContext));
	if (shader->LoadShaderFromString(m_shaderContext, shaderName, codeAsciiString, focus))
	{
		return shader;
	}
	return GLES2ShaderPtr();
}

boost::any GLES2Video::GetVideoInfo()
{
	// TODO
	return 0;
}

ShaderPtr GLES2Video::GetFontShader()
{
	return m_fastRenderVS;
}

ShaderPtr GLES2Video::GetOptimalVS()
{
	return m_optimalVS;
}

ShaderPtr GLES2Video::GetDefaultVS()
{
	return m_defaultVS;
}

ShaderPtr GLES2Video::GetVertexShader()
{
	return m_shaderContext->GetCurrentVS();
}

GLES2ShaderPtr GLES2Video::GetDefaultPS()
{
	return m_defaultPS;
}

ShaderPtr GLES2Video::GetPixelShader()
{
	return m_shaderContext->GetCurrentPS();
}

ShaderContextPtr GLES2Video::GetShaderContext()
{
	return m_shaderContext;
}

bool GLES2Video::SetVertexShader(ShaderPtr pShader)
{
	if (pShader)
	{
		m_shaderContext->SetShader(
				boost::dynamic_pointer_cast<GLES2Shader>(pShader), boost::dynamic_pointer_cast<GLES2Shader>(m_shaderContext->GetCurrentPS()),
				m_orthoMatrix, GetCurrentTargetSize());
	}
	else
	{
		m_shaderContext->SetShader(m_defaultVS, boost::dynamic_pointer_cast<GLES2Shader>(m_shaderContext->GetCurrentPS()),
				m_orthoMatrix, GetCurrentTargetSize());
	}
	return true;
}

bool GLES2Video::SetPixelShader(ShaderPtr pShader)
{
	if (pShader)
	{
		m_shaderContext->SetShader(
				boost::dynamic_pointer_cast<GLES2Shader>(m_shaderContext->GetCurrentVS()), boost::dynamic_pointer_cast<GLES2Shader>(pShader),
				m_orthoMatrix, GetCurrentTargetSize());
	}
	else
	{
		m_shaderContext->SetShader(boost::dynamic_pointer_cast<GLES2Shader>(m_shaderContext->GetCurrentVS()), m_defaultPS,
				m_orthoMatrix, GetCurrentTargetSize());
	}
	return true;
}

GS_SHADER_PROFILE GLES2Video::GetHighestVertexProfile() const
{
	// TODO
	return GSSP_NONE;
}

GS_SHADER_PROFILE GLES2Video::GetHighestPixelProfile() const
{
	// TODO
	return GSSP_NONE;
}

boost::any GLES2Video::GetGraphicContext()
{
	str_type::string out;
	ForwardCommands(out);
	return out;
}

Video::VIDEO_MODE GLES2Video::GetVideoMode(const unsigned int modeIdx) const
{
	VIDEO_MODE vm;
	vm.width = m_screenSize.x;
	vm.height = m_screenSize.y;
	vm.pf = GSPF_DEFAULT;
	vm.idx = 0x01;
	return vm;
}

unsigned int GLES2Video::GetVideoModeCount()
{
	return 1;
}

bool GLES2Video::ResetVideoMode(const VIDEO_MODE mode,
		const bool toggleFullscreen)
{
	ResetVideoMode(mode.width, mode.height, mode.pf, false);
	return true;
}

bool GLES2Video::ResetVideoMode(const unsigned int width, const unsigned int height, const GS_PIXEL_FORMAT pfBB, const bool toggleFullscreen)
{
	m_screenSize.x = width;
	m_screenSize.y = height;
	
	Enable2D(width, height);
	m_shaderContext->ResetViewConstants(m_orthoMatrix, GetScreenSizeF());

	return true;
}

bool GLES2Video::SetRenderTarget(SpritePtr pTarget, const unsigned int target)
{
	if (!pTarget)
	{
		m_currentTarget.reset();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	else
	{
		if (pTarget->GetType() == Sprite::T_TARGET)
		{
			m_currentTarget = pTarget->GetTexture();
		}
		else
		{
			Message(GS_L("The current sprite has no render target texture"), GSMT_ERROR);
		}
	}
	return true;
}

unsigned int GLES2Video::GetMaxRenderTargets() const
{
	return 1;
}

unsigned int GLES2Video::GetMaxMultiTextures() const
{
	return 2;
}

bool GLES2Video::SetBlendMode(const unsigned int passIdx, const GS_BLEND_MODE mode)
{
	// TODO
	return false;
}

GS_BLEND_MODE GLES2Video::GetBlendMode(const unsigned int passIdx) const
{
	// TODO
	return GSBM_ADD;
}

bool GLES2Video::UnsetTexture(const unsigned int passIdx)
{
	// TODO
	return false;
}

void GLES2Video::SetZBuffer(const bool enable)
{
	if (enable)
	{
		glEnable(GL_DEPTH_TEST);
	}
	else
	{
		glDisable(GL_DEPTH_TEST);
	}
}

bool GLES2Video::GetZBuffer() const
{
	GLboolean enabled;
	glGetBooleanv(GL_DEPTH_TEST, &enabled);
	return static_cast<bool>(enabled);
}

void GLES2Video::SetZWrite(const bool enable)
{
	// TODO
}

bool GLES2Video::GetZWrite() const
{
	// TODO
	return false;
}

bool GLES2Video::SetClamp(const bool set)
{
	// TODO
	return false;
}

bool GLES2Video::GetClamp() const
{
	// TODO
	return false;
}

bool GLES2Video::SetSpriteDepth(const float depth)
{
	return m_shaderContext->SetSpriteDepth(depth);
}

float GLES2Video::GetSpriteDepth() const
{
	return m_shaderContext->GetSpriteDepth();
}

void GLES2Video::SetLineWidth(const float width)
{
	// TODO
}

float GLES2Video::GetLineWidth() const
{
	// TODO
	return 0.0f;
}

bool GLES2Video::SetCameraPos(const Vector2 &pos)
{
	if (IsRoundingUpPosition())
	{
		m_v2Camera.x = floor(pos.x);
		m_v2Camera.y = floor(pos.y);
		return true;
	}
	m_v2Camera = pos;
	return true;
}

bool GLES2Video::MoveCamera(const Vector2 &dir)
{
	SetCameraPos(m_v2Camera + dir);
	return true;
}

Vector2 GLES2Video::GetCameraPos() const
{
	return m_v2Camera;
}

void GLES2Video::RoundUpPosition(const bool roundUp)
{
	m_roundUpPosition = roundUp;
}

bool GLES2Video::IsRoundingUpPosition() const
{
	return m_roundUpPosition;
}

bool GLES2Video::SetScissor(const Rect2D &rect)
{
	glEnable(GL_SCISSOR_TEST);
	m_scissor = rect;
	glScissor(static_cast<GLint>(rect.pos.x), static_cast<GLint>(m_screenSize.y) - static_cast<GLint>(rect.pos.y + rect.size.y),
			  static_cast<GLsizei>(rect.size.x), static_cast<GLsizei>(rect.size.y));
	return true;
}

Rect2D GLES2Video::GetScissor() const
{
	return m_scissor;
}

void GLES2Video::UnsetScissor()
{
	m_scissor = Rect2D(Vector2i(0, 0), Vector2i(0, 0));
	glDisable(GL_SCISSOR_TEST);
}

void GLES2Video::SetBitmapFontDefaultPath(const str_type::string& path)
{
	m_defaultBitmapFontPath = path;
}

str_type::string GLES2Video::GetBitmapFontDefaultPath() const
{
	return m_defaultBitmapFontPath;
}

BitmapFontPtr GLES2Video::LoadBitmapFont(const str_type::string& fullFilePath)
{
	str_type::string out;
	m_fileManager->GetAnsiFileString(fullFilePath, out);
	if (out != "")
	{
		BitmapFontPtr newFont = BitmapFontPtr(new BitmapFont(weak_this, fullFilePath, out));
		if (newFont->IsLoaded())
		{
			m_fonts[GetFileName(fullFilePath)] = newFont;
			Message(fullFilePath + " bitmap font created.", GSMT_INFO);
			return newFont;
		}
		else
		{
			str_type::string errorMsg = "Invalid font file ";
			errorMsg += fullFilePath;
			Message(errorMsg, GSMT_WARNING);
			return BitmapFontPtr();
		}
	}
	else
	{
		str_type::string errorMsg = "Font file not found ";
		errorMsg += fullFilePath;
		Message(errorMsg, GSMT_WARNING);
		return BitmapFontPtr();
	}
}

Vector2 GLES2Video::ComputeCarretPosition(const str_type::string& font,	const str_type::string& text, const unsigned int pos)
{
	std::map<str_type::string, BitmapFontPtr>::iterator iter = m_fonts.find(font);
	BitmapFontPtr bitmapFont;
	if (iter == m_fonts.end())
	{
		bitmapFont = LoadBitmapFont(m_defaultBitmapFontPath + font);
		if (!bitmapFont)
		{
			Message(GS_L(font + ": couldn't create bitmap font"), GSMT_ERROR);
			return Vector2(0,0);
		}
	}
	else
	{
		bitmapFont = iter->second;
	}
	return bitmapFont->ComputeCarretPosition(text, pos);
}

Vector2 GLES2Video::ComputeTextBoxSize(const str_type::string& font, const str_type::string& text)
{
	std::map<str_type::string, BitmapFontPtr>::iterator iter = m_fonts.find(font);
	BitmapFontPtr bitmapFont;
	if (iter == m_fonts.end())
	{
		bitmapFont = LoadBitmapFont(m_defaultBitmapFontPath + font);
		if (!bitmapFont)
		{
			Message(GS_L(font + ": couldn't create bitmap font"), GSMT_ERROR);
			return Vector2(0,0);
		}
	}
	else
	{
		bitmapFont = iter->second;
	}
	return bitmapFont->ComputeTextBoxSize(text);
}

unsigned int GLES2Video::FindClosestCarretPosition(const str_type::string& font,
		const str_type::string &text, const Vector2 &textPos, const Vector2 &reference)
{
	std::map<str_type::string, BitmapFontPtr>::iterator iter = m_fonts.find(font);
	BitmapFontPtr bitmapFont;
	if (iter == m_fonts.end())
	{
		bitmapFont = LoadBitmapFont(m_defaultBitmapFontPath + font);
		if (!bitmapFont)
		{
			Message(GS_L(font + ": couldn't create bitmap font"), GSMT_ERROR);
			return 0;
		}
	}
	else
	{
		bitmapFont = iter->second;
	}
	return bitmapFont->FindClosestCarretPosition(text, textPos, reference);
}

bool GLES2Video::DrawBitmapText(const Vector2 &v2Pos, const str_type::string& text, const str_type::string& font, const GS_COLOR color)
{
	// checks if this font has already been created
	std::map<str_type::string, BitmapFontPtr>::iterator iter = m_fonts.find(font);

	BitmapFontPtr bmfont;

	// if not... create a new one
	if (iter == m_fonts.end())
	{
		bmfont = LoadBitmapFont(m_defaultBitmapFontPath + font);
		if (!bmfont)
		{
			Message(GS_L(font + ": couldn't create bitmap font"), GSMT_ERROR);
			return false;
		}
	}
	else
	{
		bmfont = iter->second;
	}

	if (bmfont)
	{
		// TODO text rect stuff
		bmfont->DrawBitmapText(v2Pos, text, color);
	}
	return true;
}

bool GLES2Video::DrawLine(const Vector2 &p1, const Vector2 &p2,
		const GS_COLOR color1, const GS_COLOR color2)
{
	// TODO
	return false;
}

bool GLES2Video::DrawRectangle(const Vector2 &v2Pos, const Vector2 &v2Size,
		const GS_COLOR color, const float angle, const GS_ENTITY_ORIGIN origin)
{
	// TODO
	return false;
}

bool GLES2Video::DrawRectangle(const Vector2 &v2Pos, const Vector2 &v2Size,
		const GS_COLOR color0, const GS_COLOR color1,
		const GS_COLOR color2, const GS_COLOR color3,
		const float angle, const GS_ENTITY_ORIGIN origin)
{
	// TODO
	return false;
}

void GLES2Video::SetBGColor(const GS_COLOR backgroundColor)
{
	m_backgroundColor = backgroundColor;
}

GS_COLOR GLES2Video::GetBGColor() const
{
	return m_backgroundColor;
}


bool GLES2Video::BeginSpriteScene(const GS_COLOR dwBGColor)
{
	if (dwBGColor != GS_ZERO)
	{
		m_backgroundColor = dwBGColor;
	}
	Vector4 color;
	color.SetColor(m_backgroundColor);
	glClearColor(color.x, color.y, color.z, color.w);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	SetAlphaMode(GSAM_PIXEL);
	m_rendering = true;
	return true;
}

bool GLES2Video::EndSpriteScene()
{
	m_rendering = false;
	ComputeFPSRate();
	return true;
}

bool GLES2Video::BeginTargetScene(const GS_COLOR dwBGColor, const bool clear)
{
	// explicit static cast for better performance
	TexturePtr texturePtr = m_currentTarget.lock();
	if (texturePtr)
	{
		Texture *pTexture = texturePtr.get(); // safety compile-time error checking
		GLES2Texture *pGLES2Texture = static_cast<GLES2Texture*>(pTexture); // safer direct cast
		const GLuint target = pGLES2Texture->GetFrameBufferID();
		glBindFramebuffer(GL_FRAMEBUFFER, target);

		CheckFrameBufferStatus(m_logger, target, pGLES2Texture->GetTextureID(), false);

		if (clear)
		{
			Vector4 color;
			color.SetColor(dwBGColor);
			glClearColor(color.x, color.y, color.z, color.w);
			glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		}

		const Texture::PROFILE& profile = pGLES2Texture->GetProfile();
		Enable2D(static_cast<int>(profile.width), static_cast<int>(profile.height), true);
		m_shaderContext->ResetViewConstants(m_orthoMatrix, GetCurrentTargetSize());
	}
	else
	{
		Message(GS_L("There's no render target"), GSMT_ERROR);
	}
	m_rendering = true;
	return true;
}

bool GLES2Video::EndTargetScene()
{
	SetRenderTarget(SpritePtr());
	m_rendering = false;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	Enable2D(m_screenSize.x, m_screenSize.y);
	m_shaderContext->ResetViewConstants(m_orthoMatrix, GetScreenSizeF());
	return true;
}

bool GLES2Video::SetAlphaMode(const GS_ALPHA_MODE mode)
{
	m_alphaMode = mode;
	switch(mode)
	{
	case GSAM_PIXEL:
	case GSAM_ALPHA_TEST: // alpha test not supported
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		break;
	case GSAM_ADD:
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);
		break;
	case GSAM_MODULATE:
		glEnable(GL_BLEND);
		glBlendFunc(GL_ZERO, GL_SRC_COLOR);
		break;
	case GSAM_NONE:
	default:
		glDisable(GL_BLEND);
		break;
	};
	return true;
}

GS_ALPHA_MODE GLES2Video::GetAlphaMode() const
{
	return m_alphaMode;
}

bool GLES2Video::SetFilterMode(const GS_TEXTUREFILTER_MODE tfm)
{
	// NOTE: it won't work on OpenGL ES exactly like in the D3D9 implementation since
	// the texture has to be previously bound before we reset filter mode
	m_textureFilterMode = tfm;
	switch (tfm)
	{
	case GSTM_IFNEEDED:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		break;
	case GSTM_ALWAYS:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		break;
	case GSTM_NEVER:
	default:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		break;
	};
	return true;
}

GS_TEXTUREFILTER_MODE GLES2Video::GetFilterMode() const
{
	return m_textureFilterMode;
}

bool GLES2Video::Rendering() const
{
	return m_rendering;
}

bool GLES2Video::SaveScreenshot(const wchar_t *wcsName,
		const GS_BITMAP_FORMAT fmt, Rect2D rect)
{
	// TODO
	return false;
}

bool GLES2Video::ManageLoop()
{
	if (Rendering())
		EndSpriteScene();

	APP_STATUS status = APP_SKIP;
	while (status == APP_SKIP)
	{
		status = HandleEvents();
		if (status == APP_QUIT)
			return false;
	}

	if (!Rendering())
		BeginSpriteScene();

	return true;
}

Vector2i GLES2Video::GetClientScreenSize() const
{
	return m_screenSize;
}

Application::APP_STATUS GLES2Video::HandleEvents()
{
	if (m_quit)
	{
		Command(Platform::NativeCommandAssembler::QuitApplication());
	}
	return m_quit ? APP_QUIT : APP_OK;
}

float GLES2Video::GetFPSRate() const
{
	return Max(1.0f, m_fpsRate);
}

void GLES2Video::ComputeFPSRate()
{
	static float counter = 0.0f;
	const clock_t current = clock()/1000;
	static clock_t last = current;

	const clock_t elapsed = current-last;
	if (elapsed > 500)
	{
		m_fpsRate = (counter*2);
		counter = 0.0f;
		last = current;
	}
	else
	{
		counter++;
	}
}

void GLES2Video::Message(const str_type::string& text, const GS_MESSAGE_TYPE type) const
{
	str_type::string str;
	Platform::FileLogger::TYPE logType = Platform::FileLogger::ERROR;
	switch (type)
	{
	case GSMT_WARNING:
		str = "WARNING: ";
		logType = Platform::FileLogger::WARNING;
		break;
	case GSMT_INFO:
		str = "Info: ";
		logType = Platform::FileLogger::INFO;
		break;
	case GSMT_ERROR:
		str = "ERROR: ";
		logType = Platform::FileLogger::ERROR;
		break;
	}
	m_logger.Log(str + text, logType);
}

unsigned long GLES2Video::GetElapsedTime(const TIME_UNITY unity) const
{
	unsigned long elapsedTime = static_cast<unsigned long>((clock()/1000)-m_startTimeMS);
	switch (unity)
	{
	case TU_HOURS:
		elapsedTime /= 1000;
		elapsedTime /= 60;
		elapsedTime /= 60;
		break;
	case TU_MINUTES:
		elapsedTime /= 1000;
		elapsedTime /= 60;
		break;
	case TU_SECONDS:
		elapsedTime /= 1000;
		break;
	case TU_MILLISECONDS:
	default:
		break;
	};
	return elapsedTime;
}

float GLES2Video::GetElapsedTimeF(const TIME_UNITY unity) const
{
	const unsigned long delta = (clock()/1000)-m_startTimeMS;
	float elapsedTime = static_cast<float>(delta);
	switch (unity)
	{
	case TU_HOURS:
		elapsedTime /= 1000.0f;
		elapsedTime /= 60.0f;
		elapsedTime /= 60.0f;
		break;
	case TU_MINUTES:
		elapsedTime /= 1000.0f;
		elapsedTime /= 60.0f;
		break;
	case TU_SECONDS:
		elapsedTime /= 1000.0f;
		break;
	case TU_MILLISECONDS:
	default:
		break;
	};
	return elapsedTime;
}

void GLES2Video::ResetTimer()
{
	m_startTimeMS = clock()/1000;
}

void GLES2Video::Quit()
{
	m_quit = true;
	Command(Platform::NativeCommandAssembler::QuitApplication());
}

void GLES2Video::EnableQuitShortcuts(const bool enable)
{
	// TODO
}

bool GLES2Video::QuitShortcutsEnabled()
{
	// TODO
	return false;
}

bool GLES2Video::SetWindowTitle(const str_type::string& title)
{
	m_windowTitle = title;
	return true;
}

str_type::string GLES2Video::GetWindowTitle() const
{
	return m_windowTitle;
}

void GLES2Video::EnableMediaPlaying(const bool enable)
{
	// TODO
}

Vector2 GLES2Video::GetCurrentTargetSize() const
{
	TexturePtr currentTarget = m_currentTarget.lock();
	return (currentTarget) ? currentTarget->GetBitmapSize() : GetScreenSizeF();
}

bool GLES2Video::IsWindowed() const
{
	return false;
}

Vector2i GLES2Video::GetScreenSize() const
{
	return m_screenSize;
}

Vector2 GLES2Video::GetScreenSizeF() const
{
	return Vector2(static_cast<float>(m_screenSize.x), static_cast<float>(m_screenSize.y));
}

Vector2i GLES2Video::GetWindowPosition()
{
	return Vector2i(0, 0);
}

void GLES2Video::SetWindowPosition(const Vector2i &v2)
{
	// dummy
}

Vector2i GLES2Video::ScreenToWindow(const Vector2i &v2Point) const
{
	return v2Point;
}

bool GLES2Video::WindowVisible() const
{
	return true;
}

bool GLES2Video::WindowInFocus() const
{
	return true;
}

bool GLES2Video::HideCursor(const bool hide)
{
	return true;
}

bool GLES2Video::IsCursorHidden() const
{
	return true;
}

const Platform::FileLogger& GLES2Video::GetLogger() const
{
	return m_logger;
}

Platform::FileManagerPtr GLES2Video::GetFileManager() const
{
	return m_fileManager;
}

} // namespace gs2d