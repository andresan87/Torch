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

#ifndef GS2D_TYPES_H_
#define GS2D_TYPES_H_

#ifdef GS2D_EXPORT
 #define GS2D_API __declspec(dllexport)
#else
 #define GS2D_API
#endif

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp> 
#include <boost/any.hpp>

#define GS2D_UNUSED_ARGUMENT(argument) ((void)(argument))

#include <string>
#include <sstream>

namespace gs2d {

namespace str_type {
#ifdef ANDROID
 typedef std::string string;
 typedef std::stringstream stringstream;
 typedef char char_t;
 typedef std::ifstream ifstream;
 typedef std::ofstream ofstream;
 typedef std::ostringstream ostringstream;
 #define GS_L(x) x
 #define GS2D_STR_TYPE_ANSI
 #undef GS2D_STR_TYPE_WCHAR
#else
 typedef std::wstring string;
 typedef std::wstringstream stringstream;
 typedef wchar_t char_t;
 typedef std::wifstream ifstream;
 typedef std::wofstream ofstream;
 typedef std::wostringstream ostringstream;
 #define GS_L(x) L##x
 #define GS2D_STR_TYPE_WCHAR
 #undef GS2D_STR_TYPE_ANSI
#endif
} // namespace str

class Texture;
class Sprite;
typedef boost::shared_ptr<Texture> TexturePtr;
typedef boost::weak_ptr<Texture> TextureWeakPtr;

typedef boost::shared_ptr<Sprite> SpritePtr;
typedef boost::weak_ptr<Texture> TextureWeakPtr;

typedef unsigned long GS_DWORD;
typedef unsigned char GS_BYTE;

#pragma warning( push )
#pragma warning( disable : 4201 )

struct GS_COLOR
{
	GS_COLOR()
	{
		color = 0;
	}
	GS_COLOR(const GS_DWORD color)
	{
		this->color = color;
	}
	GS_COLOR(const GS_BYTE na, const GS_BYTE nr, const GS_BYTE ng, const GS_BYTE nb)
	{
		a=na;
		r=nr;
		g=ng;
		b=nb;
	}
	void SetColor(const GS_BYTE na, const GS_BYTE nr, const GS_BYTE ng, const GS_BYTE nb)
	{
		a=na;
		r=nr;
		g=ng;
		b=nb;
	}
	void SetColor(const GS_DWORD color)
	{
		this->color = color;
	}
	GS_COLOR &operator = (GS_DWORD color)
	{
		this->color = color;
		return *this;
	}
	operator GS_DWORD () const
	{
		return color;
	}
	void SetAlpha(const GS_BYTE na)
	{
		a=na;
	}
	void SetRed(const GS_BYTE nr)
	{
		r=nr;
	}
	void SetGreen(const GS_BYTE ng)
	{
		g=ng;
	}
	void SetBlue(const GS_BYTE nb)
	{
		b=nb;
	}

	union
	{
		struct
		{
			GS_BYTE b, g, r, a;
		};
		GS_DWORD color;
	};
};

#pragma warning( pop )

const GS_COLOR GS_ZERO(0x0);
const GS_COLOR GS_BLACK(0xFF000000);
const GS_COLOR GS_WHITE(0xFFFFFFFF);
const GS_COLOR GS_RED(0xFFFF0000);
const GS_COLOR GS_GREEN(0xFF00FF00);
const GS_COLOR GS_BLUE(0xFF0000FF);
const GS_COLOR GS_YELLOW(0xFFFFFF00);

enum GS_MESSAGE_TYPE
{
	GSMT_INFO = 0,
	GSMT_WARNING = 1,
	GSMT_ERROR = 2
};

} // namespace gs2d
#endif