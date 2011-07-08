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

#ifndef ETH_INPUT_H_
#define ETH_INPUT_H_

#include "../gs2d/src/gs2dinput.h"
#include "ETHResourceProvider.h"
using namespace gs2d::math;
using namespace gs2d;

/// InputPtr object wrapper
class ETHInput
{
	ETHResourceProviderPtr m_provider;

public:
	void SetProvider(ETHResourceProviderPtr provider);

	GS_KEY_STATE GetLeftClickState();
	GS_KEY_STATE GetRightClickState();
	GS_KEY_STATE GetMiddleClickState();
	Vector2 GetMouseMove();
	float GetWheelState();
	GS_KEY_STATE GetKeyState(const GS_KEY);
	bool KeyDown(const GS_KEY);

	GS_KEY_STATE JoyButtonState(const unsigned int, const GS_JOYSTICK_BUTTON) const;
	bool JoyButtonDown(const unsigned int, const GS_JOYSTICK_BUTTON) const;
	bool DetectJoysticks() const;
	GS_JOYSTICK_STATUS GetJoystickStatus(const unsigned int) const;
	unsigned int GetNumJoyButtons(const unsigned int) const;
	Vector2 GetJoystickXY(const unsigned int) const;
	float GetJoystickZ(const unsigned int) const;
	float GetJoystickRudder(const unsigned int) const;
	Vector2 GetJoystickUV(const unsigned int) const;
	GS_JOYSTICK_BUTTON GetFirstButtonDown(const unsigned int) const;
	unsigned int GetMaxJoysticks() const;
};
#endif