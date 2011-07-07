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

#ifndef GS2D_GLES2_SHADER_H_
#define GS2D_GLES2_SHADER_H_

#include "../../gs2dshader.h"
#include "../../Platform/Platform.h"
#include "../../Platform/FileLogger.h"
#include "../../Platform/android/Platform.android.h"
#include "GLES2RectRenderer.h"
#include "gs2dGLES2Texture.h"
#include "GLES2UniformParameter.h"

#include <map>

namespace gs2d {

class GLES2Shader;
typedef boost::shared_ptr<GLES2Shader> GLES2ShaderPtr;

inline std::size_t fastHash(const str_type::string &str)
{
	static const std::size_t seed = 0;
	std::size_t hash = seed;
	const char* s = str.c_str();
	while (*s)
	{
		hash = hash * 101  +  *s++;
	}
	return hash;
	/*static const std::size_t InitialFNV = 2166136261U;
	static const std::size_t FNVMultiple = 16777619;
	std::size_t hash = InitialFNV;
	const std::size_t len = str.length();
	for(std::size_t i = 0; i < len; i++)
	{
		hash = hash ^ (str[i]);
		hash = hash * FNVMultiple;
	}
	return hash;*/
}

class GLES2ShaderContext : public ShaderContext
{
public:
	GLES2ShaderContext(GLES2Video *pVideo);
	~GLES2ShaderContext();

	boost::any GetContextPointer();
	bool DisableTextureParams();

	void SetShader(GLES2ShaderPtr vs, GLES2ShaderPtr ps, const math::Matrix4x4 &ortho, const math::Vector2& screenSize);
	void DrawRect() const;

	void BeginFastDraw();
	void FastDraw();
	void EndFastDraw();

	void Log(const str_type::string& str, const Platform::FileLogger::TYPE& type) const;

	void ResetViewConstants(const math::Matrix4x4 &ortho, const math::Vector2& screenSize);

	GLES2ShaderPtr GetCurrentVS();
	GLES2ShaderPtr GetCurrentPS();

	bool SetSpriteDepth(const float depth);
	float GetSpriteDepth() const;
	
	static const int INVALID_ATTRIB_LOCATION;

protected:
	GLES2ShaderPtr m_currentVS;
	GLES2ShaderPtr m_currentPS;
	std::map<std::size_t, GLuint> m_programs;
	std::map<GLuint, int> m_vertexPosLocations;
	std::map<GLuint, int> m_texCoordLocations;
	bool CheckForError(const str_type::string& situation);
	GLES2RectRenderer m_rectRenderer;

private:
	mutable GLuint m_lastProgram;
	mutable int m_lastPosLocation;
	mutable int m_lastTexLocation;
	void GetLocations(int& pos, int& tex) const;

	math::Matrix4x4 m_ortho;
	math::Vector2 m_screenSize;

	GLuint m_currentProgram;
	Platform::FileLogger m_logger;

	float m_spriteDepthValue;
	
	void SetShader(GLES2ShaderPtr shader);
	void SetUniformParametersFromCurrentProgram(GLES2ShaderPtr shader) const;
	GLuint CreateProgramFromCurrentShaders();
	size_t ComputeProgramHashValue(GLES2ShaderPtr vertex, GLES2ShaderPtr pixel) const;
	bool FindLocations(const GLuint program);
};

typedef boost::shared_ptr<GLES2ShaderContext> GLES2ShaderContextPtr;

class GLES2Shader : public Shader
{
private:
	GLuint m_shader;
	boost::shared_ptr<Platform::FileLogger> m_logger;
	Platform::FileManagerPtr m_zip;

	GLES2ShaderContextPtr m_context;
	str_type::string m_shaderName;
	GS_SHADER_FOCUS m_shaderFocus;
	std::map<std::size_t, GLES2UniformParameterPtr> m_parameters;
	const str_type::string DIFFUSE_TEXTURE_NAME;
	
	std::map<str_type::string, GLint> m_texturePasses;
	GLint m_texturePassCounter;

public:

	const std::map<std::size_t, GLES2UniformParameterPtr>& GetParameters() const;
	GLuint GetShader();
	str_type::string GetShaderName();
	
	GLES2Shader(Platform::FileManagerPtr fileManager, GLES2ShaderContextPtr context);
	~GLES2Shader();

	bool LoadShaderFromFile(ShaderContextPtr context, const str_type::string& fileName, const GS_SHADER_FOCUS focus,
		const GS_SHADER_PROFILE profile = GSSP_HIGHEST,	const char *entry = 0);

	bool LoadShaderFromString(ShaderContextPtr context, const str_type::string& shaderName,
			const std::string& codeAsciiString, const GS_SHADER_FOCUS focus,
			const GS_SHADER_PROFILE profile = GSSP_HIGHEST,	const char *entry = 0);

	bool ConstantExist(const str_type::string& name);

	bool SetConstant(const std::size_t nameHash, const str_type::string& name, const math::Vector4 &v);
	bool SetConstant(const std::size_t nameHash, const str_type::string& name, const math::Vector3 &v);
	bool SetConstant(const std::size_t nameHash, const str_type::string& name, const math::Vector2 &v);
	bool SetConstant(const std::size_t nameHash, const str_type::string& name, const float x);
	bool SetMatrixConstant(const std::size_t nameHash, const str_type::string& name, const math::Matrix4x4 &matrix);
	bool SetConstant(const std::size_t nameHash, const str_type::string& name, const GS_COLOR dw);
	bool SetConstantArray(const std::size_t nameHash, const str_type::string& name, unsigned int nElements,
						  const boost::shared_array<const math::Vector2>& v);

	bool SetConstant(const str_type::string& name, const GS_COLOR dw);
	bool SetConstant(const str_type::string& name, const math::Vector4 &v);
	bool SetConstant(const str_type::string& name, const math::Vector3 &v);
	bool SetConstant(const str_type::string& name, const math::Vector2 &v);
	bool SetConstant(const str_type::string& name, const float x, const float y, const float z, const float w);
	bool SetConstant(const str_type::string& name, const float x, const float y, const float z);
	bool SetConstant(const str_type::string& name, const float x, const float y);
	bool SetConstant(const str_type::string& name, const float x);
	bool SetConstant(const str_type::string& name, const int n);
	bool SetConstantArray(const str_type::string& name, unsigned int nElements, const boost::shared_array<const math::Vector2>& v);
	bool SetMatrixConstant(const str_type::string& name, const math::Matrix4x4 &matrix);
	bool SetTexture(const str_type::string& name, TextureWeakPtr pTexture);

	bool SetShader();
	GS_SHADER_FOCUS GetShaderFocus() const;
	GS_SHADER_PROFILE GetShaderProfile() const;
	void UnbindShader();
};

} // namespace gs2d

#endif