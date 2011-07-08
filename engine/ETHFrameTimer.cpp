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

#include "ETHFrameTimer.h"

ETHFrameTimer::ETHFrameTimer()
{
	Reset();
}

void ETHFrameTimer::Reset()
{
	m_interpolator = 0.0f;
	m_currentFrame = 0;
	m_currentFirstFrame = m_currentLastFrame = 0;
	restart();
}

unsigned int ETHFrameTimer::Get()
{
	return m_currentFrame;
}

unsigned int ETHFrameTimer::Update(const unsigned int firstFrame, const unsigned int lastFrame, const unsigned int stride)
{
	if (firstFrame != m_currentFirstFrame || lastFrame != m_currentLastFrame)
	{
		m_currentFrame = m_currentFirstFrame = firstFrame;
		m_currentLastFrame = lastFrame;
		restart();
		return m_currentFrame;
	}

	if (elapsed() > static_cast<double>(stride)/1000.0)
	{
		restart();
		m_currentFrame++;
		if (m_currentFrame > lastFrame)
			m_currentFrame = firstFrame;
	}

	m_interpolator = (static_cast<float>(elapsed())*1000.0f)/static_cast<float>(stride);
	return m_currentFrame;
}

float ETHFrameTimer::GetInterpolationBias() const
{
	return m_interpolator;
}
