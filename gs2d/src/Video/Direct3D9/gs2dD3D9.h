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

#ifndef GS2D_DIRECT3D9_H_
#define GS2D_DIRECT3D9_H_

#include "../../gs2d.h"
#include "gs2dD3D9CgShader.h"
#include <vector>
#include <map>

namespace gs2d {

/// Converts a GS_BITMAP_FORMAT constant to an extension string
std::wstring GetImageExtension(const GS_BITMAP_FORMAT fmt);

/// Returns true if the file name has an explicit extension in its string
bool IsTheExtensionRight(const wchar_t *fileName, const wchar_t *extension);

/// Returns a D3DXIMAGE constant for a GS_BITMAP_FORMAT one
D3DXIMAGE_FILEFORMAT GetD3DPF(const GS_BITMAP_FORMAT fmt);

class D3D9VideoInfo
{
	void BuildIn()
	{
		ZeroMemory(&m_msg, sizeof(MSG));
		className.clear();
		m_pow2Tex = m_scissorSupported = 0x00;
		m_inFocus = m_visible = true;
		m_deviceLost = false;
		m_startTime = 0x0;
		m_quitKeysEnabled = true;
		m_pRect2 = m_pRect4 = m_pRect3 = NULL;
		m_lastVertexProfile = m_lastPixelProfile = GSSP_NONE;
		m_pBackBuffer = NULL;
		m_nMaxRTs = 1;
		m_nMaxMultiTex = 0;
		v2Resize = v2RestoredSize = math::Vector2i(0,0);
		m_maximized = false;
	}

public:
	static const DWORD SPRITE_FVF;

	struct LINE_VERTEX
	{
		math::Vector3 pos;
		float rhw;
		GS_DWORD color;
	};

	struct VERTEX
	{
		math::Vector3 pos;
		float rhw;
		GS_DWORD color;
		math::Vector2 t0;
	};

	struct SPRITE_VERTEX
	{
		math::Vector3 pos;
		math::Vector2 t0;
	};

	D3D9VideoInfo()
	{
		BuildIn();
	}

	~D3D9VideoInfo()
	{
		ReleaseVB();
	}

	void ReleaseVB()
	{
		if (m_pRect2)
		{
			m_pRect2->Release();
			m_pRect2 = NULL;
		}
		if (m_pRect4)
		{
			m_pRect4->Release();
			m_pRect4 = NULL;
		}
		if (m_pRect3)
		{
			m_pRect3->Release();
			m_pRect3 = NULL;
		}
	}

	bool CreateVB(IDirect3DDevice9 *pDevice)
	{
		ReleaseVB();
		// Create the vertex buffer for the 2-triangled rect
		const SPRITE_VERTEX vertices2[] =
		{
			{  math::Vector3(0.0f, 1.0f, 0.0f), math::Vector2(0, 1) },
			{  math::Vector3(0.0f, 0.0f, 0.0f), math::Vector2(0, 0) },
			{  math::Vector3(1.0f, 0.0f, 0.0f), math::Vector2(1, 0) },
			{  math::Vector3(1.0f, 1.0f, 0.0f), math::Vector2(1, 1) },
		};

		if (FAILED(pDevice->CreateVertexBuffer(sizeof(vertices2),
											 D3DUSAGE_WRITEONLY, SPRITE_FVF,
											 D3DPOOL_MANAGED, &m_pRect2, NULL)))
		{
			return false;
		}

		void* pVertices;
		if (FAILED(m_pRect2->Lock(0, 0, &pVertices, 0)))
		{
			return false;
		}
		memcpy(pVertices, vertices2, sizeof(vertices2));
		m_pRect2->Unlock();

		// Create the vertex buffer for the 4-triangled rect
		const SPRITE_VERTEX vertices4[] =
		{
			{  math::Vector3(0.5f, 0.5f, 0.0f), math::Vector2(0.5f, 0.5f) },
			{  math::Vector3(0.0f, 1.0f, 0.0f), math::Vector2(0, 1) },
			{  math::Vector3(0.0f, 0.0f, 0.0f), math::Vector2(0, 0) },
			{  math::Vector3(1.0f, 0.0f, 0.0f), math::Vector2(1, 0) },
			{  math::Vector3(1.0f, 1.0f, 0.0f), math::Vector2(1, 1) },
			{  math::Vector3(0.0f, 1.0f, 0.0f), math::Vector2(0, 1) },
		};
		if (FAILED(pDevice->CreateVertexBuffer(sizeof(vertices4),
											 D3DUSAGE_WRITEONLY, SPRITE_FVF,
											 D3DPOOL_MANAGED, &m_pRect4, NULL)))
		{
			return false;
		}
		if (FAILED(m_pRect4->Lock(0, 0, &pVertices, 0)))
		{
			return false;
		}
		memcpy(pVertices, vertices4, sizeof(vertices4));
		m_pRect4->Unlock();

		// Create the vertex buffer for the 3-triangled rect
		const SPRITE_VERTEX vertices3[] =
		{
			{  math::Vector3(0.0f, 0.0f, 0.0f), math::Vector2(0, 0) },
			{  math::Vector3(0.0f, 1.0f, 0.0f), math::Vector2(0, 1) },
			{  math::Vector3(0.5f, 0.0f, 0.0f), math::Vector2(0.5f, 0) },
			{  math::Vector3(1.0f, 1.0f, 0.0f), math::Vector2(1, 1) },
			{  math::Vector3(1.0f, 0.0f, 0.0f), math::Vector2(1, 0) },
		};
		if (FAILED(pDevice->CreateVertexBuffer(sizeof(vertices3),
											 D3DUSAGE_WRITEONLY, SPRITE_FVF,
											 D3DPOOL_MANAGED, &m_pRect3, NULL)))
		{
			return false;
		}
		if (FAILED(m_pRect3->Lock(0, 0, &pVertices, 0)))
		{
			return false;
		}
		memcpy(pVertices, vertices3, sizeof(vertices3));
		m_pRect3->Unlock();

		return true;
	}

