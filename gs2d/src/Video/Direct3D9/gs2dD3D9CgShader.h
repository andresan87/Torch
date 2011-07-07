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

#ifndef GS2D_CGD3D9_SHADER_H_
#define GS2D_CGD3D9_SHADER_H_

#include "../../gs2d.h"

//#define WIN32_LEAN_AND_MEAN
//#ifdef _DEBUG
//#define D3D_DEBUG_INFO
//#endif

#include <Cg/cg.h>
#include <Cg/cgD3D9.h>
#include <map>

namespace gs2d {

/**
 * \brief Implements an NVIDIA Cg based Direct3D 9 context
 */
class D3D9CgShaderContext : public ShaderContext
{
	friend class D3D9Video;
	CGcontext m_cgContext;
	D3D9CgShaderContext(IDirect3DDevice9 *pDevice);

public:
	~D3D9CgShaderContext();

	boost::any GetContextPointer();

	/// Dummy function (isn't necessary with D3D9)
	bool DisableTextureParams() { return true; }

protected:
	bool CheckForError(const std::wstring& situation);
};

typedef boost::shared_ptr<D3D9CgShaderContext> D3D9CgShaderContextPtr;

/**
 * \brief Implemtns an NVIDIA Cg based Direct3D 9 shader object
 */
class D3D9CgShader : public Shader
{
	CGprogram m_cgProgram;
	CGprofile m_cgLatestProfile;
	std::map<std::wstring, CGparameter> m_mParam;

	bool SetupParameters();
	GS_SHADER_FOCUS m_focus;
	GS_SHADER_PROFILE m_profile;
	std::wstring m_shaderName;
	ShaderContextPtr m_pShaderContext;

	bool CheckForError(const std::wstring &situation, const std::wstring &add);

public:
	D3D9CgShader();
	~D3D9CgShader();

	bool ConstantExist(const std::wstring& name);
	bool SetConstant(const std::wstring& name, const GS_COLOR dw);
	bool SetConstant(const std::wstring& name, const math::Vector4 &v);
	bool SetConstant(const std::wstring& name, const math::Vector3 &v);
	bool SetConstant(const std::wstring& name, const math::Vector2 &v);
	bool SetConstant(const std::wstring& name, const float x, const float y, const float z, const float w);
	bool SetConstant(const std::wstring& name, const float x, const float y, const float z);
	bool SetConstant(const std::wstring& name, const float x, const float y);
	bool SetConstant(const std::wstring& name, const float x);
	bool SetConstant(const std::wstring& name, const int n);
	bool SetConstantArray(const std::wstring& name, unsigned int nElements, const boost::shared_array<const math::Vector2>& v);
	bool SetMatrixConstant(const std::wstring& name, const math::Matrix4x4 &matrix);
	bool SetTexture(const std::wstring& name, TextureWeakPtr pTexture);

	bool CompileShader();
	bool SetShader();
	GS_SHADER_FOCUS GetShaderFocus() const;
	GS_SHADER_PROFILE GetShaderProfile() const;

	/// dummy since it's unecessary with D3D9
	void UnbindShader() {}

protected:

	bool LoadShaderFromFile(ShaderContextPtr context, const std::wstring& fileName, const GS_SHADER_FOCUS focus,
		const GS_SHADER_PROFILE profile = GSSP_HIGHEST, const char *entry = 0);

	bool LoadShaderFromString(ShaderContextPtr context, const std::wstring& shaderName, const std::string& codeAsciiString, const GS_SHADER_FOCUS focus,
		const GS_SHADER_PROFILE profile = GSSP_HIGHEST, const char *entry = 0);
};

typedef boost::shared_ptr<D3D9CgShader> D3D9CgShaderPtr;

} // namespace gs2d

#endif 