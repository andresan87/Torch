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

#ifndef ETH_DATE_TIME_H_
#define ETH_DATE_TIME_H_

class ETHDateTime
{
public:
	void AddRef();
	void Release();

	ETHDateTime();
	void Update();

	unsigned int GetDay() const;
	unsigned int GetMonth() const;
	unsigned int GetYear() const;
	unsigned int GetHours() const;
	unsigned int GetMinutes() const;
	unsigned int GetSeconds() const;

private:
	unsigned int m_day;
	unsigned int m_month;
	unsigned int m_year;
	unsigned int m_hours;
	unsigned int m_minutes;
	unsigned int m_seconds;
	int m_ref;
};

#endif