	bool DrawSprite(IDirect3DDevice9 *pDevice, const GS_RECT_MODE mode)
	{
		IDirect3DVertexBuffer9 *pSprite = (mode == GSRM_TWO_TRIANGLES) ? m_pRect2 : ((mode == GSRM_FOUR_TRIANGLES) ? m_pRect4 : m_pRect3);
		const unsigned int polyCount = (mode == GSRM_TWO_TRIANGLES) ? 2 : ((mode == GSRM_FOUR_TRIANGLES) ? 4 : 3);
		const D3DPRIMITIVETYPE primType = (mode == GSRM_TWO_TRIANGLES) ? D3DPT_TRIANGLEFAN : ((mode == GSRM_FOUR_TRIANGLES) ? D3DPT_TRIANGLEFAN : D3DPT_TRIANGLESTRIP);

		if (FAILED(pDevice->SetStreamSource(0, pSprite, 0, sizeof(SPRITE_VERTEX))))
			return false;
		pDevice->SetFVF(SPRITE_FVF);
		if (FAILED(pDevice->DrawPrimitive(primType, 0, polyCount)))
			return false;
		return true;
	}

	void BeginFastDraw(IDirect3DDevice9 *pDevice, const GS_RECT_MODE mode)
	{
		IDirect3DVertexBuffer9 *pSprite = (mode == GSRM_TWO_TRIANGLES) ? m_pRect2 : ((mode == GSRM_FOUR_TRIANGLES) ? m_pRect4 : m_pRect3);
		if (FAILED(pDevice->SetStreamSource(0, pSprite, 0, sizeof(SPRITE_VERTEX))))
			return;
		pDevice->SetFVF(SPRITE_FVF);
	}

	bool DrawSpriteFast(IDirect3DDevice9 *pDevice, const GS_RECT_MODE mode)
	{
		const unsigned int polyCount = (mode == GSRM_TWO_TRIANGLES) ? 2 : ((mode == GSRM_FOUR_TRIANGLES) ? 4 : 3);
		const D3DPRIMITIVETYPE primType = (mode == GSRM_TWO_TRIANGLES) ? D3DPT_TRIANGLEFAN : ((mode == GSRM_FOUR_TRIANGLES) ? D3DPT_TRIANGLEFAN : D3DPT_TRIANGLESTRIP);
		if (FAILED(pDevice->DrawPrimitive(primType, 0, polyCount)))
			return false;
		return true;
	}

	void SetupShaderProfile()
	{
		if (m_caps.VertexShaderVersion >= D3DVS_VERSION(1,1))
			m_lastVertexProfile = GSSP_MODEL_1;
		if (m_caps.VertexShaderVersion >= D3DVS_VERSION(2,0))
			m_lastVertexProfile = GSSP_MODEL_2;
		if (m_caps.VertexShaderVersion >= D3DVS_VERSION(3,0))
			m_lastVertexProfile = GSSP_MODEL_3;

		if (m_caps.PixelShaderVersion >= D3DPS_VERSION(1,0))
			m_lastPixelProfile = GSSP_MODEL_1;
		if (m_caps.PixelShaderVersion >= D3DPS_VERSION(2,0))
			m_lastPixelProfile = GSSP_MODEL_2;
		if (m_caps.PixelShaderVersion >= D3DPS_VERSION(3,0))
			m_lastPixelProfile = GSSP_MODEL_3;
	}

