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

#include "gs2dWinInput.h"
#include "../../Video/Direct3D9/gs2dD3D9.h"

namespace gs2d {
using namespace math;

/// Instantiates an Input object
GS2D_API InputPtr CreateInput(boost::any data, const bool showJoystickWarnings)
{
	return InputPtr(new WinInput(data, showJoystickWarnings));
}

inline void CheckJoyMinimumValue(float &a)
{
	static const float joyMinimumValue = 0.01f;
	if (a > -joyMinimumValue && a < joyMinimumValue)
		a = 0.0f;
}

WinInput::WinInput(boost::any data, const bool showJoystickWarnings)
{
	ShowJoystickWarnings(showJoystickWarnings);
	m_mouseWheel = 0.0f;

	POINT point;
	GetCursorPos(&point);
	m_lastPos.x = point.x;
	m_lastPos.y = point.y;

	m_keyID[GSK_UP] = VK_UP;
	m_keyID[GSK_DOWN] = VK_DOWN;
	m_keyID[GSK_LEFT] = VK_LEFT;
	m_keyID[GSK_RIGHT] = VK_RIGHT;
	m_keyID[GSK_CTRL] = VK_CONTROL;
	m_keyID[GSK_ALT] = VK_MENU;
	m_keyID[GSK_SHIFT] = VK_SHIFT;
	m_keyID[GSK_PAGEDOWN] = VK_NEXT;
	m_keyID[GSK_PAGEUP] = VK_PRIOR;
	m_keyID[GSK_SPACE] = VK_SPACE;
	m_keyID[GSK_ENTER] = VK_RETURN;
	m_keyID[GSK_DELETE] = VK_DELETE;
	m_keyID[GSK_HOME] = VK_HOME;
	m_keyID[GSK_END] = VK_END;
	m_keyID[GSK_INSERT] = VK_INSERT;
	m_keyID[GSK_PAUSE] = VK_PAUSE;
	m_keyID[GSK_ESC] = VK_ESCAPE;
	m_keyID[GSK_BACK] = VK_BACK;
	m_keyID[GSK_TAB] = VK_TAB;
	m_keyID[GSK_PRINTSCREEN] = VK_SNAPSHOT;
	m_keyID[GSK_SUBTRACT] = VK_SUBTRACT;
	m_keyID[GSK_ADD] = VK_ADD;
	m_keyID[GSK_F1] = VK_F1;
	m_keyID[GSK_F2] = VK_F2;
	m_keyID[GSK_F3] = VK_F3;
	m_keyID[GSK_F4] = VK_F4;
	m_keyID[GSK_F5] = VK_F5;
	m_keyID[GSK_F6] = VK_F6;
	m_keyID[GSK_F7] = VK_F7;
	m_keyID[GSK_F8] = VK_F8;
	m_keyID[GSK_F9] = VK_F9;
	m_keyID[GSK_F10] = VK_F10;
	m_keyID[GSK_F11] = VK_F11;
	m_keyID[GSK_F12] = VK_F12;
	m_keyID[GSK_F13] = VK_F13;
	m_keyID[GSK_F14] = VK_F14;
	m_keyID[GSK_F15] = VK_F15;
	m_keyID[GSK_F16] = VK_F16;
	m_keyID[GSK_F17] = VK_F17;
	m_keyID[GSK_F18] = VK_F18;
	m_keyID[GSK_F19] = VK_F19;
	m_keyID[GSK_F20] = VK_F20;
	m_keyID[GSK_F21] = VK_F21;
	m_keyID[GSK_F22] = VK_F22;
	m_keyID[GSK_F23] = VK_F23;
	m_keyID[GSK_F24] = VK_F24;
	m_keyID[GSK_RMOUSE] = VK_RBUTTON;
	m_keyID[GSK_LMOUSE] = VK_LBUTTON;
	m_keyID[GSK_MMOUSE] = VK_MBUTTON;
	m_keyID[GSK_A] = (DWORD)'A';
	m_keyID[GSK_B] = (DWORD)'B';
	m_keyID[GSK_C] = (DWORD)'C';
	m_keyID[GSK_D] = (DWORD)'D';
	m_keyID[GSK_E] = (DWORD)'E';
	m_keyID[GSK_F] = (DWORD)'F';
	m_keyID[GSK_G] = (DWORD)'G';
	m_keyID[GSK_H] = (DWORD)'H';
	m_keyID[GSK_I] = (DWORD)'I';
	m_keyID[GSK_J] = (DWORD)'J';
	m_keyID[GSK_K] = (DWORD)'K';
	m_keyID[GSK_L] = (DWORD)'L';
	m_keyID[GSK_M] = (DWORD)'M';
	m_keyID[GSK_N] = (DWORD)'N';
	m_keyID[GSK_O] = (DWORD)'O';
	m_keyID[GSK_P] = (DWORD)'P';
	m_keyID[GSK_Q] = (DWORD)'Q';
	m_keyID[GSK_R] = (DWORD)'R';
	m_keyID[GSK_S] = (DWORD)'S';
	m_keyID[GSK_T] = (DWORD)'T';
	m_keyID[GSK_U] = (DWORD)'U';
	m_keyID[GSK_V] = (DWORD)'V';
	m_keyID[GSK_W] = (DWORD)'W';
	m_keyID[GSK_X] = (DWORD)'X';
	m_keyID[GSK_Y] = (DWORD)'Y';
	m_keyID[GSK_Z] = (DWORD)'Z';
	m_keyID[GSK_1] = (DWORD)'1';
	m_keyID[GSK_2] = (DWORD)'2';
	m_keyID[GSK_3] = (DWORD)'3';
	m_keyID[GSK_4] = (DWORD)'4';
	m_keyID[GSK_5] = (DWORD)'5';
	m_keyID[GSK_6] = (DWORD)'6';
	m_keyID[GSK_7] = (DWORD)'7';
	m_keyID[GSK_8] = (DWORD)'8';
	m_keyID[GSK_9] = (DWORD)'9';
	m_keyID[GSK_0] = (DWORD)'0';
	m_keyID[GSK_MINUS] = 0xBD;
	m_keyID[GSK_PLUS] = 0xBB;
	m_keyID[GSK_COMMA] = 0xBC;
	m_keyID[GSK_DOT] = 0xBE;
	m_keyID[GSK_NUMPAD0] = VK_NUMPAD0;
	m_keyID[GSK_NUMPAD1] = VK_NUMPAD1;
	m_keyID[GSK_NUMPAD2] = VK_NUMPAD2;
	m_keyID[GSK_NUMPAD3] = VK_NUMPAD3;
	m_keyID[GSK_NUMPAD4] = VK_NUMPAD4;
	m_keyID[GSK_NUMPAD5] = VK_NUMPAD5;
	m_keyID[GSK_NUMPAD6] = VK_NUMPAD6;
	m_keyID[GSK_NUMPAD7] = VK_NUMPAD7;
	m_keyID[GSK_NUMPAD8] = VK_NUMPAD8;
	m_keyID[GSK_NUMPAD9] = VK_NUMPAD9;

	for (int t=0; t<GS_NUM_KEYS; t++)
	{
		m_count[t] = 0;
	}
	m_newPos = false;

	DetectJoysticks();
	Update();
}

bool WinInput::IsKeyDown(const GS_KEY key) const
{
	const GS_KEY_STATE state = GetKeyState(key);
	return (state == GSKS_DOWN || state == GSKS_HIT);
}

Vector2 WinInput::GetMouseMoveF() const
{
	Vector2 r;
	Vector2i ir = GetMouseMove();
	r.x = static_cast<float>(ir.x);
	r.y = static_cast<float>(ir.y);
	return r;
}

float WinInput::GetWheelState() const
{
	return m_mouseWheel;
}

wchar_t WinInput::GetLastCharInput() const
{
	return m_charInput;
}

GS_JOYSTICK_STATUS GetJoystickError(const MMRESULT mmr, const bool showJoystickWarnings)
{
	GS_JOYSTICK_STATUS r;
	switch (mmr)
	{
	case JOYERR_NOERROR:
		r = GSJS_DETECTED;
		break;
	case JOYERR_UNPLUGGED:
		r = GSJS_NOTDETECTED;
		if (showJoystickWarnings)
			ShowMessage(L"joystick is unplugged - WinInput::DetectJoysticks", GSMT_WARNING);
		break;
	case MMSYSERR_NODRIVER:
		r = GSJS_INVALID;
		if (showJoystickWarnings)
			ShowMessage(L"joystick driver couldn't be found - WinInput::DetectJoysticks", GSMT_WARNING);
		break;
	case MMSYSERR_INVALPARAM:
		r = GSJS_INVALID;
		if (showJoystickWarnings)
			ShowMessage(L"joystick couldn't be detected - WinInput::DetectJoysticks", GSMT_WARNING);
		break;
	case MMSYSERR_BADDEVICEID:
		r = GSJS_INVALID;
		if (showJoystickWarnings)
			ShowMessage(L"joystick couldn't be detected (bad device) - WinInput::DetectJoysticks", GSMT_WARNING);
		break;
	case JOYERR_PARMS:
		r = GSJS_INVALID;
		if (showJoystickWarnings)
			ShowMessage(L"WARNING: joystick couldn't be detected - WinInput::DetectJoysticks", GSMT_WARNING);
		break;
	default:
		r = GSJS_INVALID;
		if (showJoystickWarnings)
			ShowMessage(L"Joystick unknown error - WinInput::DetectJoysticks", GSMT_WARNING);
		break;
	};
	return r;
}

void WinInput::ShowJoystickWarnings(const bool enable)
{
	m_showJoyWarnings = enable;
}

bool WinInput::IsShowingJoystickWarnings() const
{
	return m_showJoyWarnings;
}

unsigned int WinInput::GetNumJoysticks() const
{
	return m_nJoysticks;
}

UINT GetWinJoystick(const unsigned int id)
{
	UINT uID;
	switch (id)
	{
	case 0:
		uID = JOYSTICKID1;
		break;
	case 1:
		uID = JOYSTICKID2;
		break;
	case 2:
		uID = 2;
		break;
	case 3:
		uID = 3;
		break;
	default:
		uID = JOYSTICKID1;
	};
	return uID;
}

bool WinInput::DetectJoysticks()
{
	m_nJoysticks = static_cast<unsigned int>(joyGetNumDevs());
	m_nJoysticks = Min(m_nJoysticks, MAX_JOYSTICKS);
	
	if (m_nJoysticks == 0)
	{
		if (IsShowingJoystickWarnings())
			ShowMessage(L"Joysticks are not supported - WinInput::DetectJoysticks", GSMT_WARNING);
		return false;
	}

	for (unsigned int t=0; t<m_nJoysticks; t++)
	{
		m_joystick[t].buttonID[GSB_01] = 1;
		m_joystick[t].buttonID[GSB_02] = 2;
		m_joystick[t].buttonID[GSB_03] = 4;
		m_joystick[t].buttonID[GSB_04] = 8;
		m_joystick[t].buttonID[GSB_05] = 16;
		m_joystick[t].buttonID[GSB_06] = 32;
		m_joystick[t].buttonID[GSB_07] = 64;
		m_joystick[t].buttonID[GSB_08] = 128;
		m_joystick[t].buttonID[GSB_09] = 256;
		m_joystick[t].buttonID[GSB_10] = 512;
		m_joystick[t].buttonID[GSB_11] = 1024;
		m_joystick[t].buttonID[GSB_12] = 2048;
		m_joystick[t].buttonID[GSB_13] = 4096;
		m_joystick[t].buttonID[GSB_14] = 8192;
		m_joystick[t].buttonID[GSB_15] = 16384;
		m_joystick[t].buttonID[GSB_16] = 32768;
		m_joystick[t].buttonID[GSB_17] = 65536;
		m_joystick[t].buttonID[GSB_18] = 131072;
		m_joystick[t].buttonID[GSB_19] = 262144;
		m_joystick[t].buttonID[GSB_20] = JOY_BUTTON20;
		m_joystick[t].buttonID[GSB_21] = JOY_BUTTON21;
		m_joystick[t].buttonID[GSB_22] = JOY_BUTTON22;
		m_joystick[t].buttonID[GSB_23] = JOY_BUTTON23;
		m_joystick[t].buttonID[GSB_24] = JOY_BUTTON24;
		m_joystick[t].buttonID[GSB_25] = JOY_BUTTON25;
		m_joystick[t].buttonID[GSB_26] = JOY_BUTTON26;
		m_joystick[t].buttonID[GSB_27] = JOY_BUTTON27;
		m_joystick[t].buttonID[GSB_28] = JOY_BUTTON28;
		m_joystick[t].buttonID[GSB_29] = JOY_BUTTON29;
		m_joystick[t].buttonID[GSB_30] = JOY_BUTTON30;
		m_joystick[t].buttonID[GSB_31] = JOY_BUTTON31;
		m_joystick[t].buttonID[GSB_32] = JOY_BUTTON32;

		const UINT id = GetWinJoystick(t);
		JOYINFOEX joyInfoEx;
		ZeroMemory(&joyInfoEx, sizeof(JOYINFOEX));
		joyInfoEx.dwSize = sizeof(JOYINFOEX);
		MMRESULT mmr = joyGetPosEx(id, &joyInfoEx);

		m_joystick[t].status = GetJoystickError(mmr, IsShowingJoystickWarnings());

		if (m_joystick[t].status == GSJS_DETECTED)
		{
			joyGetDevCaps(id, &m_joyCaps[t], sizeof(JOYCAPS));
			m_joystick[t].nButtons = m_joyCaps[t].wNumButtons;
		}
	}
	return true;
}

Vector2i WinInput::GetCursorPosition(WindowPtr pWindow) const
{
	POINT point;
	GetCursorPos(&point);
	Vector2i r(point.x, point.y);
	if (pWindow)
	{
		r = pWindow->ScreenToWindow(r);
	}
	return r;
}

Vector2 WinInput::GetCursorPositionF(WindowPtr pWindow) const
{
	Vector2i v2 = GetCursorPosition(pWindow);
	return Vector2(static_cast<float>(v2.x), static_cast<float>(v2.y));
}

unsigned int WinInput::GetMaxJoysticks() const
{
	return MAX_JOYSTICKS;
}

GS_KEY_STATE WinInput::GetJoystickButtonState(const unsigned int id, const GS_JOYSTICK_BUTTON key) const
{
	if (id >= MAX_JOYSTICKS)
	{
		ShowMessage(L"Invalid joystick id - WinInput::JoyButtonState");
		return GSKS_UP;
	}

	if (m_joystick[id].status != GSJS_DETECTED)
	{
		if (IsShowingJoystickWarnings())
			ShowMessage(L"The joystick hasn't been detected - WinInput::JoyButtonState");
		return GSKS_UP;
	}
	if (key >= m_joystick[id].nButtons && key < GSB_UP)
	{
		if (IsShowingJoystickWarnings())
			ShowMessage(L"Invalid button. Make sure that the joypad has enough buttons - WinInput::JoyButtonState");
		return GSKS_UP;
	}
	return m_joystick[id].state[key];
}

bool WinInput::IsJoystickButtonDown(const unsigned int id, const GS_JOYSTICK_BUTTON key) const
{
	const GS_KEY_STATE state = GetJoystickButtonState(id, key);
	return (state == GSKS_DOWN || state == GSKS_HIT);
}

GS_KEY_STATE WinInput::GetKeyState(const GS_KEY key) const
{
	return m_keyState[key];
}

GS_KEY_STATE WinInput::GetLeftClickState() const
{
	return m_keyState[GSK_LMOUSE];
}

GS_KEY_STATE WinInput::GetRightClickState() const
{
	return m_keyState[GSK_RMOUSE];
}

GS_KEY_STATE WinInput::GetMiddleClickState() const
{
	return m_keyState[GSK_MMOUSE];
}

// emulates the screen touch with the left mouse click
math::Vector2 WinInput::GetTouchPos(const unsigned int n, WindowPtr pWindow) const
{
	if (n == 0 && IsKeyDown(GSK_LMOUSE))
	{
		return GetCursorPositionF(pWindow);
	}
	else
	{
		return GS_NO_TOUCH;
	}
}

// emulates the screen touch with the left mouse click
GS_KEY_STATE WinInput::GetTouchState(const unsigned int n, WindowPtr pWindow) const
{
	if (n == 0)
		return m_keyState[GSK_LMOUSE];
	else
		return GSKS_UP;
}

unsigned int WinInput::GetMaxTouchCount() const
{
	return 1;
}

Vector2i WinInput::GetMouseMove() const
{
	return m_mouseMove;
}

Vector2 WinInput::GetTouchMove(const unsigned int n) const
{
	GS2D_UNUSED_ARGUMENT(n);
	if (IsKeyDown(GSK_LMOUSE))
		return GetMouseMoveF();
	else
		return Vector2(0, 0);
}

bool WinInput::SetCursorPosition(Vector2i v2Pos)
{
	m_newPos = true;
	m_v2NewPos = v2Pos;
	return true;
}

bool WinInput::SetCursorPositionF(Vector2 v2Pos)
{
	m_newPos = true;
	m_v2NewPos = Vector2i(static_cast<int>(v2Pos.x), static_cast<int>(v2Pos.y));
	return true;
}

bool WinInput::Update()
{
	for (unsigned int t=0; t<GS_NUM_KEYS; t++)
	{
		m_keyState[t] = UpdateKeyState((GS_KEY)t);
	}

	POINT point;
	GetCursorPos(&point);

	m_mouseMove.x = point.x-m_lastPos.x;
	m_mouseMove.y = point.y-m_lastPos.y;

	m_lastPos.x = point.x;
	m_lastPos.y = point.y;

	if (m_newPos)
	{
		SetCursorPos(m_v2NewPos.x, m_v2NewPos.y);
		m_lastPos = m_v2NewPos;
		m_newPos = false;
	}

	m_mouseWheel = static_cast<float>(D3D9Video::m_wheelDelta)/120.0f;
	m_charInput = static_cast<wchar_t>(D3D9Video::m_currentChar);
	D3D9Video::m_wheelDelta = 0;
	D3D9Video::m_currentChar = '\0';

	UpdateJoystick();
	return true;
}

Vector2 WinInput::GetJoystickXY(const unsigned int id) const
{
	if (id >= MAX_JOYSTICKS)
	{
		ShowMessage(L"Invalid joystick id - WinInput::GetJoystickXY");
		return Vector2(0,0);
	}

	if (m_joystick[id].status != GSJS_DETECTED)
	{
		if (IsShowingJoystickWarnings())
			ShowMessage(L"The joystick hasn't been detected - WinInput::GetJoystickXY");
		return Vector2(0.0f,0.0f);
	}
	return m_joystick[id].xy;
}

float WinInput::GetJoystickZ(const unsigned int id) const
{
	if (id >= MAX_JOYSTICKS)
	{
		ShowMessage(L"Invalid joystick id - WinInput::GetJoystickXY");
		return 0.0f;
	}

	if (m_joystick[id].status != GSJS_DETECTED)
	{
		if (IsShowingJoystickWarnings())
			ShowMessage(L"The joystick hasn't been detected - WinInput::GetJoystickZ");
		return 0.0f;
	}
	return m_joystick[id].z;
}


float WinInput::GetJoystickRudder(const unsigned int id) const
{
	if (id >= MAX_JOYSTICKS)
	{
		ShowMessage(L"Invalid joystick id - WinInput::GetJoystickRudder");
		return 0.0f;
	}

	if (m_joystick[id].status != GSJS_DETECTED)
	{
		if (IsShowingJoystickWarnings())
			ShowMessage(L"The joystick hasn't been detected - WinInput::GetJoystickRudder");
		return 0.0f;
	}
	return m_joystick[id].rudder;
}

Vector2 WinInput::GetJoystickUV(const unsigned int id) const
{
	if (id >= MAX_JOYSTICKS)
	{
		ShowMessage(L"Invalid joystick id - WinInput::GetJoystickUV");
		return Vector2(0,0);
	}

	if (m_joystick[id].status != GSJS_DETECTED)
	{
		if (IsShowingJoystickWarnings())
			ShowMessage(L"The joystick hasn't been detected - WinInput::GetJoystickUV");
		return Vector2(0.0f,0.0f);
	}
	return m_joystick[id].uv;
}

GS_JOYSTICK_STATUS WinInput::GetJoystickStatus(const unsigned int id) const
{
	if (id >= MAX_JOYSTICKS)
	{
		ShowMessage(L"Invalid joystick id - WinInput::GetJoystickStatus");
		return GSJS_INVALID;
	}
	return m_joystick[id].status;
}

unsigned int WinInput::GetNumJoyButtons(const unsigned int id) const
{
	if (id >= MAX_JOYSTICKS)
	{
		ShowMessage(L"Invalid joystick id - WinInput::GetNumJoyButtons");
		return 0;
	}

	if (m_joystick[id].status != GSJS_DETECTED)
	{
		if (IsShowingJoystickWarnings())
			ShowMessage(L"The joystick hasn't been detected - WinInput::GetNumJoyButtons");
		return 0;
	}
	return m_joystick[id].nButtons;
}

GS_JOYSTICK_BUTTON WinInput::GetFirstButtonDown(const unsigned int id) const
{
	if (id >= MAX_JOYSTICKS)
	{
		ShowMessage(L"Invalid joystick id - WinInput::GetFirstButtonDown");
		return GSB_NONE;
	}

	if (m_joystick[id].status != GSJS_DETECTED)
	{
		if (IsShowingJoystickWarnings())
			ShowMessage(L"The joystick is not detected - WinInput::GetFirstButtonDown");
		return GSB_NONE;
	}
	const unsigned int nButtons = static_cast<unsigned int>(m_joystick[id].nButtons);
	for (unsigned int t=0; t<nButtons; t++)
	{
		if (m_joystick[id].state[t] == GSKS_HIT || m_joystick[id].state[t] == GSKS_DOWN)
			return (GS_JOYSTICK_BUTTON)t;
	}
	return GSB_NONE;
}

bool WinInput::UpdateJoystick()
{
	for (unsigned int j=0; j<m_nJoysticks; j++)
	{
		const UINT id = GetWinJoystick(j);
		if (m_joystick[j].status == GSJS_DETECTED)
		{
			ZeroMemory(&m_joyInfoEx[j], sizeof(JOYINFOEX));
			m_joyInfoEx[j].dwSize = sizeof(JOYINFOEX);
			m_joyInfoEx[j].dwFlags = JOY_RETURNALL;

			MMRESULT mmr = joyGetPosEx(id, &m_joyInfoEx[j]);
			m_joystick[j].status = GetJoystickError(mmr, IsShowingJoystickWarnings());
			if (m_joystick[j].status != GSJS_DETECTED)
			{
				if (IsShowingJoystickWarnings())
					ShowMessage(L"The joystick has been unplugged - WinInput::UpdateJoystick");
				continue;
			}

			for (unsigned int t=0; t<GSB_MAX_BUTTONS; t++)
			{
				const unsigned int nButtons = static_cast<unsigned int>(m_joystick[j].nButtons);
				if (t<nButtons)
				{
					m_joystick[j].state[t] = UpdateJoyButton(j, (GS_JOYSTICK_BUTTON)t);
				}
				else
				{
					m_joystick[j].state[t] = GSKS_UP;
					m_joystick[j].count[t] = 0;
				}
			}

			// update X and Y axis
			const long strideX = (m_joyCaps[j].wXmax-m_joyCaps[j].wXmin)/2;
			const long strideY = (m_joyCaps[j].wYmax-m_joyCaps[j].wYmin)/2;
			m_joystick[j].xy.x = (float)((long)m_joyInfoEx[j].dwXpos-strideX)/(float)strideX;
			m_joystick[j].xy.y = (float)((long)m_joyInfoEx[j].dwYpos-strideY)/(float)strideY;
			m_joystick[j].xy.x = Min(Max(m_joystick[j].xy.x, -1.0f), 1.0f);
			m_joystick[j].xy.y = Min(Max(m_joystick[j].xy.y, -1.0f), 1.0f);
			CheckJoyMinimumValue(m_joystick[j].xy.x);
			CheckJoyMinimumValue(m_joystick[j].xy.y);

			// update Z axis
			const DWORD strideZ = (m_joyCaps[j].wZmax-m_joyCaps[j].wZmin)/2;
			m_joystick[j].z = (float)((long)m_joyInfoEx[j].dwZpos-(long)strideZ)/(float)strideZ;
			m_joystick[j].z = Min(Max(m_joystick[j].z, -1.0f), 1.0f);
			CheckJoyMinimumValue(m_joystick[j].z);

			// update Rudder axis
			const DWORD strideR = (m_joyCaps[j].wRmax-m_joyCaps[j].wRmin)/2;
			m_joystick[j].rudder = (float)((long)m_joyInfoEx[j].dwRpos-(long)strideR)/(float)strideR;
			m_joystick[j].rudder = Min(Max(m_joystick[j].rudder, -1.0f), 1.0f);
			CheckJoyMinimumValue(m_joystick[j].rudder);

			// update U and V axis
			const DWORD strideU = (m_joyCaps[j].wUmax-m_joyCaps[j].wUmin)/2;
			const DWORD strideV = (m_joyCaps[j].wVmax-m_joyCaps[j].wVmin)/2;
			m_joystick[j].uv.x = (float)((long)m_joyInfoEx[j].dwUpos-(long)strideU)/(float)strideU;
			m_joystick[j].uv.y = (float)((long)m_joyInfoEx[j].dwVpos-(long)strideV)/(float)strideV;
			m_joystick[j].uv.x = Min(Max(m_joystick[j].uv.x, -1.0f), 1.0f);
			m_joystick[j].uv.y = Min(Max(m_joystick[j].uv.y, -1.0f), 1.0f);
			CheckJoyMinimumValue(m_joystick[j].uv.x);
			CheckJoyMinimumValue(m_joystick[j].uv.y);

			m_joystick[j].state[GSB_LEFT ] = UpdateJoyArrowHit(j, GSB_LEFT, m_joystick[j].xy.x);
			m_joystick[j].state[GSB_RIGHT] = UpdateJoyArrowHit(j, GSB_RIGHT, m_joystick[j].xy.x);
			m_joystick[j].state[GSB_UP   ] = UpdateJoyArrowHit(j, GSB_UP, m_joystick[j].xy.y);
			m_joystick[j].state[GSB_DOWN ] = UpdateJoyArrowHit(j, GSB_DOWN, m_joystick[j].xy.y);
		}
	}
	return true;
}

GS_KEY_STATE WinInput::UpdateJoyButton(const unsigned int id, const GS_JOYSTICK_BUTTON key)
{
	if (m_joyInfoEx[id].dwButtons & m_joystick[id].buttonID[key])
	{
		m_joystick[id].count[key]++;
	}
	else if (m_joystick[id].count[key] > 0)
	{
		m_joystick[id].count[key] = 0;
		return GSKS_RELEASE;
	}
	else if (m_joystick[id].count[key] == 0)
		return GSKS_UP;

	if (m_joystick[id].count[key] == 1)
		return GSKS_HIT;

	return GSKS_DOWN;
}

GS_KEY_STATE WinInput::UpdateJoyArrowHit(const unsigned int id, const GS_JOYSTICK_BUTTON key, const float direction)
{
	static const float minArrowValue = 0.8f;
	if (((key==GSB_DOWN||key==GSB_RIGHT) && direction >= minArrowValue) ||
		((key==GSB_UP  ||key==GSB_LEFT ) && direction <=-minArrowValue))
	{
		m_joystick[id].count[key]++;
	}
	else if (m_joystick[id].count[key] > 0)
	{
		m_joystick[id].count[key] = 0;
		return GSKS_RELEASE;
	}
	else if (m_joystick[id].count[key] == 0)
		return GSKS_UP;

	if (m_joystick[id].count[key] == 1)
		return GSKS_HIT;

	return GSKS_DOWN;
}

GS_KEY_STATE WinInput::UpdateKeyState(const GS_KEY key)
{
	if (!D3D9Video::m_inputFocus)
	{
		m_count[key] = 0;
		return GSKS_UP;
	}

	if (::GetKeyState(m_keyID[key]) & 0x80)
	{
		m_count[key]++;
	}
	else if (m_count[key] > 0)
	{
		m_count[key] = 0;
		return GSKS_RELEASE;
	}
	else if (m_count[key] == 0)
		return GSKS_UP;

	if (m_count[key] == 1)
		return GSKS_HIT;

	return GSKS_DOWN;
}

Vector3 WinInput::GetAccelerometerData() const
{
	// TODO/TO-DO: emulate
	return Vector3(0, 9.8f, 0);
}

} // namespace gs2d