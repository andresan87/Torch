/*-----------------------------------------------------------------------

 Ethanon Engine (C) Copyright 2009-2011 Andre Santee
 http://www.asantee.net/ethanon/

    This file is part of Ethanon Engine.

    Ethanon Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    Ethanon Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with Ethanon Engine. If not, see
    <http://www.gnu.org/licenses/>.

-----------------------------------------------------------------------*/

#include "ETHScriptObjRegister.h"
#include "ETHEntityArray.h"
#include "ETHDateTime.h"
#include "ETHInput.h"
#include <assert.h>
#include "../gs2d/src/enml/enml.h"

#include "../addons/scriptmath3d.h"
#include "../addons/scriptmath2d.h"
#include "../addons/scriptarray.h"
#include "../addons/scriptmath.h"

#ifdef GS2D_STR_TYPE_WCHAR
 #include "../addons/utf16/scriptfile.h"
 #include "../addons/utf16/scriptstdstring.h"
#else
 #include "../addons/ansi/scriptfile.h"
 #include "../addons/ansi/scriptstdstring.h"
#endif

namespace ETHGlobal {

void Matrix4x4DefaultConstructor(Matrix4x4 *self)
{
	new(self) Matrix4x4();
}

void Matrix4x4CopyConstructor(const Matrix4x4 &other, Matrix4x4 *self)
{
	new(self) Matrix4x4(other);
}

float Matrix4x4Getter(const unsigned int i, const unsigned int j, Matrix4x4 *p)
{
	assert(i < 4);
	assert(j < 4);
	return p->m[i][j];
}

void Matrix4x4Setter(const unsigned int i, const unsigned int j, const float value, Matrix4x4 *p)
{
	assert(i < 4);
	assert(j < 4);
	p->m[i][j] = value;
}

void CollisionBoxDefaultConstructor(ETH_COLLISION_BOX *self)
{
	new(self) ETH_COLLISION_BOX();
}

void CollisionBoxCopyConstructor(const ETH_COLLISION_BOX &other, ETH_COLLISION_BOX *self)
{
	new(self) ETH_COLLISION_BOX(other);
}

void CollisionBoxInitConstructor(const Vector3 &pos, const Vector3 &size, ETH_COLLISION_BOX *self)
{
	new(self) ETH_COLLISION_BOX(pos,size);
}

void EnmlFileConstructor(const str_type::string &str, enml::File *self)
{
	new(self) enml::File(str);
}

void DateTimeConstructor(ETHDateTime *self)
{
	new(self) ETHDateTime();
}

float GetAngle(const Vector2 &v2)
{
	const float r = atan2f(v2.x, v2.y);
	return (r<0) ? r+(2*constant::PI) : r;
}

void AddRef()
{
	// dummy...
}

void Release()
{
	// dummy...
}

ETHEntityArray *ETHEntityArrayFactory()
{
	return new ETHEntityArray();
}

enml::File *EnmlFileFactory()
{
	return new enml::File();
}

enml::Entity *EnmlEntityFactory()
{
	return new enml::Entity();
}

ETHDateTime *DateTimeFactory()
{
	return new ETHDateTime();
}

bool CheckASError(const int r, const str_type::string &description)
{
	if (r < 0)
	{
		ETH_STREAM_DECL(ss) << GS_L("ERROR - ") << description;
		ETHResourceProvider::Log(ss.str(), Platform::FileLogger::ERROR);
		return false;
	}
	else
	{
		return true;
	}
}

Matrix4x4 Scale(const float x, const float y, const float z)
{
	return Matrix4x4(
		x, 0, 0, 0,
		0, y, 0, 0,
		0, 0, z, 0,
		0, 0, 0, 1
	);
}

Matrix4x4 Translate(const float x, const float y, const float z)
{
	return Matrix4x4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		x, y, z, 1
	);
}

Matrix4x4 RotateX(const float theta)
{
	const float fSin=sinf(theta), fCos=cosf(theta);
	return Matrix4x4(
		1, 0,    0,    0,
		0, fCos, fSin, 0,
		0,-fSin, fCos, 0,
		0, 0,    0,    1
	);
}

Matrix4x4 RotateY(const float theta)
{
	const float fSin=sinf(theta), fCos=cosf(theta);
	return Matrix4x4(
		fCos, 0,-fSin, 0,
		0,    1,    0, 0,
		fSin, 0, fCos, 0,
		0,    0,    0, 1
	);
}

Matrix4x4 RotateZ(const float theta)
{
	const float fSin=sinf(theta), fCos=cosf(theta);
	return Matrix4x4(
		fCos, fSin, 0, 0,
		-fSin,fCos, 0, 0,
		0,    0,    1, 0,
		0,    0,    0, 1
	);
}

Matrix4x4 Multiply(const Matrix4x4 &m1, const Matrix4x4 &m2)
{
	Matrix4x4 r;
	for (int i = 0; i<4; i++)
	{
		for (int j = 0; j<4; j++)
		{
			r.m[i][j] = 0;
			for (int k = 0; k < 4; k++)
			{
				r.m[i][j] += m1.m[i][k] * m2.m[k][j];
			}
		}
	}
	return r;
}

