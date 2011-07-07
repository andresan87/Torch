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

#include "GLES2UniformParameter.h"

namespace gs2d {

int counter = 1;

GLES2UniformParameter::GLES2UniformParameter(const str_type::string& name) :
	m_locations(new LocationMap),
	m_name(name)
{
}

LocationMapPtr GLES2UniformParameter::GetLocations()
{
	return m_locations;
}

const str_type::string& GLES2UniformParameter::GetName() const
{
	return m_name;
}

void GLES2UniformParameter::SetLocations(const LocationMapPtr& locations)
{
	m_locations = locations;
}

inline int GLES2UniformParameter::GetLocation(const GLuint program, const str_type::string& name, const Platform::FileLogger& logger)
{
	LocationMap& locations = *m_locations.get();
	std::map<GLuint, int>::iterator iter = locations.find(program);
	if (iter != locations.end())
	{
		return iter->second;
	}
	else
	{
		const int location = glGetUniformLocation(program, name.c_str());
		GLES2Video::CheckGLError(name + ": uniform parameter not found with glGetUniformLocation", logger);
		str_type::stringstream ss;
		ss << "Location obtained successfully [" << name << "] " << counter++ << ": " << location;
		logger.Log(ss.str(), Platform::FileLogger::INFO);
		locations[program] = location;
		return location;
	}
}

GLES2UPVec1::GLES2UPVec1(const float v, const str_type::string& name) :
	GLES2UniformParameter(name)
{
	this->v = v;
}
bool GLES2UPVec1::SetParameter(const GLuint program, const str_type::string& name, const Platform::FileLogger& logger)
{
	const int location = GetLocation(program, name, logger);
	glUniform1f(location, v);
	//GLES2Video::CheckGLError(name + ": uniform parameter not found with glUniform1f", logger);
	return true;
}

GLES2UPVec2::GLES2UPVec2(const math::Vector2& v, const str_type::string& name) :
	GLES2UniformParameter(name)
{
	this->v = v;
}
bool GLES2UPVec2::SetParameter(const GLuint program, const str_type::string& name, const Platform::FileLogger& logger)
{
	const int location = GetLocation(program, name, logger);
	glUniform2fv(location, 1, &v.x);
	//GLES2Video::CheckGLError(name + ": uniform parameter not found with glUniform2fv", logger);
	return true;
}

GLES2UPVec3::GLES2UPVec3(const math::Vector3& v, const str_type::string& name) :
	GLES2UniformParameter(name)
{
	this->v = v;
}
bool GLES2UPVec3::SetParameter(const GLuint program, const str_type::string& name, const Platform::FileLogger& logger)
{
	const int location = GetLocation(program, name, logger);
	glUniform3fv(location, 1, &v.x);
	//GLES2Video::CheckGLError(name + ": uniform parameter not found with glUniform3fv", logger);
	return true;
}

GLES2UPVec4::GLES2UPVec4(const math::Vector4& v, const str_type::string& name) :
	GLES2UniformParameter(name)
{
	this->v = v;
}
bool GLES2UPVec4::SetParameter(const GLuint program, const str_type::string& name, const Platform::FileLogger& logger)
{
	const int location = GetLocation(program, name, logger);
	glUniform4fv(location, 1, &v.x);
	//GLES2Video::CheckGLError(name + ": uniform parameter not found with glUniform4fv", logger);
	return true;
}

GLES2UPMat4x4::GLES2UPMat4x4(const math::Matrix4x4& v, const str_type::string& name) :
	GLES2UniformParameter(name)
{
	this->v = v;
}
bool GLES2UPMat4x4::SetParameter(const GLuint program, const str_type::string& name, const Platform::FileLogger& logger)
{
	const int location = GetLocation(program, name, logger);
	glUniformMatrix4fv(location, 1, GL_FALSE, (GLfloat*)&v.m[0][0]);
	//GLES2Video::CheckGLError(name + ": uniform parameter not found with glUniformMatrix4fv", logger);
	return true;
}

GLES2UPTexture::GLES2UPTexture(const GLenum texturePass, GLuint texture, const GLint unit, const str_type::string& name) :
	GLES2UniformParameter(name)
{
	this->texturePass = texturePass;
	this->texture = texture;
	this->unit = unit;
}
bool GLES2UPTexture::SetParameter(const GLuint program, const str_type::string& name, const Platform::FileLogger& logger)
{
	glActiveTexture(texturePass);
	glBindTexture(GL_TEXTURE_2D, texture);
	const int location = GetLocation(program, name, logger);
	glUniform1i(location, unit);
	//GLES2Video::CheckGLError(name + ": uniform parameter not found with glUniform1i", logger);
	return true;
}

GLES2UPVec2Array::GLES2UPVec2Array(const boost::shared_array<const gs2d::math::Vector2>& v, const unsigned int count, const str_type::string& name) :
	GLES2UniformParameter(name),
	count(static_cast<GLsizei>(count))
{
	this->va = v;
}
bool GLES2UPVec2Array::SetParameter(const GLuint program, const str_type::string& name, const Platform::FileLogger& logger)
{
	const int location = GetLocation(program, name, logger);
	glUniform2fv(location, count, &(va.get()->x));
	//GLES2Video::CheckGLError(name + ": uniform parameter not found with glUniform2fv", logger);
	return true;
}

} // namespace gs2d
