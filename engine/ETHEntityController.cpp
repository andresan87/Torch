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

#include "ETHEntityController.h"
#include <iostream>

ETHRawEntityController::ETHRawEntityController(const Vector3& pos, const float angle) :
	m_pos(pos),
	m_angle(angle),
	m_pContext(0),
	m_callbackID(-1)
{
}

ETHRawEntityController::ETHRawEntityController(const ETHEntityControllerPtr& old, asIScriptContext *pContext, const int callbackID) :
	m_pContext(pContext),
	m_callbackID(callbackID)
{
	m_pos = old->GetPos();
	m_angle = old->GetAngle();
}

void ETHRawEntityController::Update(const unsigned long lastFrameElapsedTime)
{
	GS2D_UNUSED_ARGUMENT(lastFrameElapsedTime);
	// TODO/TO-DO
}

Vector3 ETHRawEntityController::GetPos() const
{
	return m_pos;
}

Vector2 ETHRawEntityController::GetPosXY() const
{
	return Vector2(m_pos.x, m_pos.y);
}

float ETHRawEntityController::GetAngle() const
{
	return m_angle;
}

void ETHRawEntityController::SetPos(const Vector3& pos)
{
	m_pos = pos;
}

void ETHRawEntityController::SetAngle(const float angle)
{
	m_angle = angle;
}

void ETHRawEntityController::AddToPos(const Vector3& pos)
{
	m_pos += pos;
}

void ETHRawEntityController::AddToAngle(const float angle)
{
	m_angle += angle;
}

bool ETHRawEntityController::RunCallback(ETHScriptEntity* entity)
{
	if (!HasCallback())
	{
		ETH_STREAM_DECL(ss) << GS_L("ETHRawEntityController::RunCallbackScript: no context to run the script");
		return false;
	}
	if (m_callbackID < 0)
	{
		ETH_STREAM_DECL(ss) << GS_L("ETHRawEntityController::RunCallbackScript: no callbacks to run");
		return false;
	}

	// prepare the context to start receiving the arguments
	if (m_pContext->Prepare(m_callbackID) < 0)
	{
		ETH_STREAM_DECL(ss) << GS_L("(RunCallbackScript) Couldn't prepare context for function ETHCallback_") << ETHGlobal::RemoveExtension(entity->GetEntityName().c_str());
		return false;
	}

	// set the argument
	if (m_pContext->SetArgObject(0, entity) >= 0)
	{
		// execute the script but do not prepare it (false)
		// because it has already been prepared to receive the arguments
		ETHGlobal::ExecuteContext(m_pContext, m_callbackID, false);
	}
	else
	{
		return false;
	}
	return true;
}