bool RegisterInputObject(asIScriptEngine *pASEngine)
{
	int r;
	// register the input object
	r = pASEngine->RegisterObjectType("ETHInput", 0, asOBJ_REF); assert(r >= 0);
	r = pASEngine->RegisterObjectBehaviour("ETHInput", asBEHAVE_ADDREF, "void f()", asFUNCTION(AddRef),  asCALL_CDECL_OBJLAST); assert(r >= 0);
	r = pASEngine->RegisterObjectBehaviour("ETHInput", asBEHAVE_RELEASE, "void f()", asFUNCTION(Release),  asCALL_CDECL_OBJLAST); assert(r >= 0);

	// register ETHInput methods
	r = pASEngine->RegisterObjectMethod("ETHInput", "KEY_STATE GetLeftClickState()", asMETHOD(ETHInput,GetLeftClickState), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHInput", "KEY_STATE GetRightClickState()", asMETHOD(ETHInput,GetRightClickState), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHInput", "KEY_STATE GetMiddleClickState()", asMETHOD(ETHInput,GetMiddleClickState), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHInput", "vector2 GetMouseMove()", asMETHOD(ETHInput,GetMouseMove), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHInput", "float GetWheelState()", asMETHOD(ETHInput,GetWheelState), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHInput", "KEY_STATE GetKeyState(const KEY)", asMETHOD(ETHInput,GetKeyState), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHInput", "bool KeyDown(const KEY)", asMETHOD(ETHInput,KeyDown), asCALL_THISCALL); assert(r >= 0);

	r = pASEngine->RegisterObjectMethod("ETHInput", "KEY_STATE JoyButtonState(const uint, const J_KEY) const", asMETHOD(ETHInput,JoyButtonState), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHInput", "bool JoyButtonDown(const uint, const J_KEY) const", asMETHOD(ETHInput,JoyButtonDown), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHInput", "bool DetectJoysticks() const", asMETHOD(ETHInput,DetectJoysticks), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHInput", "J_STATUS GetJoystickStatus(const uint) const", asMETHOD(ETHInput,GetJoystickStatus), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHInput", "uint GetNumJoyButtons(const uint) const", asMETHOD(ETHInput,GetNumJoyButtons), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHInput", "vector2 GetJoystickXY(const uint) const", asMETHOD(ETHInput,GetJoystickXY), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHInput", "float GetJoystickZ(const uint) const", asMETHOD(ETHInput,GetJoystickZ), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHInput", "float GetJoystickRudder(const uint) const", asMETHOD(ETHInput,GetJoystickRudder), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHInput", "vector2 GetJoystickUV(const uint) const", asMETHOD(ETHInput,GetJoystickUV), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHInput", "J_KEY GetFirstButtonDown(const uint) const", asMETHOD(ETHInput,GetFirstButtonDown), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHInput", "uint GetMaxJoysticks() const", asMETHOD(ETHInput,GetMaxJoysticks), asCALL_THISCALL); assert(r >= 0);

	return true;
}

