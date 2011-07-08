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

#ifndef ETH_SCRIPT_OBJECT_REGISTER_H_
#define ETH_SCRIPT_OBJECT_REGISTER_H_

#include "ETHScene.h"
#include <ostream>
#include <string>

namespace ETHGlobal
{
	void Matrix4x4DefaultConstructor(Matrix4x4 *self);
	void Matrix4x4CopyConstructor(const Matrix4x4 &other, Matrix4x4 *self);
	void CollisionBoxDefaultConstructor(ETH_COLLISION_BOX *self);
	void CollisionBoxCopyConstructor(const ETH_COLLISION_BOX &other, ETH_COLLISION_BOX *self);
	void CollisionBoxInitConstructor(const Vector3 &pos, const Vector3 &size, ETH_COLLISION_BOX *self);
	void AddRef();
	void Release();
	bool CheckASError(const int r, const str_type::string &description);
	bool RegisterInputObject(asIScriptEngine *pASEngine);
	bool RegisterENMLObjetct(asIScriptEngine *pASEngine);
	bool RegisterEntityObject(asIScriptEngine *pASEngine);
	bool RegisterCollisionObject(asIScriptEngine *pASEngine);
	bool RegisterMathObjectsAndFunctions(asIScriptEngine *pASEngine);
	bool RegisterEntityArrayObject(asIScriptEngine *pASEngine);
	void RegisterEnumTypes(asIScriptEngine *pASEngine);
	void RegisterGlobalProperties(asIScriptEngine *pASEngine);
	bool RegisterVideoModeObject(asIScriptEngine *pASEngine);
	bool RegisterDateTimeObject(asIScriptEngine *pASEngine);

	void RegisterAllObjects(asIScriptEngine *pASEngine);
}

#endif