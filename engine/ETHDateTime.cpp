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

#include "ETHDateTime.h"

#define _SCL_SECURE_NO_WARNINGS
#pragma warning( push )
#pragma warning( disable : 4127 )
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#pragma warning( pop )
#undef _SCL_SECURE_NO_WARNINGS

ETHDateTime::ETHDateTime()
{
	m_ref = 1;
	Update();
}

void ETHDateTime::AddRef()
{
	m_ref++;
}

void ETHDateTime::Release()
{
	if (--m_ref == 0)
	{
		delete this;
	}
}

void ETHDateTime::Update()
{
	boost::posix_time::ptime local_time = boost::posix_time::second_clock::local_time();
	boost::gregorian::date d = local_time.date();

	m_day = d.day();
	m_month = d.month();
	m_year = d.year();

	m_hours = local_time.time_of_day().hours();
	m_minutes = local_time.time_of_day().minutes();
	m_seconds = local_time.time_of_day().seconds();
}

unsigned int ETHDateTime::GetDay() const
{
	return m_day;
}

unsigned int ETHDateTime::GetMonth() const
{
	return m_month;
}

unsigned int ETHDateTime::GetYear() const
{
	return m_year;
}

unsigned int ETHDateTime::GetHours() const
{
	return m_hours;
}

unsigned int ETHDateTime::GetMinutes() const
{
	return m_minutes;
}

unsigned int ETHDateTime::GetSeconds() const
{
	return m_seconds;
}