bool RegisterENMLObjetct(asIScriptEngine *pASEngine)
{
	int r;
	// register the ENML entity object
	r = pASEngine->RegisterObjectType("enmlEntity", 0, asOBJ_REF); assert(r >= 0);
	r = pASEngine->RegisterObjectBehaviour("enmlEntity", asBEHAVE_FACTORY, "enmlEntity@ f()", asFUNCTION(EnmlEntityFactory), asCALL_CDECL); assert( r >= 0 );
	r = pASEngine->RegisterObjectBehaviour("enmlEntity", asBEHAVE_ADDREF, "void f()", asMETHOD(enml::Entity,addRef), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectBehaviour("enmlEntity", asBEHAVE_RELEASE, "void f()", asMETHOD(enml::Entity,release), asCALL_THISCALL); assert(r >= 0);

	// methods
	r = pASEngine->RegisterObjectMethod("enmlEntity", "void clear()", asMETHOD(enml::Entity,clear), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("enmlEntity", "void add(const string &in, string)", asMETHOD(enml::Entity,add), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("enmlEntity", "string get(const string &in)", asMETHOD(enml::Entity,get), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("enmlEntity", "string getAttributeNames() const", asMETHOD(enml::Entity,getAttributeNames), asCALL_THISCALL); assert(r >= 0);

	// register the ENML file object
	r = pASEngine->RegisterObjectType("enmlFile", 0, asOBJ_REF); assert(r >= 0);
	r = pASEngine->RegisterObjectBehaviour("enmlFile", asBEHAVE_FACTORY, "enmlFile@ f()", asFUNCTION(EnmlFileFactory), asCALL_CDECL); assert( r >= 0 );
	r = pASEngine->RegisterObjectBehaviour("enmlFile", asBEHAVE_ADDREF, "void f()", asMETHOD(enml::File,addRef), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectBehaviour("enmlFile", asBEHAVE_RELEASE, "void f()", asMETHOD(enml::File,release), asCALL_THISCALL); assert(r >= 0);
	//r = pASEngine->RegisterObjectBehaviour("enmlFile", asBEHAVE_CONSTRUCT,  "void f(const string &in)", asFUNCTION(EnmlFileConstructor), asCALL_CDECL_OBJLAST); assert( r >= 0 );

	// methods
	r = pASEngine->RegisterObjectMethod("enmlFile", "void clear()", asMETHOD(enml::File,clear), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("enmlFile", "void addEntity(const string &in, const enmlEntity &in)", asMETHOD(enml::File,addEntity), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("enmlFile", "string generateString() const", asMETHOD(enml::File,generateString), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("enmlFile", "uint parseString(const string &in)", asMETHOD(enml::File,parseString), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("enmlFile", "bool exists(const string &in) const", asMETHOD(enml::File,exists), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("enmlFile", "string get(const string &in, const string &in) const", asMETHOD(enml::File,get), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("enmlFile", "bool getDouble(const string &in, const string &in, double &out) const", asMETHOD(enml::File,getDouble), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("enmlFile", "bool getInt(const string &in, const string &in, int &out) const", asMETHOD(enml::File,getInt), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("enmlFile", "bool getUInt(const string &in, const string &in, uint &out) const", asMETHOD(enml::File,getUInt), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("enmlFile", "bool getFloat(const string &in, const string &in, float &out) const", asMETHOD(enml::File,getFloat), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("enmlFile", "string getErrorString() const", asMETHOD(enml::File,getErrorString), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("enmlFile", "string getEntityNames() const", asMETHOD(enml::File,getEntityNames), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("enmlFile", "string getAttributeNames(const string &in) const", asMETHOD(enml::File,getAttributeNames), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("enmlFile", "void writeToFile(const string &in) const", asMETHOD(enml::File,writeToFile), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("enmlFile", "bool parseFromFile(const string &in)", asMETHOD(enml::File,parseFromFile), asCALL_THISCALL); assert(r >= 0);

	// util globals
	r = pASEngine->RegisterGlobalFunction("string GetStringFromFile(const string &in)", asFUNCTION(enml::getStringFromAnsiFile), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("bool SaveStringToFile(const string &in, const string &in)", asFUNCTION(enml::saveStringToAnsiFile), asCALL_CDECL); assert(r >= 0);

	return true;
}

bool RegisterEntityArrayObject(asIScriptEngine *pASEngine)
{
	int r;
	// register the entity vector object
	r = pASEngine->RegisterObjectType("ETHEntityArray", 0, asOBJ_REF); assert(r >= 0);
	r = pASEngine->RegisterObjectBehaviour("ETHEntityArray", asBEHAVE_FACTORY, "ETHEntityArray@ f()", asFUNCTION(ETHEntityArrayFactory), asCALL_CDECL); assert( r >= 0 );
	r = pASEngine->RegisterObjectBehaviour("ETHEntityArray", asBEHAVE_ADDREF, "void f()", asMETHOD(ETHEntityArray,AddRef), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectBehaviour("ETHEntityArray", asBEHAVE_RELEASE, "void f()", asMETHOD(ETHEntityArray,Release), asCALL_THISCALL); assert(r >= 0);

	// methods and operators
	r = pASEngine->RegisterObjectMethod("ETHEntityArray", "ETHEntity@ &opIndex(uint)", asMETHODPR(ETHEntityArray, operator[], (const unsigned int), ETHEntityRawPtr &), asCALL_THISCALL); assert( r >= 0 );
	r = pASEngine->RegisterObjectMethod("ETHEntityArray", "const ETHEntity@ &opIndex(uint) const", asMETHODPR(ETHEntityArray, operator[], (const unsigned int), ETHEntityRawPtr &), asCALL_THISCALL); assert( r >= 0 );

	r = pASEngine->RegisterObjectMethod("ETHEntityArray", "ETHEntityArray &opAddAssign(const ETHEntityArray &in)", asMETHOD(ETHEntityArray, operator+=), asCALL_THISCALL); assert( r >= 0 );
	r = pASEngine->RegisterObjectMethod("ETHEntityArray", "void push_back(const ETHEntity &in)", asMETHOD(ETHEntityArray,push_back), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntityArray", "uint size() const", asMETHOD(ETHEntityArray,size), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntityArray", "void clear()", asMETHOD(ETHEntityArray,clear), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntityArray", "void unique()", asMETHOD(ETHEntityArray,unique), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntityArray", "void removeDeadEntities()", asMETHOD(ETHEntityArray,removeDeadEntities), asCALL_THISCALL); assert(r >= 0);

	return true;
}

bool RegisterEntityObject(asIScriptEngine *pASEngine)
{
	int r;
	// register the entity object
	r = pASEngine->RegisterObjectType("ETHEntity", 0, asOBJ_REF); assert(r >= 0);
	r = pASEngine->RegisterObjectBehaviour("ETHEntity", asBEHAVE_ADDREF, "void f()", asMETHOD(ETHScriptEntity,AddRef), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectBehaviour("ETHEntity", asBEHAVE_RELEASE, "void f()", asMETHOD(ETHScriptEntity,Release), asCALL_THISCALL); assert(r >= 0);

	// register ETHEntity methods
	r = pASEngine->RegisterObjectMethod("ETHEntity", "vector3 GetPosition() const", asMETHOD(ETHScriptEntity,GetPosition), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "vector2 GetPositionXY() const", asMETHOD(ETHScriptEntity,GetPositionXY), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "vector2 GetSize() const", asMETHOD(ETHScriptEntity,GetCurrentSize), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "uint GetFrame() const", asMETHOD(ETHScriptEntity,GetFrame), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "bool SetFrame(const uint)", asMETHODPR(ETHScriptEntity,SetFrame,(const unsigned int),bool), asCALL_THISCALL);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "bool SetFrame(const uint, const uint)", asMETHODPR(ETHScriptEntity,SetFrame,(const unsigned int,const unsigned int),bool), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "vector2 GetScreenRectMin() const", asMETHOD(ETHScriptEntity,GetScreenRectMin), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "vector2 GetScreenRectMax() const", asMETHOD(ETHScriptEntity,GetScreenRectMax), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "int GetID() const", asMETHOD(ETHScriptEntity,GetID), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "float GetAngle() const", asMETHOD(ETHScriptEntity,GetAngle), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "void SetAngle(const float)", asMETHOD(ETHScriptEntity,SetAngle), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "bool IsStatic() const", asMETHOD(ETHScriptEntity,IsStatic), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "void TurnDynamic()", asMETHOD(ETHScriptEntity,TurnDynamic), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "void SilenceParticleSystems(const bool)", asMETHOD(ETHScriptEntity,SilenceParticleSystems), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "void SetSoundVolume(const float)", asMETHOD(ETHScriptEntity,SetSoundVolume), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "collisionBox GetCollisionBox() const", asMETHOD(ETHScriptEntity,GetCollisionBox), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "string GetEntityName() const", asMETHOD(ETHScriptEntity,GetEntityName), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "bool Collidable() const", asMETHOD(ETHScriptEntity, IsCollidable), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "DATA_TYPE CheckCustomData(const string &in) const", asMETHOD(ETHScriptEntity,CheckCustomData), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "bool HasCustomData() const", asMETHOD(ETHScriptEntity,HasCustomData), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "void DebugPrintCustomData() const", asMETHOD(ETHScriptEntity,DebugPrintCustomData), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "uint GetNumFrames() const", asMETHOD(ETHScriptEntity,GetNumFrames), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "ENTITY_TYPE GetType() const", asMETHOD(ETHScriptEntity,GetType), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "bool SetSprite(const string &in)", asMETHODPR(ETHScriptEntity,SetSprite,(const str_type::string&), bool), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "bool SetNormal(const string &in)", asMETHODPR(ETHScriptEntity,SetNormal,(const str_type::string&), bool), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "bool IsInvisible() const", asMETHOD(ETHScriptEntity,IsInvisible), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "bool HasParticleSystem() const",     asMETHODPR(ETHScriptEntity,HasParticleSystems, (void) const, bool), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "bool HasParticleSystem(uint) const", asMETHODPR(ETHScriptEntity,HasParticleSystem, (const unsigned int) const, bool), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "bool HasLightSource() const", asMETHOD(ETHScriptEntity,HasLightSource), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "bool HasSoundEffect() const", asMETHOD(ETHScriptEntity,HasSoundEffect), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "bool PlayParticleSystem(const uint)", asMETHOD(ETHScriptEntity,PlayParticleSystem), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "void SetLightRange(const float)", asMETHOD(ETHScriptEntity,SetLightRange), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "float GetLightRange() const", asMETHOD(ETHScriptEntity,GetLightRange), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "void SetLightColor(const vector3 &in)", asMETHOD(ETHScriptEntity,SetLightColor), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "vector3 GetLightColor() const", asMETHOD(ETHScriptEntity,GetLightColor), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "void SetEmissiveColor(const vector3 &in)", asMETHOD(ETHScriptEntity,SetEmissiveColor), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "vector3 GetEmissiveColor() const", asMETHOD(ETHScriptEntity,GetEmissiveColor), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "void KillParticleSystem(const uint)", asMETHOD(ETHScriptEntity,KillParticleSystem), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "bool ParticlesKilled(const uint) const", asMETHOD(ETHScriptEntity,ParticlesKilled), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "bool EraseData(const string &in)", asMETHOD(ETHScriptEntity,EraseData), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "void SetColor(const vector3 &in)", asMETHOD(ETHScriptEntity,SetColor), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "void SetAlpha(const float)", asMETHOD(ETHScriptEntity,SetAlpha), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "vector3 GetColor()", asMETHOD(ETHScriptEntity,GetColor), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "float GetAlpha()", asMETHOD(ETHScriptEntity,GetAlpha), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "bool IsAlive() const", asMETHOD(ETHScriptEntity,IsAlive), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "bool IsHidden() const", asMETHOD(ETHScriptEntity,IsHidden), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "void Hide(const bool)", asMETHOD(ETHScriptEntity,Hide), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "float GetShadowZ() const", asMETHOD(ETHScriptEntity,GetShadowZ), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "void SetShadowZ(const float)", asMETHOD(ETHScriptEntity,SetShadowZ), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "bool AreParticlesOver() const", asMETHOD(ETHScriptEntity,AreParticlesOver), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "bool HasShadow() const", asMETHOD(ETHScriptEntity,HasShadow), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "bool HasHalo() const", asMETHOD(ETHScriptEntity,HasHalo), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "void SetLayerDepth(const float)", asMETHOD(ETHScriptEntity,SetLayerDepth), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "float GetLayerDepth() const", asMETHOD(ETHScriptEntity,GetLayerDepth), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "void AddToAngle(const float)", asMETHOD(ETHScriptEntity,AddToAngle), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "bool SetSpriteCut(const uint col, const uint row)", asMETHOD(ETHScriptEntity,SetSpriteCut), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "void SetCollision(const bool)", asMETHOD(ETHScriptEntity,SetCollision), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "void SetParticlePosition(const uint n, const vector3 &in)", asMETHOD(ETHScriptEntity,SetParticlePosition), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "void ScaleParticleSystem(const uint n, const float)", asMETHOD(ETHScriptEntity,ScaleParticleSystem), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "bool MirrorParticleSystemX(const uint n, const bool)", asMETHOD(ETHScriptEntity,MirrorParticleSystemX), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "bool MirrorParticleSystemY(const uint n, const bool)", asMETHOD(ETHScriptEntity,MirrorParticleSystemY), asCALL_THISCALL); assert(r >= 0);

	r = pASEngine->RegisterObjectMethod("ETHEntity", "bool SetFloat(const string &in, const float &in)", asMETHOD(ETHScriptEntity,SetFloat), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "bool SetInt(const string &in, const int &in)", asMETHOD(ETHScriptEntity,SetInt), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "bool SetUInt(const string &in, const uint &in)", asMETHOD(ETHScriptEntity,SetUInt), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "bool SetString(const string &in, const string &in)", asMETHOD(ETHScriptEntity,SetString), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "bool SetVector2(const string &in, const vector2 &in)", asMETHOD(ETHScriptEntity,SetVector2), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "bool SetVector3(const string &in, const vector3 &in)", asMETHOD(ETHScriptEntity,SetVector3), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "float GetFloat(const string &in) const", asMETHOD(ETHScriptEntity,GetFloat), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "int GetInt(const string &in) const", asMETHOD(ETHScriptEntity,GetInt), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "uint GetUInt(const string &in) const", asMETHOD(ETHScriptEntity,GetUInt), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "string GetString(const string &in) const", asMETHOD(ETHScriptEntity,GetString), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "vector2 GetVector2(const string &in) const", asMETHOD(ETHScriptEntity,GetVector2), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "vector3 GetVector3(const string &in) const", asMETHOD(ETHScriptEntity,GetVector3), asCALL_THISCALL); assert(r >= 0);

	r = pASEngine->RegisterObjectMethod("ETHEntity", "void AddToFloat(const string &in, const float &in)", asMETHOD(ETHScriptEntity,AddToFloat), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "void AddToInt(const string &in, const int &in)", asMETHOD(ETHScriptEntity,AddToInt), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "void AddToUInt(const string &in, const uint &in)", asMETHOD(ETHScriptEntity,AddToUInt), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "void AddToVector2(const string &in, const vector2 &in)", asMETHOD(ETHScriptEntity,AddToVector2), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "void AddToVector3(const string &in, const vector3 &in)", asMETHOD(ETHScriptEntity,AddToVector3), asCALL_THISCALL); assert(r >= 0);

	r = pASEngine->RegisterObjectMethod("ETHEntity", "void MultiplyFloat(const string &in, const float &in)", asMETHOD(ETHScriptEntity,MultiplyFloat), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "void MultiplyInt(const string &in, const int &in)", asMETHOD(ETHScriptEntity,MultiplyInt), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "void MultiplyUInt(const string &in, const uint &in)", asMETHOD(ETHScriptEntity,MultiplyUInt), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "void MultiplyVector2(const string &in, const float &in)", asMETHOD(ETHScriptEntity,MultiplyVector2), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "void MultiplyVector3(const string &in, const float &in)", asMETHOD(ETHScriptEntity,MultiplyVector3), asCALL_THISCALL); assert(r >= 0);

#ifdef ETH_REGISTER_DEPRECATED_CUSTOM_DATA_METHODS
	r = pASEngine->RegisterObjectMethod("ETHEntity", "bool AddFloatData(const string &in, const float)", asMETHOD(ETHScriptEntity,SetFloat), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "bool AddIntData(const string &in, const int)", asMETHOD(ETHScriptEntity,SetInt), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "bool AddUIntData(const string &in, const uint)", asMETHOD(ETHScriptEntity,SetUInt), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "bool AddStringData(const string &in, const string &in)", asMETHOD(ETHScriptEntity,SetString), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "bool AddVector2Data(const string &in, const vector2 &in)", asMETHOD(ETHScriptEntity,SetVector2), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "bool AddVector3Data(const string &in, const vector3 &in)", asMETHOD(ETHScriptEntity,SetVector3), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "float GetFloatData(const string &in) const", asMETHOD(ETHScriptEntity,GetFloat), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "int GetIntData(const string &in) const", asMETHOD(ETHScriptEntity,GetInt), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "uint GetUIntData(const string &in) const", asMETHOD(ETHScriptEntity,GetUInt), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "string GetStringData(const string &in) const", asMETHOD(ETHScriptEntity,GetString), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "vector2 GetVector2Data(const string &in) const", asMETHOD(ETHScriptEntity,GetVector2), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "vector3 GetVector3Data(const string &in) const", asMETHOD(ETHScriptEntity,GetVector3), asCALL_THISCALL); assert(r >= 0);
#endif

	return true;
}

void RegisterEnumTypes(asIScriptEngine *pASEngine)
{
	int r;
	// Entity type
	r = pASEngine->RegisterEnum("ENTITY_TYPE"); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("ENTITY_TYPE", "ET_HORIZONTAL", ETH_HORIZONTAL); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("ENTITY_TYPE", "ET_VERTICAL", ETH_VERTICAL); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("ENTITY_TYPE", "ET_OVERALL", ETH_OVERALL); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("ENTITY_TYPE", "ET_GROUND_DECAL", ETH_GROUND_DECAL); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("ENTITY_TYPE", "ET_OPAQUE_DECAL", ETH_OPAQUE_DECAL); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("ENTITY_TYPE", "ET_LAYERABLE", ETH_LAYERABLE); assert(r >= 0);

	// Custom data type
	r = pASEngine->RegisterEnum("DATA_TYPE"); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("DATA_TYPE", "DT_NODATA", ETHDT_NODATA); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("DATA_TYPE", "DT_INT", ETHDT_INT); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("DATA_TYPE", "DT_UINT", ETHDT_UINT); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("DATA_TYPE", "DT_FLOAT", ETHDT_FLOAT); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("DATA_TYPE", "DT_STRING", ETHDT_STRING); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("DATA_TYPE", "DT_VECTOR2", ETHDT_VECTOR2); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("DATA_TYPE", "DT_VECTOR3", ETHDT_VECTOR3); assert(r >= 0);

	// Pixel format
	r = pASEngine->RegisterEnum("PIXEL_FORMAT"); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("PIXEL_FORMAT", "PF32BIT", GSPF_32BIT); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("PIXEL_FORMAT", "PF16BIT", GSPF_16BIT); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("PIXEL_FORMAT", "PFUNKNOWN", GSPF_UNKNOWN); assert(r >= 0);

	// GS_KEY_STATE
	r = pASEngine->RegisterEnum("KEY_STATE"); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY_STATE", "KS_HIT", GSKS_HIT); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY_STATE", "KS_UP", GSKS_UP); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY_STATE", "KS_DOWN", GSKS_DOWN); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY_STATE", "KS_RELEASE", GSKS_RELEASE); assert(r >= 0);

	// Joystick status
	r = pASEngine->RegisterEnum("J_STATUS"); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("J_STATUS", "JS_DETECTED", GSJS_DETECTED); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("J_STATUS", "JS_NOTDETECTED", GSJS_NOTDETECTED); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("J_STATUS", "JS_INVALID", GSJS_INVALID); assert(r >= 0);

	// Joystick keys
	r = pASEngine->RegisterEnum("J_KEY"); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("J_KEY", "JK_01", GSB_01); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("J_KEY", "JK_02", GSB_02); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("J_KEY", "JK_03", GSB_03); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("J_KEY", "JK_04", GSB_04); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("J_KEY", "JK_05", GSB_05); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("J_KEY", "JK_06", GSB_06); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("J_KEY", "JK_07", GSB_07); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("J_KEY", "JK_08", GSB_08); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("J_KEY", "JK_09", GSB_09); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("J_KEY", "JK_10", GSB_10); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("J_KEY", "JK_11", GSB_11); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("J_KEY", "JK_12", GSB_12); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("J_KEY", "JK_13", GSB_13); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("J_KEY", "JK_14", GSB_14); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("J_KEY", "JK_15", GSB_15); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("J_KEY", "JK_16", GSB_16); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("J_KEY", "JK_17", GSB_17); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("J_KEY", "JK_18", GSB_18); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("J_KEY", "JK_19", GSB_19); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("J_KEY", "JK_20", GSB_20); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("J_KEY", "JK_21", GSB_21); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("J_KEY", "JK_22", GSB_22); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("J_KEY", "JK_23", GSB_23); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("J_KEY", "JK_24", GSB_24); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("J_KEY", "JK_25", GSB_25); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("J_KEY", "JK_26", GSB_26); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("J_KEY", "JK_27", GSB_27); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("J_KEY", "JK_28", GSB_28); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("J_KEY", "JK_29", GSB_29); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("J_KEY", "JK_30", GSB_30); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("J_KEY", "JK_31", GSB_31); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("J_KEY", "JK_32", GSB_32); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("J_KEY", "JK_UP", GSB_UP); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("J_KEY", "JK_DOWN", GSB_DOWN); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("J_KEY", "JK_LEFT", GSB_LEFT); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("J_KEY", "JK_RIGHT", GSB_RIGHT); assert(r >= 0);

	// GS_KEY
	r = pASEngine->RegisterEnum("KEY"); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_UP", GSK_UP); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_DOWN", GSK_DOWN); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_LEFT", GSK_LEFT); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_RIGHT", GSK_RIGHT); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_PAGEDOWN", GSK_PAGEDOWN); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_PAGEUP", GSK_PAGEUP); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_SPACE", GSK_SPACE); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_ENTER", GSK_ENTER); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_RETURN", GSK_ENTER); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_DELETE", GSK_DELETE); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_HOME", GSK_HOME); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_END", GSK_END); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_INSERT", GSK_INSERT); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_PAUSE", GSK_PAUSE); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_ESC", GSK_ESC); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_BACK", GSK_BACK); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_BACKSPACE", GSK_BACK); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_TAB", GSK_TAB); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_PRINTSCREEN", GSK_PRINTSCREEN); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_SUBTRACT", GSK_SUBTRACT); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_ADD", GSK_ADD); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_F1", GSK_F1); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_F2", GSK_F2); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_F3", GSK_F3); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_F4", GSK_F4); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_F5", GSK_F5); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_F6", GSK_F6); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_F7", GSK_F7); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_F8", GSK_F8); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_F9", GSK_F9); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_F10", GSK_F10); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_F11", GSK_F11); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_F12", GSK_F12); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_F13", GSK_F13); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_F14", GSK_F14); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_F15", GSK_F15); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_F16", GSK_F16); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_F17", GSK_F17); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_F18", GSK_F18); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_F19", GSK_F19); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_F20", GSK_F20); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_F21", GSK_F21); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_F22", GSK_F22); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_F23", GSK_F23); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_F24", GSK_F24); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_A", GSK_A); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_B", GSK_B); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_C", GSK_C); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_D", GSK_D); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_E", GSK_E); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_F", GSK_F); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_G", GSK_G); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_H", GSK_H); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_I", GSK_I); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_J", GSK_J); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_K", GSK_K); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_L", GSK_L); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_M", GSK_M); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_N", GSK_N); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_O", GSK_O); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_P", GSK_P); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_Q", GSK_Q); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_R", GSK_R); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_S", GSK_S); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_T", GSK_T); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_U", GSK_U); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_V", GSK_V); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_W", GSK_W); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_X", GSK_X); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_Y", GSK_Y); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_Z", GSK_Z); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_0", GSK_0); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_1", GSK_1); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_2", GSK_2); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_3", GSK_3); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_4", GSK_4); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_5", GSK_5); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_6", GSK_6); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_7", GSK_7); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_8", GSK_8); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_9", GSK_9); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_MINUS", GSK_MINUS); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_PLUS", GSK_PLUS); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_COMMA", GSK_COMMA); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_DOT", GSK_DOT); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_CTRL", GSK_CTRL); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_ALT", GSK_ALT); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_SHIFT", GSK_SHIFT); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_RMOUSE", GSK_RMOUSE); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_MMOUSE", GSK_MMOUSE); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_LMOUSE", GSK_LMOUSE); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_NUMPAD0", GSK_NUMPAD0); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_NUMPAD1", GSK_NUMPAD1); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_NUMPAD2", GSK_NUMPAD2); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_NUMPAD3", GSK_NUMPAD3); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_NUMPAD4", GSK_NUMPAD4); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_NUMPAD5", GSK_NUMPAD5); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_NUMPAD6", GSK_NUMPAD6); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_NUMPAD7", GSK_NUMPAD7); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_NUMPAD8", GSK_NUMPAD8); assert(r >= 0);
	r = pASEngine->RegisterEnumValue("KEY", "K_NUMPAD9", GSK_NUMPAD9); assert(r >= 0);
}

