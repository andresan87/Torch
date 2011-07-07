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

#ifndef PLATFORM_H_
#define PLATFORM_H_

#include "../gs2dtypes.h"

// TODO implement wchar_t support on Android... not yet supported by the NDK
#ifndef ANDROID
#include <xstring>
#endif

#include <string>

namespace Platform {

gs2d::str_type::string GetFileName(const gs2d::str_type::string &source);

// TODO implement wchar_t support on Android... not yet supported by the NDK
#ifndef ANDROID
std::wstring AddLastSlash(const std::wstring& path);
std::string AddLastSlash(const std::string& path);
std::wstring& FixSlashes(std::wstring& path);
std::string& FixSlashes(std::string& path);

gs2d::str_type::string GetModulePath();
gs2d::str_type::string GetCurrentDirectoryPath();
std::wstring ConvertUtf8ToUnicode(const char* utf8String); // use it in low-level only. utf8::converter is a high-level wrapper
std::string ConvertUnicodeToUtf8(const wchar_t* unicodeString); // use it in low-level only. utf8::converter is a high-level wrapper
std::wstring ConvertAsciiToUnicode(const char* asciiString); // use it in low-level only. utf8::converter is a high-level wrapper
std::string ConvertUnicodeToAscii(const wchar_t* unicodeString); // use it in low-level only. utf8::converter is a high-level wrapper
#else
gs2d::str_type::string AddLastSlash(const gs2d::str_type::string& path);
gs2d::str_type::string& FixSlashes(gs2d::str_type::string& path);
gs2d::str_type::string GetModulePath();
#endif // #ifndef ANDROID
} 
// namespace Platform

#endif