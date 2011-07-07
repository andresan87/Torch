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

#include "gs2dAndroidInput.h"

namespace gs2d {

using namespace math;

GS2D_API InputPtr CreateInput(boost::any data, const bool showJoystickWarnings)
{
	std::string *str;
	try
	{
		str = boost::any_cast<std::string*>(data);
	}
	catch(const boost::bad_any_cast &)
	{
		return InputPtr();
	}
	return InputPtr(new AndroidInput(5, str));
}

AndroidInput::AndroidInput(const unsigned int maxTouchCount, const std::string *input) :
	m_maxTouchCount(maxTouchCount),
	m_input(input),
	m_currentTouch(maxTouchCount),
	m_touchState(maxTouchCount),
	m_touchStepCount(maxTouchCount),
	m_touchMove(maxTouchCount),
	m_lastTouch(maxTouchCount),
	m_logger(Platform::FileLogger::GetLogPath() + "AndroidInput.log.txt")
{
	for (unsigned int t=0; t<m_maxTouchCount; t++)
	{
		m_touchMove[t] = Vector2(0, 0);
		m_lastTouch[t] = GS_NO_TOUCH;
		m_currentTouch[t] = GS_NO_TOUCH;
		m_touchState[t] = GSKS_UP;
		m_touchStepCount[t] = 0;
	}
}

bool AndroidInput::IsKeyDown(const GS_KEY key) const
{
	// TODO
	return false;
}

GS_KEY_STATE AndroidInput::GetKeyState(const GS_KEY key) const
{
	// TODO
	return GSKS_UP;
}

GS_KEY_STATE AndroidInput::GetLeftClickState() const
{
	return m_touchState[0];
}

GS_KEY_STATE AndroidInput::GetRightClickState() const
{
	// TODO
	return GSKS_UP;
}

GS_KEY_STATE AndroidInput::GetMiddleClickState() const
{
	// TODO
	return GSKS_UP;
}

math::Vector2i AndroidInput::GetMouseMove() const
{
	// TODO
	return Vector2i();
}

math::Vector2  AndroidInput::GetMouseMoveF() const
{
	// TODO
	return Vector2();
}

bool AndroidInput::SetCursorPosition(math::Vector2i v2Pos)
{
	// TODO
	return false;
}

bool AndroidInput::SetCursorPositionF(math::Vector2 v2Pos)
{
	// TODO
	return false;
}

math::Vector2i AndroidInput::GetCursorPosition(WindowPtr pWindow) const
{
	return Vector2i(static_cast<int>(m_currentTouch[0].x), static_cast<int>(m_currentTouch[0].y));
}

math::Vector2  AndroidInput::GetCursorPositionF(WindowPtr pWindow) const
{
	return m_currentTouch[0];
}

unsigned int AndroidInput::GetMaxJoysticks() const
{
	// TODO
	return 0;
}

float AndroidInput::GetWheelState() const
{
	// TODO
	return 0.0f;
}

double ReadValue(std::stringstream& ss)
{
	double r;
	ss >> r;
	return r;
}

unsigned int ReadValuePairs(std::stringstream& stream, std::vector<Vector2>& out, unsigned int maxPairs)
{
	unsigned int numPairs = Min(static_cast<unsigned int>(ReadValue(stream)), maxPairs);
	for (unsigned int t=0; t<maxPairs; t++)
	{
		if (!stream.eof() && t < numPairs)
		{
			out[t].x = static_cast<float>(ReadValue(stream));
			out[t].y = static_cast<float>(ReadValue(stream));
		}
		else
		{
			out[t] = GS_NO_TOUCH;
		}
	}
	return numPairs;
}

bool AndroidInput::Update()
{
	std::stringstream stream;
	stream << *m_input;
	ReadValuePairs(stream, m_currentTouch, m_maxTouchCount);
	m_accelerometer.x = static_cast<float>(ReadValue(stream));
	m_accelerometer.y = static_cast<float>(ReadValue(stream));
	m_accelerometer.z = static_cast<float>(ReadValue(stream));

	for (unsigned int t = 0; t < m_maxTouchCount; t++)
	{
		m_touchMove[t] = Vector2(0, 0);
		if (m_currentTouch[t] != GS_NO_TOUCH)
		{
			m_touchStepCount[t]++;
			if (m_touchStepCount[t] == 1)
			{
				m_lastTouch[t] = m_currentTouch[t];
				m_touchState[t] = GSKS_HIT;
			}
			else
			{
				m_touchState[t] = GSKS_DOWN;
			}

			if (m_touchState[t] == GSKS_DOWN)
			{
				m_touchMove[t] = m_currentTouch[t] - m_lastTouch[t];
				m_lastTouch[t] = m_currentTouch[t];
			}
		}
		else
		{
			if (m_touchStepCount[t] != 0)
			{
				m_touchState[t] = GSKS_RELEASE;
			}
			else
			{
				m_touchState[t] = GSKS_UP;
			}
			m_touchStepCount[t] = 0;
		}
	}
	return true;
}

void AndroidInput::ShowJoystickWarnings(const bool enable)
{
	// TODO
}

bool AndroidInput::IsShowingJoystickWarnings() const
{
	// TODO
	return false;
}

math::Vector2 AndroidInput::GetTouchPos(const unsigned int n, WindowPtr pWindow) const
{
	if (n < m_maxTouchCount)
	{
		return m_currentTouch[n];
	}
	else
	{
		return GS_NO_TOUCH;
	}
}

GS_KEY_STATE AndroidInput::GetTouchState(const unsigned int n, WindowPtr pWindow) const
{
	if (n < m_maxTouchCount)
	{
		return m_touchState[n];
	}
	else
	{
		return GSKS_UP;
	}
}

Vector2 AndroidInput::GetTouchMove(const unsigned int n) const
{
	if (n < m_maxTouchCount)
	{
		return m_touchMove[n];
	}
	else
	{
		return Vector2(0, 0);
	}
}

unsigned int AndroidInput::GetMaxTouchCount() const
{
	return m_maxTouchCount;
}

str_type::char_t AndroidInput::GetLastCharInput() const
{
	// TODO
	return '\0';
}

GS_KEY_STATE AndroidInput::GetJoystickButtonState(const unsigned int id, const GS_JOYSTICK_BUTTON key) const
{
	// TODO
	return GSKS_UP;
}

bool AndroidInput::IsJoystickButtonDown(const unsigned int id, const GS_JOYSTICK_BUTTON key) const
{
	// TODO
	return false;
}

bool AndroidInput::DetectJoysticks()
{
	// TODO
	return false;
}

GS_JOYSTICK_STATUS AndroidInput::GetJoystickStatus(const unsigned int id) const
{
	// TODO
	return GSJS_NOTDETECTED;
}

unsigned int AndroidInput::GetNumJoyButtons(const unsigned int id) const
{
	// TODO
	return 0;
}

math::Vector2 AndroidInput::GetJoystickXY(const unsigned int id) const
{
	// TODO
	return Vector2();
}

float AndroidInput::GetJoystickZ(const unsigned int id) const
{
	// TODO
	return 0.0f;
}

float AndroidInput::GetJoystickRudder(const unsigned int id) const
{
	// TODO
	return 0.0f;
}

math::Vector2 AndroidInput::GetJoystickUV(const unsigned int id) const
{
	// TODO
	return Vector2();
}

GS_JOYSTICK_BUTTON AndroidInput::GetFirstButtonDown(const unsigned int id) const
{
	// TODO
	return GSB_01;
}

unsigned int AndroidInput::GetNumJoysticks() const
{
	// TODO
	return 0;
}

math::Vector3 AndroidInput::GetAccelerometerData() const
{
	return m_accelerometer;
}

} // namespace gs2d