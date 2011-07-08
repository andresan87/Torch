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

#include "ETHInput.h"

void ETHInput::SetProvider(ETHResourceProviderPtr provider)
{
	m_provider = (provider);
}

GS_KEY_STATE ETHInput::GetLeftClickState()
{
	return m_provider->GetInput()->GetLeftClickState();
}

GS_KEY_STATE ETHInput::GetRightClickState()
{
	return m_provider->GetInput()->GetRightClickState();
}

GS_KEY_STATE ETHInput::GetMiddleClickState()
{
	return m_provider->GetInput()->GetMiddleClickState();
}

Vector2 ETHInput::GetMouseMove()
{
	return m_provider->GetInput()->GetMouseMoveF();
}

float ETHInput::GetWheelState()
{
	return m_provider->GetInput()->GetWheelState();
}

GS_KEY_STATE ETHInput::GetKeyState(const GS_KEY key)
{
	return m_provider->GetInput()->GetKeyState(key);
}

bool ETHInput::KeyDown(const GS_KEY key)
{
	return m_provider->GetInput()->IsKeyDown(key);
}


GS_KEY_STATE ETHInput::JoyButtonState(const unsigned int j, const GS_JOYSTICK_BUTTON button) const
{
	return m_provider->GetInput()->GetJoystickButtonState(j, button);
}

bool ETHInput::JoyButtonDown(const unsigned int j, const GS_JOYSTICK_BUTTON button) const
{
	return m_provider->GetInput()->IsJoystickButtonDown(j, button);
}

bool ETHInput::DetectJoysticks() const
{
	return m_provider->GetInput()->DetectJoysticks();
}

GS_JOYSTICK_STATUS ETHInput::GetJoystickStatus(const unsigned int j) const
{
	return m_provider->GetInput()->GetJoystickStatus(j);
}

unsigned int ETHInput::GetNumJoyButtons(const unsigned int j) const
{
	return m_provider->GetInput()->GetNumJoyButtons(j);
}

Vector2 ETHInput::GetJoystickXY(const unsigned int j) const
{
	return m_provider->GetInput()->GetJoystickXY(j);
}

float ETHInput::GetJoystickZ(const unsigned int j) const
{
	return m_provider->GetInput()->GetJoystickZ(j);
}

float ETHInput::GetJoystickRudder(const unsigned int j) const
{
	return m_provider->GetInput()->GetJoystickRudder(j);
}

Vector2 ETHInput::GetJoystickUV(const unsigned int j) const
{
	return m_provider->GetInput()->GetJoystickUV(j);
}

GS_JOYSTICK_BUTTON ETHInput::GetFirstButtonDown(const unsigned int j) const
{
	return m_provider->GetInput()->GetFirstButtonDown(j);
}

unsigned int ETHInput::GetMaxJoysticks() const
{
	return m_provider->GetInput()->GetMaxJoysticks();
}