	D3DCAPS9	m_caps;
	HWND		m_hWnd;
	HINSTANCE	m_hInstance;
	MSG			m_msg;
	HCURSOR   m_cursor;
	std::wstring className;
	DWORD m_pow2Tex;
	DWORD m_magAniso;
	DWORD m_minAniso;
	DWORD m_maxAniso;
	DWORD m_scissorSupported;
	unsigned int m_nMaxRTs;
	unsigned int m_nMaxMultiTex;
	bool m_isVSSupported;
	DWORD	m_windowStyle;
	D3DPRESENT_PARAMETERS m_d3dPP;
	bool m_visible;
	bool m_inFocus;
	bool m_deviceLost;
	bool m_quitKeysEnabled;
	bool m_maximized;
	DWORD m_startTime;
	IDirect3DVertexBuffer9 *m_pRect2;
	IDirect3DVertexBuffer9 *m_pRect4;
	IDirect3DVertexBuffer9 *m_pRect3;
	math::Matrix4x4 m_orthoMatrix;
	GS_SHADER_PROFILE m_lastVertexProfile;
	GS_SHADER_PROFILE m_lastPixelProfile;
	IDirect3DSurface9 *m_pBackBuffer;
	math::Vector2i v2Resize, v2RestoredSize;
};

typedef boost::shared_ptr<D3D9VideoInfo> D3D9VideoInfoPtr;

class D3D9Video : public Video
{
	friend class D3D9Sprite;
	friend class WinInput;
	/*friend VideoPtr CreateVideo(const unsigned int, const unsigned int, const std::wstring&,
		const bool, const bool, const std::wstring&, const GS_PIXEL_FORMAT, const bool,
		Platform::FileManagerPtr);*/

	D3D9Video(const unsigned int width, const unsigned int height, const std::wstring& winTitle,
			const bool windowed, const bool sync, const std::wstring& bitmapFontDefaultPath,
			const GS_PIXEL_FORMAT pfBB, const bool maximizable);

	struct RENDER_TARGET
	{
		RENDER_TARGET()
		{
			width = height = 0;
			format = GSTF_DEFAULT;
		}
		RENDER_TARGET(Sprite *sprite, const unsigned int width, const unsigned int height, const GS_TARGET_FORMAT fmt)
		{
			pSprite = sprite;
			this->width = width;
			this->height = height;
			format = fmt;
		}
		Sprite *pSprite;
		unsigned int width;
		unsigned int height;
		GS_TARGET_FORMAT format;
	};

	typedef std::list<RENDER_TARGET> RENDER_TARGET_LIST;

	math::Vector2i m_screenDim;
	math::Vector2i m_windowPos;
	GS_COLOR m_backgroundColor;
	float m_fpsRate;
	bool m_quit;
	std::vector<GS_BLEND_MODE> m_blendModes;
	IDirect3DDevice9 *m_pDevice;
	IDirect3D9 *m_pD3D;
	math::Vector2i m_windowedDim;
	math::Vector2i m_topBarSize;

	D3D9VideoInfoPtr m_videoInfo;
	GS_ALPHA_MODE m_alphaMode;

	bool m_rendering;
	bool m_windowed;
	bool m_sync;
	bool m_cursorHidden;
	bool m_clamp;
	bool m_maximizable;
	math::Rect2D m_scissor;
	math::Vector2 m_v2Camera;
	std::wstring m_windowTitle;
	float m_lineWidth;
	float m_depth;
	bool m_roundUpPosition;
	std::map<std::wstring, BitmapFontPtr> m_fonts;
	std::wstring m_defaultBitmapFontPath;

	std::vector<VIDEO_MODE> m_modes;
	unsigned int m_nVideoModes;
	GS_TEXTUREFILTER_MODE m_textureFilter;
	D3D9CgShaderContextPtr m_shaderContext;
	ShaderPtr m_pCurrentPS,
			  m_pCurrentVS,
			  m_defaultVS,
			  m_rectVS,
			  m_fastVS;

	void ComputeFPSRate();
	void SetDisplayModes(IDirect3D9 *pD3D);
	static LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	static void RemoveFromTargetList(Sprite *pSprite);
	static void AddToTargetList(const RENDER_TARGET &target);

	boost::weak_ptr<D3D9Video> weak_this;

public:
	~D3D9Video();

