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

#include "ETHScriptWrapper.h"
#include <math.h>

void ETHScriptWrapper::Math::RegisterGlobals(asIScriptEngine *pASEngine)
{
	int r;
	r = pASEngine->RegisterGlobalFunction("float length(const vector2 &in)", asFUNCTIONPR(Length, (const Vector2 &), float), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("float length(const vector3 &in)", asFUNCTIONPR(Length, (const Vector3 &), float), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("float sign(const float)", asFUNCTIONPR(Sign, (const float), float), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("int sign(const int)", asFUNCTIONPR(Sign, (const int), int), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("float distance(const vector2 &in, const vector2 &in)", asFUNCTIONPR(Distance, (const Vector2 &, const Vector2 &), float), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("float distance(const vector3 &in, const vector3 &in)", asFUNCTIONPR(Distance, (const Vector3 &, const Vector3 &), float), asCALL_CDECL); assert(r >= 0);
}

float ETHScriptWrapper::Math::Length(const Vector3 &v)
{
	return sqrtf((v.x*v.x) + (v.y*v.y) + (v.z*v.z));
}

float ETHScriptWrapper::Math::Length(const Vector2 &v)
{
	return sqrtf((v.x*v.x) + (v.y*v.y));
}

float ETHScriptWrapper::Math::Sign(const float v)
{
	return gs2d::math::Sign(v);
}

int ETHScriptWrapper::Math::Sign(const int v)
{
	return gs2d::math::Sign(v);
}

float ETHScriptWrapper::Math::Distance(const Vector2 &a, const Vector2 &b)
{
	return Length(a-b);
}

float ETHScriptWrapper::Math::Distance(const Vector3 &a, const Vector3 &b)
{
	return Length(a-b);
}