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

#ifndef ETH_UTF8STRING_H_
#define ETH_UTF8STRING_H_

#include <xstring>

namespace utf8
{
	class converter
	{
	private:
		std::string ansidata;
		std::wstring widedata;

	public:

		// this is a stub to convert any char sequence to wstring and vice versa
//#define UNSAFE_UTF8_DISABLE_CHECKS	// just comment it and compile to see where are you have non-safe strings conversion
#if defined(UNSAFE_UTF8_DISABLE_CHECKS) || defined(UNSAFE_UTF8_ALLOWED) // UNSAFE_UTF8_ALLOWED used to don't verify Editor
		#pragma message("WARNING: possible treating of invalid string data as UTF8 while conversion to Unicode")
		converter()
		{
		}

		converter(const char * szData)
		{
			SetData(szData);
		}
		converter(const std::string& sData)
		{
			SetData(sData.c_str());
		}
		converter(const wchar_t * wcsData)
		{
			SetWideData(wcsData);
		}
		converter(const std::wstring& wsData)
		{
			SetWideData(wsData.c_str());
		}

		const char* c_str() const
		{
			return ansidata.c_str();
		}

		const wchar_t* wc_str() const
		{
			return widedata.c_str();
		}

		const std::string& str() const
		{
			return ansidata;
		}

		const std::wstring& wstr() const
		{
			return widedata;
		}

		std::string::size_type length() const
		{
			//if (widedata.length() != ansidata.length()) _asm { int 3 }; // temporary debug test stuff
			return ansidata.length();
		}
#else
	private:
		converter()
		{
		}
#endif

	private:
		void SetData(const char * szData);
		void SetWideData(const wchar_t * wcsData);
	};

#if defined(UNSAFE_UTF8_ALLOWED)
	// this is a short notation for utf8::converter, it should be removed after full implementation of I18N
	// use full notation for normal conversions
	typedef converter c;
#endif
} 
// namespace ETHPlatform

#endif