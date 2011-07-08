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

#ifndef ETH_ENTITY_CONTROLLER_H_
#define ETH_ENTITY_CONTROLLER_H_

#include "ETHASUtil.h"

#include "ETHScriptEntity.h"

using namespace gs2d;
using namespace gs2d::math;

class ETHEntityController
{
public:
	virtual void Update(const unsigned long lastFrameElapsedTime) = 0;
	virtual Vector3 GetPos() const = 0;
	virtual Vector2 GetPosXY() const = 0;
	virtual float GetAngle() const = 0;
	virtual void SetPos(const Vector3& pos) = 0;

	virtual void AddToPos(const Vector3& pos) = 0;
	virtual void AddToAngle(const float angle) = 0;

	virtual void SetAngle(const float angle) = 0;
	virtual bool HasCallback() const = 0;
	virtual bool RunCallback(ETHScriptEntity* entity) = 0;
};

typedef boost::shared_ptr<ETHEntityController> ETHEntityControllerPtr;

class ETHRawEntityController : public ETHEntityController
{
	asIScriptContext *m_pContext;
	int m_callbackID;
	Vector3 m_pos;
	float m_angle;

public:
	ETHRawEntityController(const Vector3& pos, const float angle);
	ETHRawEntityController(const ETHEntityControllerPtr& old, asIScriptContext *pContext, const int callbackID);
	void Update(const unsigned long lastFrameElapsedTime);
	Vector3 GetPos() const;
	Vector2 GetPosXY() const;
	float GetAngle() const;
	void SetPos(const Vector3& pos);
	void SetAngle(const float angle);
	bool HasCallback() const { return (m_callbackID != -1); }
	bool RunCallback(ETHScriptEntity* entity);
	void AddToPos(const Vector3& pos);
	void AddToAngle(const float angle);
};

typedef boost::shared_ptr<ETHRawEntityController> ETHRawEntityControllerPtr;

#endif