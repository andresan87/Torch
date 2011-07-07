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

#ifndef GS2D_ANDROID_INPUT_H_
#define GS2D_ANDROID_INPUT_H_

#include "../../gs2dinput.h"
#include <vector>
#include <string>
#include "../../Platform/FileLogger.h"
#include "../../Platform/Android/Platform.android.h"

namespace gs2d {

class AndroidInput : public Input
{
	const unsigned int m_maxTouchCount;

	std::vector<math::Vector2> m_currentTouch;
	std::vector<math::Vector2> m_touchMove;
	std::vector<math::Vector2> m_lastTouch;
	math::Vector3 m_accelerometer;

	std::vector<GS_KEY_STATE> m_touchState;
	std::vector<int> m_touchStepCount;
	const std::string *m_input;
	Platform::FileLogger m_logger;

public:
	AndroidInput(const unsigned int maxTouchCount, const std::string *input);

	bool IsKeyDown(const GS_KEY key) const;
	GS_KEY_STATE GetKeyState(const GS_KEY key) const;

	GS_KEY_STATE GetLeftClickState() const;
	GS_KEY_STATE GetRightClickState() const;
	GS_KEY_STATE GetMiddleClickState() const;

	math::Vector2i GetMouseMove() const;
	math::Vector2  GetMouseMoveF() const;

	bool SetCursorPosition(math::Vector2i v2Pos);
	bool SetCursorPositionF(math::Vector2 v2Pos);
	math::Vector2i GetCursorPosition(WindowPtr pWindow) const;
	math::Vector2  GetCursorPositionF(WindowPtr pWindow) const;

	unsigned int GetMaxJoysticks() const;
	float GetWheelState() const;

	bool Update();

	void ShowJoystickWarnings(const bool enable);
	bool IsShowingJoystickWarnings() const;

	math::Vector2 GetTouchPos(const unsigned int n, WindowPtr pWindow) const;
	GS_KEY_STATE  GetTouchState(const unsigned int n, WindowPtr pWindow) const;
	unsigned int GetMaxTouchCount() const;
	math::Vector2 GetTouchMove(const unsigned int n) const;

	str_type::char_t GetLastCharInput() const;

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

typedef boost::shared_ptr<AndroidInput> AndroidInputPtr;

} // namespace gs2d

#endif