	static const DWORD W32_WINDOWED_STYLE;
	static const DWORD W32_FULLSCREEN_STYLE;
	static const float ZNEAR;
	static const float ZFAR;
	static const DWORD ALPHAREF;
	static const unsigned int TEXTURE_CHANNELS;

	static boost::shared_ptr<D3D9Video> Create(const unsigned int width, const unsigned int height,
			const std::wstring& winTitle, const bool windowed, const bool sync,
			const std::wstring& bitmapFontDefaultPath, const GS_PIXEL_FORMAT pfBB, const bool maximizable);

	/// Instantiate a texture object and load the texture from a file in memory (supported formats: .dds, .bmp, .png, .jpg/jpeg).
	TexturePtr CreateTextureFromFileInMemory(const void *pBuffer, const unsigned int bufferLength, GS_COLOR mask,
				 const unsigned int width = 0, const unsigned int height = 0,
				 const unsigned int nMipMaps = 0);

	/// Instantiate a texture object and load the texture from a file in a hard disk (supported formats: .dds, .bmp, .png, .jpg/jpeg).
	TexturePtr LoadTextureFromFile(const std::wstring& fileName, GS_COLOR mask,
				 const unsigned int width = 0, const unsigned int height = 0,
				 const unsigned int nMipMaps = 0);

	/// Instantiate a texture object and create a surface as render target.
	TexturePtr CreateRenderTargetTexture(const unsigned int width, const unsigned int height, const GS_TARGET_FORMAT = GSTF_DEFAULT);

	/// Creates a sprite from a texture in a file
	SpritePtr CreateSprite(GS_BYTE *pBuffer, const unsigned int bufferLength,
						GS_COLOR mask = GS_ZERO, const unsigned int width = 0, const unsigned int height = 0);

	/// Creates a sprite from a texture in virtual memory
	SpritePtr CreateSprite(const std::wstring& fileName,
					GS_COLOR mask = GS_ZERO, const unsigned int width = 0, const unsigned int height = 0);

	/// Creates a sprite as render target
	SpritePtr CreateRenderTarget(const unsigned int width, const unsigned int height,
							const GS_TARGET_FORMAT format = GSTF_DEFAULT);

	/// Create a shader object and load/compile it.
	ShaderPtr LoadShaderFromFile(const std::wstring& fileName, const GS_SHADER_FOCUS focus,
				const GS_SHADER_PROFILE profile = GSSP_HIGHEST, const char *entry = 0);

	/// Create a shader object and load/compile it.
	ShaderPtr LoadShaderFromString(const std::wstring& shaderName, const std::string& codeAsciiString, const GS_SHADER_FOCUS focus,
				const GS_SHADER_PROFILE profile = GSSP_HIGHEST, const char *entry = 0);

	ShaderPtr GetFontShader();
	ShaderPtr GetOptimalVS();
	ShaderPtr GetDefaultVS();
	ShaderPtr GetVertexShader();
	ShaderPtr GetPixelShader();
	ShaderContextPtr GetShaderContext();
	bool SetVertexShader(ShaderPtr pShader);
	bool SetPixelShader(ShaderPtr pShader);
	GS_SHADER_PROFILE GetHighestVertexProfile() const;
	GS_SHADER_PROFILE GetHighestPixelProfile() const;

	boost::any GetVideoInfo();

	VIDEO_MODE GetVideoMode(const unsigned int modeIdx) const;
	unsigned int GetVideoModeCount();
	bool ResetVideoMode(const VIDEO_MODE mode, const bool toggleFullscreen = false);
	bool ResetVideoMode(const unsigned int width, const unsigned int height, const GS_PIXEL_FORMAT pfBB, const bool toggleFullscreen = false);

	bool SetRenderTarget(SpritePtr pTarget, const unsigned int target = 0);
	unsigned int GetMaxRenderTargets() const;
	unsigned int GetMaxMultiTextures() const;
	bool SetBlendMode(const unsigned int passIdx, const GS_BLEND_MODE mode);
	GS_BLEND_MODE GetBlendMode(const unsigned int passIdx) const;
	bool UnsetTexture(const unsigned int passIdx);

	void SetZBuffer(const bool enable);
	bool GetZBuffer() const;

	void SetZWrite(const bool enable);
	bool GetZWrite() const;

	bool SetClamp(const bool set);
	bool GetClamp() const;

	bool SetSpriteDepth(const float depth);
	float GetSpriteDepth() const;

	void SetLineWidth(const float width);
	float GetLineWidth() const;

	bool SetCameraPos(const math::Vector2 &pos);
	bool MoveCamera(const math::Vector2 &dir);
	math::Vector2 GetCameraPos() const;

