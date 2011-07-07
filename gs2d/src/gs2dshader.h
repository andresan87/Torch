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

#ifndef GS2D_SHADER_H_
#define GS2D_SHADER_H_

#include "gs2dmath.h"
#include <boost/shared_array.hpp>

namespace gs2d {

enum GS_SHADER_PROFILE
{
	GSSP_NONE =-1,
	GSSP_MODEL_1 = 0,
	GSSP_MODEL_2 = 1,
	GSSP_MODEL_3 = 2,
	GSSP_HIGHEST = 3,
};

enum GS_SHADER_FOCUS
{
	GSSF_PIXEL = 0,
	GSSF_VERTEX = 1,
	GSSF_NONE = 2
};

class Video;

/**
 * \brief Provides a render context to the shaders
 */
class ShaderContext
{
public:
	virtual boost::any GetContextPointer() = 0;
	virtual bool DisableTextureParams() = 0;

protected:
	virtual bool CheckForError(const str_type::string& situation) = 0;
};

typedef boost::shared_ptr<ShaderContext> ShaderContextPtr;

/**
 * \brief Abstracts a shader object
 *
 * Stores, manages and binds a fragment or vertex shader.
 */
class Shader
{
public:

	virtual bool LoadShaderFromFile(ShaderContextPtr context, const str_type::string& fileName, const GS_SHADER_FOCUS focus,
		const GS_SHADER_PROFILE profile = GSSP_HIGHEST,	const char *entry = 0) = 0;

	virtual bool LoadShaderFromString(ShaderContextPtr context, const str_type::string& shaderName, const std::string& codeAsciiString, const GS_SHADER_FOCUS focus,
		const GS_SHADER_PROFILE profile = GSSP_HIGHEST,	const char *entry = 0) = 0;

	virtual bool ConstantExist(const str_type::string& name) = 0;
	virtual bool SetConstant(const str_type::string& name, const GS_COLOR dw) = 0;
	virtual bool SetConstant(const str_type::string& name, const math::Vector4 &v) = 0;
	virtual bool SetConstant(const str_type::string& name, const math::Vector3 &v) = 0;
	virtual bool SetConstant(const str_type::string& name, const math::Vector2 &v) = 0;
	virtual bool SetConstant(const str_type::string& name, const float x, const float y, const float z, const float w) = 0;
	virtual bool SetConstant(const str_type::string& name, const float x, const float y, const float z) = 0;
	virtual bool SetConstant(const str_type::string& name, const float x, const float y) = 0;
	virtual bool SetConstant(const str_type::string& name, const float x) = 0;
	virtual bool SetConstant(const str_type::string& name, const int n) = 0;
	virtual bool SetConstantArray(const str_type::string& name, unsigned int nElements, const boost::shared_array<const math::Vector2>& v) = 0;
	virtual bool SetMatrixConstant(const str_type::string& name, const math::Matrix4x4 &matrix) = 0;
	virtual bool SetTexture(const str_type::string& name, TextureWeakPtr pTexture) = 0;

	virtual bool SetShader() = 0;
	virtual GS_SHADER_FOCUS GetShaderFocus() const = 0;
	virtual GS_SHADER_PROFILE GetShaderProfile() const = 0;
	virtual void UnbindShader() = 0;

};

typedef boost::shared_ptr<Shader> ShaderPtr;

} // namespace gs2d

#endif 