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

#ifndef GLES2_UNIFORM_PARAMETER_H_
#define GLES2_UNIFORM_PARAMETER_H_

#include "gs2dGLES2.h"

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <map>

namespace gs2d {

typedef std::map<GLuint, int> LocationMap;
typedef boost::shared_ptr<LocationMap> LocationMapPtr;

class GLES2UniformParameter
{
	LocationMapPtr m_locations;
	str_type::string m_name;
public:
	const str_type::string& GetName() const;
	GLES2UniformParameter(const str_type::string& name);
	void SetLocations(const LocationMapPtr& locations);
	LocationMapPtr GetLocations();
	inline int GetLocation(const GLuint program, const str_type::string& name, const Platform::FileLogger& logger);
	virtual bool SetParameter(const GLuint program, const str_type::string& name, const Platform::FileLogger& logger) = 0;
};

class GLES2UPVec1 : public GLES2UniformParameter
{
	GLfloat v;
public:
	GLES2UPVec1(const float v, const str_type::string& name);
	bool SetParameter(const GLuint program, const str_type::string& name, const Platform::FileLogger& logger);
};

class GLES2UPVec2 : public GLES2UniformParameter
{
	math::Vector2 v;
public:
	GLES2UPVec2(const math::Vector2& v, const str_type::string& name);
	bool SetParameter(const GLuint program, const str_type::string& name, const Platform::FileLogger& logger);
};

class GLES2UPVec3 : public GLES2UniformParameter
{
	math::Vector3 v;
public:
	GLES2UPVec3(const math::Vector3& v, const str_type::string& name);
	bool SetParameter(const GLuint program, const str_type::string& name, const Platform::FileLogger& logger);
};

class GLES2UPVec4 : public GLES2UniformParameter
{
	math::Vector4 v;
public:
	GLES2UPVec4(const math::Vector4& v, const str_type::string& name);
	bool SetParameter(const GLuint program, const str_type::string& name, const Platform::FileLogger& logger);
};

class GLES2UPMat4x4 : public GLES2UniformParameter
{
	math::Matrix4x4 v;
public:
	GLES2UPMat4x4(const math::Matrix4x4& v, const str_type::string& name);
	bool SetParameter(const GLuint program, const str_type::string& name, const Platform::FileLogger& logger);
};

class GLES2UPTexture : public GLES2UniformParameter
{
	GLenum texturePass;
	GLuint texture;
	GLint unit;
public:
	GLES2UPTexture(const GLenum texturePass, GLuint texture, const GLint unit, const str_type::string& name);
	bool SetParameter(const GLuint program, const str_type::string& name, const Platform::FileLogger& logger);
};

class GLES2UPVec2Array : public GLES2UniformParameter
{
	boost::shared_array<const math::Vector2> va;
	GLsizei count;
public:
	GLES2UPVec2Array(const boost::shared_array<const math::Vector2>& v, const unsigned int count, const str_type::string& name);
	bool SetParameter(const GLuint program, const str_type::string& name, const Platform::FileLogger& logger);
};

typedef boost::shared_ptr<GLES2UniformParameter> GLES2UniformParameterPtr;
typedef boost::shared_ptr<GLES2UPVec1> GLES2UPVec1Ptr;
typedef boost::shared_ptr<GLES2UPVec2> GLES2UPVec2Ptr;
typedef boost::shared_ptr<GLES2UPVec3> GLES2UPVec3Ptr;
typedef boost::shared_ptr<GLES2UPVec4> GLES2UPVec4Ptr;
typedef boost::shared_ptr<GLES2UPMat4x4> GLES2UPMat4x4Ptr;
typedef boost::shared_ptr<GLES2UPTexture> GLES2UPTexturePtr;
typedef boost::shared_ptr<GLES2UPVec2Array> GLES2UPVec2ArrayPtr;

} // namespace gs2d

#endif