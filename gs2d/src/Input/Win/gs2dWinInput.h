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

#ifndef GS2D_WIN_INPUT_H_
#define GS2D_WIN_INPUT_H_

#include "../../gs2dinput.h"
#include "../../gs2d.h"
#include <windows.h>
#pragma warning( push )
#pragma warning( disable : 4201 )
#include <mmsystem.h>
#pragma warning( pop )

namespace gs2d {

struct JOYSTICK_DATA
{
	JOYSTICK_DATA()
	{
		status = GSJS_NOTDETECTED;
		z = rudder = 0.0f;
		for (unsigned int t=0; t<GSB_NUM_BUTTONS; t++)
		{
			buttonID[t] = 0x0;
			state[t] = GSKS_UP;
			count[t] = 0;
		}
		nButtons = 0;
	}

	GS_JOYSTICK_STATUS status;
	math::Vector2 xy;
	float z;
	float rudder;
	math::Vector2 uv;
	DWORD buttonID[GSB_NUM_BUTTONS];
	GS_KEY_STATE state[GSB_NUM_BUTTONS];
	int count[GSB_NUM_BUTTONS];
	int nButtons;
};

class WinInput : public Input
{
	friend InputPtr CreateInput(boost::any data, const bool showJoystickWarnings);
	WinInput(boost::any data, const bool showJoystickWarnings);
	const static unsigned int MAX_JOYSTICKS = 4;
	bool UpdateJoystick();
	JOYSTICK_DATA m_joystick[MAX_JOYSTICKS];
	GS_KEY_STATE UpdateJoyButton(const unsigned int id, const GS_JOYSTICK_BUTTON key);
	GS_KEY_STATE UpdateJoyArrowHit(const unsigned int id, const GS_JOYSTICK_BUTTON key, const float direction);

	GS_KEY_STATE UpdateKeyState(const GS_KEY key);

	GS_KEY_STATE m_keyState[GS_NUM_KEYS];
	int m_count[GS_NUM_KEYS];
	math::Vector2i m_mouseMove;
	math::Vector2i m_lastPos;
	math::Vector2i m_v2NewPos;
	bool m_newPos;
	unsigned int m_nJoysticks;
	bool m_showJoyWarnings;
	int m_keyID[GS_NUM_KEYS];

	JOYCAPS m_joyCaps[MAX_JOYSTICKS];
	JOYINFOEX m_joyInfoEx[MAX_JOYSTICKS];
	float m_mouseWheel;
	wchar_t m_charInput;

public:
	bool IsKeyDown(const GS_KEY key) const;
	GS_KEY_STATE GetKeyState(const GS_KEY key) const;

	GS_KEY_STATE GetLeftClickState() const;
	GS_KEY_STATE GetRightClickState() const;
	GS_KEY_STATE GetMiddleClickState() const;

	math::Vector2i GetMouseMove() const;
	math::Vector2 GetMouseMoveF() const;

	math::Vector2 GetTouchPos(const unsigned int n, WindowPtr pWindow) const;
	GS_KEY_STATE  GetTouchState(const unsigned int n, WindowPtr pWindow) const;
	unsigned int GetMaxTouchCount() const;
	math::Vector2 GetTouchMove(const unsigned int n) const;

	bool SetCursorPosition(math::Vector2i v2Pos);
	bool SetCursorPositionF(math::Vector2 v2Pos);
	math::Vector2i GetCursorPosition(WindowPtr pWindow) const;
	math::Vector2  GetCursorPositionF(WindowPtr pWindow) const;

	float GetWheelState() const;

	bool Update();

	void ShowJoystickWarnings(const bool enable);
	bool IsShowingJoystickWarnings() const;

	wchar_t GetLastCharInput() const;

	unsigned int GetMaxJoysticks() const;
	GS_KEY_STATE GetJoystickButtonState(const unsigned int id, const GS_JOYSTICK_BUTTON key) const;
	bool IsJoystickButtonDown(const unsigned int id, const GS_JOYSTICK_BUTTON key) const;
	bool DetectJoysticks();
	GS_JOYSTICK_STATUS GetJoystickStatus(const unsigned int id) const;
	unsigned int GetNumJoyButtons(const unsigned int id) const;
	math::Vector2 GetJoystickXY(const unsigned int id) const;
	float GetJoystickZ(const unsigned int id) const;
	float GetJoystickRudder(const unsigned int id) const;
	math::Vector2 GetJoystickUV(const unsigned int id) const;
	GS_JOYSTICK_BUTTON GetFirstButtonDown(const unsigned int id) const;
	unsigned int GetNumJoysticks() const;

	math::Vector3 GetAccelerometerData() const;
};

typedef boost::shared_ptr<WinInput> WinInputPtr;
typedef boost::weak_ptr<WinInput> WinInputWeakPtr;

} // namespace gs2d

#endif