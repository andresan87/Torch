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

#include "ETHSpeedTimer.h"

ETHSpeedTimer::ETHSpeedTimer()
{
	m_elapsed = 1;
}

double ETHSpeedTimer::CalcLastFrame()
{
	m_elapsed = elapsed();
	restart();
	return m_elapsed;
}

float ETHSpeedTimer::UnitsPerSecond(float speed) const
{
	return static_cast<float>(static_cast<double>(speed)*(m_elapsed));
}

float ETHSpeedTimer::UnitsPerSecond() const
{
	return static_cast<float>(m_elapsed);
}

double ETHSpeedTimer::GetElapsedTime() const
{
	return m_elapsed;
}
