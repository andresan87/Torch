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

#ifndef ETH_PRIMITIVE_DRAWER_H_
#define ETH_PRIMITIVE_DRAWER_H_

#include "ETHResourceManager.h"
#include "ETHResourceProvider.h"

class ETHPrimitiveDrawer
{
public:
	virtual bool Draw(const unsigned long time) = 0;
	virtual bool IsAlive(const unsigned long time) const = 0;
};

class ETHTextDrawer : public ETHPrimitiveDrawer
{
public:
	ETHTextDrawer(const ETHResourceProviderPtr& provider, const Vector2 &pos, const str_type::string &text, const str_type::string &font,
				 const GS_DWORD color, const unsigned long time,
				 const unsigned long startTime);

	ETHTextDrawer(const ETHResourceProviderPtr& provider, const Vector2 &pos, const str_type::string &text, const str_type::string &font,
				 const GS_DWORD color);

	bool Draw(const unsigned long time);
	bool IsAlive(const unsigned long time) const
	{
		if (time-startTime > this->time)
			return false;
		return true;
	}

private:
	Vector2 v2Pos;
	str_type::string text;
	str_type::string font;
	GS_DWORD color;
	unsigned long time;
	unsigned long startTime;
	ETHResourceProviderPtr provider;
};

class ETHRectangleDrawer : public ETHPrimitiveDrawer
{
public:
	ETHRectangleDrawer(const ETHResourceProviderPtr& provider, const Vector2 &pos, const Vector2 &size, const GS_COLOR color, const float depth = 0.0f);
	ETHRectangleDrawer(const ETHResourceProviderPtr& provider, const Vector2 &pos, const Vector2 &size,
					  const GS_COLOR color0, const GS_COLOR color1, const GS_COLOR color2, const GS_COLOR color3,
					  const float depth = 0.0f);

	bool Draw(const unsigned long time);
	bool IsAlive(const unsigned long time) const
	{
		GS2D_UNUSED_ARGUMENT(time);
		return false;
	}

private:
	Vector2 v2Pos;
	Vector2 v2Size;
	GS_COLOR color0, color1, color2, color3;
	float depth;
	ETHResourceProviderPtr provider;
};


class ETHLineDrawer : public ETHPrimitiveDrawer
{
public:
	ETHLineDrawer(const ETHResourceProviderPtr& provider, const Vector2 &a, const Vector2 &b, const GS_COLOR color0,
				  const GS_COLOR color1, const float width, const float depth = 0.0f);

	bool Draw(const unsigned long time);
	bool IsAlive(const unsigned long time) const
	{
		GS2D_UNUSED_ARGUMENT(time);
		return false;
	}

private:
	Vector2 a, b;
	GS_COLOR colorA, colorB;
	float width, depth;
	ETHResourceProviderPtr provider;
};

class ETHSpriteDrawer : public ETHPrimitiveDrawer
{
public:
	ETHSpriteDrawer(const ETHResourceProviderPtr& provider, ETHGraphicResourceManagerPtr graphicResources, const str_type::string &currentPath,
					const str_type::string &name, const Vector2 &pos,
					const Vector2 &size, const GS_COLOR color, const float depth = 0.0f);

	bool Draw(const unsigned long time);
	bool IsAlive(const unsigned long time) const
	{
		GS2D_UNUSED_ARGUMENT(time);
		return false;
	}

private:
	Vector2 v2Pos;
	Vector2 v2Size;
	GS_COLOR color0, color1, color2, color3;
	str_type::string name;
	float depth;
	str_type::string currentPath;
	ETHGraphicResourceManagerPtr graphicResources;
	ETHResourceProviderPtr provider;
};

#endif