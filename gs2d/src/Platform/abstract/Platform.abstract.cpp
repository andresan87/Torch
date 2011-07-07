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

#ifdef ABSTRACT_PLATFORM

#include "../Platform.h"

namespace Platform 
{
	wstring ConvertUtf8ToUnicode(const string& stringUtf8)
	{
		// TODO: implement for every particular platform
		return wstring();
	}

	string ConvertUnicodeToUtf8(const wchar_t * wcsUnicode)
	{
		// TODO: implement for every particular platform
		return string();
	}

	std::wstring ConvertAsciiToUnicode(const char* szUtf8)
	{
		return wstring();
	}

	std::string ConvertUnicodeToAscii(const wchar_t* wcsUnicode)
	{
		return string();
	}

	wstring GetCurrentDirectoryPath()
	{
		// TODO: implement for every particular platform
		return string();
	}
} 
// namespace Platform

#endif // ABSTRACT_PLATFORM