	void RoundUpPosition(const bool roundUp);
	bool IsRoundingUpPosition() const;

	bool SetScissor(const math::Rect2D &rect);
	math::Rect2D GetScissor() const;
	void UnsetScissor();

	void SetBitmapFontDefaultPath(const std::wstring& path);
	std::wstring GetBitmapFontDefaultPath() const;

	math::Vector2 ComputeCarretPosition(const std::wstring& font, const std::wstring& text, const unsigned int pos);
	math::Vector2 ComputeTextBoxSize(const std::wstring& font, const std::wstring& text);
	unsigned int FindClosestCarretPosition(const std::wstring& font, const std::wstring &text, const math::Vector2 &textPos, const math::Vector2 &reference);
	bool DrawBitmapText(const math::Vector2 &v2Pos, const std::wstring& text,
		const std::wstring& font, const GS_COLOR color);

	bool DrawLine(const math::Vector2 &p1, const math::Vector2 &p2, const GS_COLOR color1, const GS_COLOR color2);
	bool DrawRectangle(const math::Vector2 &v2Pos, const math::Vector2 &v2Size,
						const GS_COLOR color, const float angle = 0.0f, const GS_ENTITY_ORIGIN origin = GSEO_DEFAULT);
	bool DrawRectangle(const math::Vector2 &v2Pos, const math::Vector2 &v2Size,
						const GS_COLOR color0, const GS_COLOR color1, const GS_COLOR color2, const GS_COLOR color3,
						const float angle = 0.0f, const GS_ENTITY_ORIGIN origin = GSEO_DEFAULT);

	void SetBGColor(const GS_COLOR backgroundColor);
	GS_COLOR GetBGColor() const;

	bool BeginSpriteScene(const GS_COLOR bgColor = GS_ZERO);
	bool EndSpriteScene();
	bool BeginTargetScene(const GS_COLOR bgColor = GS_ZERO, const bool clear = true);
	bool EndTargetScene();

	bool SetAlphaMode(const GS_ALPHA_MODE mode);
	GS_ALPHA_MODE GetAlphaMode() const;

	bool SetFilterMode(const GS_TEXTUREFILTER_MODE tfm);
	GS_TEXTUREFILTER_MODE GetFilterMode() const;

	bool Rendering() const;

	bool SaveScreenshot(const wchar_t *wcsName, const GS_BITMAP_FORMAT fmt = GSBF_BMP,
						math::Rect2D rect = math::Rect2D(0,0,0,0));

	boost::any GetGraphicContext();

	// Application method implementations:
	bool ManageLoop();
	math::Vector2i GetClientScreenSize() const;
	APP_STATUS HandleEvents();
	float GetFPSRate() const;
	void Message(const std::wstring& text, const GS_MESSAGE_TYPE type = GSMT_ERROR) const;
	unsigned long GetElapsedTime(const TIME_UNITY unity = TU_MILLISECONDS) const;
	float GetElapsedTimeF(const TIME_UNITY unity = TU_MILLISECONDS) const;
	void ResetTimer();
	Platform::FileManagerPtr GetFileManager() const;

	// Window method implementations:
	void EnableQuitShortcuts(const bool enable);
	bool QuitShortcutsEnabled();
	bool SetWindowTitle(const std::wstring& wcsTitle);
	std::wstring GetWindowTitle() const;
	void EnableMediaPlaying(const bool enable);
	bool IsWindowed() const;
	math::Vector2i GetScreenSize() const;
	math::Vector2 GetScreenSizeF() const;
	math::Vector2i GetWindowPosition();
	void SetWindowPosition(const math::Vector2i &v2);
	math::Vector2i ScreenToWindow(const math::Vector2i &v2Point) const;
	bool WindowVisible() const;
	bool WindowInFocus() const;
	bool HideCursor(const bool hide);
	bool IsCursorHidden() const;
	void Quit();

private:

	bool StartApplication(const unsigned int width, const unsigned int height,
			const std::wstring& winTitle,
			const bool windowed,
			const bool sync,
			const std::wstring& bitmapFontDefaultPath,
			const GS_PIXEL_FORMAT pfBB,
			const bool maximizable);

	bool BeginScene(const GS_COLOR bgColor = GS_ZERO, const bool clear = true);
	bool EndScene(const bool swap = true);

	BitmapFontPtr LoadBitmapFont(const std::wstring& fullFilePath);

	static RENDER_TARGET_LIST m_targets;

	// these members must be static because it's data will be exchanged with the Input object
	static SHORT m_wheelDelta;
	static bool m_inputFocus;
	static TCHAR m_currentChar;
};

} // namespace gs2d

#endif