void RegisterGlobalProperties(asIScriptEngine *pASEngine)
{
	int r;
	static float pi = gs2d::math::constant::PI;
	static float pib = gs2d::math::constant::PIb;
	r = pASEngine->RegisterGlobalProperty("const float PI", &pi); assert( r >= 0 );
	r = pASEngine->RegisterGlobalProperty("const float PIb", &pib); assert( r >= 0 );
}

bool RegisterCollisionObject(asIScriptEngine *pASEngine)
{
	int r;
	r = pASEngine->RegisterObjectType("collisionBox", sizeof(ETH_COLLISION_BOX), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CA); assert(r >= 0);

	// Register the object properties
	r = pASEngine->RegisterObjectProperty("collisionBox", "vector3 pos", offsetof(ETH_COLLISION_BOX, pos)); assert(r >= 0);
	r = pASEngine->RegisterObjectProperty("collisionBox", "vector3 size", offsetof(ETH_COLLISION_BOX, size)); assert(r >= 0);

	// constructors
	r = pASEngine->RegisterObjectBehaviour("collisionBox", asBEHAVE_CONSTRUCT,  "void f()",                     asFUNCTION(CollisionBoxDefaultConstructor), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = pASEngine->RegisterObjectBehaviour("collisionBox", asBEHAVE_CONSTRUCT,  "void f(const collisionBox &in)",       asFUNCTION(CollisionBoxCopyConstructor), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = pASEngine->RegisterObjectBehaviour("collisionBox", asBEHAVE_CONSTRUCT,  "void f(const vector3 &in, const vector3 &in)",  asFUNCTION(CollisionBoxInitConstructor), asCALL_CDECL_OBJLAST); assert( r >= 0 );

	return true;
}

bool RegisterMathObjectsAndFunctions(asIScriptEngine *pASEngine)
{
	// register matrix object
	int r;
	r = pASEngine->RegisterObjectType("matrix4x4", sizeof(Matrix4x4), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CA); assert(r >= 0);

	// Register the object properties
	r = pASEngine->RegisterObjectProperty("matrix4x4", "float a11", offsetof(Matrix4x4, mat.a11)); assert(r >= 0);
	r = pASEngine->RegisterObjectProperty("matrix4x4", "float a12", offsetof(Matrix4x4, mat.a12)); assert(r >= 0);
	r = pASEngine->RegisterObjectProperty("matrix4x4", "float a13", offsetof(Matrix4x4, mat.a13)); assert(r >= 0);
	r = pASEngine->RegisterObjectProperty("matrix4x4", "float a14", offsetof(Matrix4x4, mat.a14)); assert(r >= 0);

	r = pASEngine->RegisterObjectProperty("matrix4x4", "float a21", offsetof(Matrix4x4, mat.a21)); assert(r >= 0);
	r = pASEngine->RegisterObjectProperty("matrix4x4", "float a22", offsetof(Matrix4x4, mat.a22)); assert(r >= 0);
	r = pASEngine->RegisterObjectProperty("matrix4x4", "float a23", offsetof(Matrix4x4, mat.a23)); assert(r >= 0);
	r = pASEngine->RegisterObjectProperty("matrix4x4", "float a24", offsetof(Matrix4x4, mat.a24)); assert(r >= 0);

	r = pASEngine->RegisterObjectProperty("matrix4x4", "float a31", offsetof(Matrix4x4, mat.a31)); assert(r >= 0);
	r = pASEngine->RegisterObjectProperty("matrix4x4", "float a32", offsetof(Matrix4x4, mat.a32)); assert(r >= 0);
	r = pASEngine->RegisterObjectProperty("matrix4x4", "float a33", offsetof(Matrix4x4, mat.a33)); assert(r >= 0);
	r = pASEngine->RegisterObjectProperty("matrix4x4", "float a34", offsetof(Matrix4x4, mat.a34)); assert(r >= 0);

	r = pASEngine->RegisterObjectProperty("matrix4x4", "float a41", offsetof(Matrix4x4, mat.a41)); assert(r >= 0);
	r = pASEngine->RegisterObjectProperty("matrix4x4", "float a42", offsetof(Matrix4x4, mat.a42)); assert(r >= 0);
	r = pASEngine->RegisterObjectProperty("matrix4x4", "float a43", offsetof(Matrix4x4, mat.a43)); assert(r >= 0);
	r = pASEngine->RegisterObjectProperty("matrix4x4", "float a44", offsetof(Matrix4x4, mat.a44)); assert(r >= 0);

	// constructors
	r = pASEngine->RegisterObjectBehaviour("matrix4x4", asBEHAVE_CONSTRUCT,  "void f()",                    asFUNCTION(Matrix4x4DefaultConstructor), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = pASEngine->RegisterObjectBehaviour("matrix4x4", asBEHAVE_CONSTRUCT,  "void f(const matrix4x4 &in)", asFUNCTION(Matrix4x4CopyConstructor), asCALL_CDECL_OBJLAST); assert( r >= 0 );

	// matrix functions
	r = pASEngine->RegisterGlobalFunction("matrix4x4 scale(const float, const float, const float)", asFUNCTION(Scale), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("matrix4x4 translate(const float, const float, const float)", asFUNCTION(Translate), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("matrix4x4 rotateX(const float)", asFUNCTION(RotateX), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("matrix4x4 rotateY(const float)", asFUNCTION(RotateY), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("matrix4x4 rotateZ(const float)", asFUNCTION(RotateZ), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("matrix4x4 multiply(const matrix4x4 &in, const matrix4x4 &in)", asFUNCTIONPR(Multiply, (const Matrix4x4&, const Matrix4x4&), Matrix4x4), asCALL_CDECL); assert(r >= 0);

	// methods from globals
	r = pASEngine->RegisterObjectMethod("matrix4x4", "float get(const uint i, const uint j) const", asFUNCTION(Matrix4x4Getter), asCALL_CDECL_OBJLAST); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("matrix4x4", "void set(const uint i, const uint j, const float)", asFUNCTION(Matrix4x4Setter), asCALL_CDECL_OBJLAST); assert(r >= 0);

	//float Matrix4x4Getter(const unsigned int i, const unsigned int j, Matrix4x4 *p)
	//void Matrix4x4Setter(const unsigned int i, const unsigned int j, const float value, Matrix4x4 *p)

	// misc functions:
	r = pASEngine->RegisterGlobalFunction("float getAngle(const vector2 &in)", asFUNCTION(GetAngle), asCALL_CDECL); assert(r >= 0);

	return true;
}

bool RegisterVideoModeObject(asIScriptEngine *pASEngine)
{
	int r;
	r = pASEngine->RegisterObjectType("videoMode", sizeof(ETH_VIDEO_MODE), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CA); assert(r >= 0);

	// Register the object properties
	r = pASEngine->RegisterObjectProperty("videoMode", "uint width", offsetof(ETH_VIDEO_MODE, width)); assert(r >= 0);
	r = pASEngine->RegisterObjectProperty("videoMode", "uint height", offsetof(ETH_VIDEO_MODE, height)); assert(r >= 0);
	r = pASEngine->RegisterObjectProperty("videoMode", "PIXEL_FORMAT format", offsetof(ETH_VIDEO_MODE, pf)); assert(r >= 0);

	return true;
}

bool RegisterDateTimeObject(asIScriptEngine *pASEngine)
{
	int r;
	// register the ENML entity object
	r = pASEngine->RegisterObjectType("dateTime", 0, asOBJ_REF); assert(r >= 0);
	r = pASEngine->RegisterObjectBehaviour("dateTime", asBEHAVE_FACTORY, "dateTime@ f()", asFUNCTION(DateTimeFactory), asCALL_CDECL); assert( r >= 0 );
	r = pASEngine->RegisterObjectBehaviour("dateTime", asBEHAVE_ADDREF, "void f()", asMETHOD(ETHDateTime,AddRef), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectBehaviour("dateTime", asBEHAVE_RELEASE, "void f()", asMETHOD(ETHDateTime,Release), asCALL_THISCALL); assert(r >= 0);

	// methods
	r = pASEngine->RegisterObjectMethod("dateTime", "uint update()", asMETHOD(ETHDateTime,Update), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("dateTime", "uint getDay() const", asMETHOD(ETHDateTime,GetDay), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("dateTime", "uint getMonth() const", asMETHOD(ETHDateTime,GetMonth), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("dateTime", "uint getYear() const", asMETHOD(ETHDateTime,GetYear), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("dateTime", "uint getHours() const", asMETHOD(ETHDateTime,GetHours), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("dateTime", "uint getMinutes() const", asMETHOD(ETHDateTime,GetMinutes), asCALL_THISCALL); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("dateTime", "uint getSeconds() const", asMETHOD(ETHDateTime,GetSeconds), asCALL_THISCALL); assert(r >= 0);

	return true;
}

void RegisterAllObjects(asIScriptEngine *pASEngine)
{
	// Register some useful scripts
	RegisterScriptMath2D(pASEngine);
	RegisterScriptMath3D(pASEngine);
	RegisterStdString(pASEngine);
	RegisterScriptMath(pASEngine);
	RegisterScriptFile(pASEngine);
	// RegisterScriptDictionary_Native(m_pASEngine);
	RegisterScriptArray(pASEngine, true);

	ETHGlobal::RegisterCollisionObject(pASEngine);
	ETHGlobal::RegisterVideoModeObject(pASEngine);
	ETHGlobal::RegisterMathObjectsAndFunctions(pASEngine);
	ETHGlobal::RegisterENMLObjetct(pASEngine);
	ETHGlobal::RegisterDateTimeObject(pASEngine);
	ETHGlobal::RegisterEntityObject(pASEngine);
	ETHGlobal::RegisterInputObject(pASEngine);
	ETHGlobal::RegisterEntityArrayObject(pASEngine);
}

}
