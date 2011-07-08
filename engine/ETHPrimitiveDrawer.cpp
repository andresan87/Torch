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

#include "ETHPrimitiveDrawer.h"

ETHTextDrawer::ETHTextDrawer(const ETHResourceProviderPtr& provider, const Vector2 &pos, 
							 const str_type::string &text, const str_type::string &font,
							 const GS_DWORD color, const unsigned long time,
							 const unsigned long startTime)
{
	this->v2Pos = pos;
	this->text = text;
	this->font = font;
	this->color = color;
	this->time = time;
	this->startTime = startTime;
	this->provider = provider;
}

ETHTextDrawer::ETHTextDrawer(const ETHResourceProviderPtr& provider, const Vector2 &pos, 
							 const str_type::string &text, const str_type::string &font,
							 const GS_DWORD color)
{
	this->v2Pos = pos;
	this->text = text;
	this->font = font;
	this->color = color;
	this->time = 0;
	this->startTime = 0;
	this->provider = provider;
}

bool ETHTextDrawer::Draw(const unsigned long time)
{
	GS_COLOR color = this->color;

	if (this->time > 0)
	{
		const unsigned int  elapsed = time-this->startTime;
		const float fade = 1.0f-Clamp((float)elapsed/(float)this->time, 0.0f, 1.0f);
		color.a = (GS_BYTE)(fade*255.0f);
	}
	return provider->GetVideo()->DrawBitmapText( v2Pos, text, font, color );
}

ETHRectangleDrawer::ETHRectangleDrawer(const ETHResourceProviderPtr& provider, const Vector2 &pos, const Vector2 &size, const GS_COLOR color, const float depth)
{
	this->v2Pos = pos;
	this->v2Size = size;
	this->color0 = color;
	this->color1 = color;
	this->color2 = color;
	this->color3 = color;
	this->depth = depth;
	this->provider = provider;
}

ETHRectangleDrawer::ETHRectangleDrawer(const ETHResourceProviderPtr& provider,
									   const Vector2 &pos, const Vector2 &size, const GS_COLOR color0, const GS_COLOR color1,
									   const GS_COLOR color2, const GS_COLOR color3, const float depth)
{
	this->v2Pos = pos;
	this->v2Size = size;
	this->color0 = color0;
	this->color1 = color1;
	this->color2 = color2;
	this->color3 = color3;
	this->depth = depth;
	this->provider = provider;
}

bool ETHRectangleDrawer::Draw(const unsigned long time)
{
	GS2D_UNUSED_ARGUMENT(time);
	provider->GetVideo()->SetSpriteDepth(depth);
	return provider->GetVideo()->DrawRectangle(v2Pos, v2Size, color0, color1, color2, color3, 0.0f);
}

ETHLineDrawer::ETHLineDrawer(const ETHResourceProviderPtr& provider, const Vector2 &a, const Vector2 &b,
								   const GS_COLOR color0, const GS_COLOR color1, const float width, const float depth)
{
	this->a = a;
	this->b = b;
	this->colorA = color0;
	this->colorB = color1;
	this->depth = depth;
	this->width = width;
	this->provider = provider;
}

bool ETHLineDrawer::Draw(const unsigned long time)
{
	GS2D_UNUSED_ARGUMENT(time);
	provider->GetVideo()->SetSpriteDepth(depth);
	provider->GetVideo()->SetLineWidth(width);
	return provider->GetVideo()->DrawLine(a, b, colorA, colorB);
}

ETHSpriteDrawer::ETHSpriteDrawer(const ETHResourceProviderPtr& provider, ETHGraphicResourceManagerPtr graphicResources,
								 const str_type::string &currentPath, const str_type::string &name, const Vector2 &pos,
								 const Vector2 &size, const GS_COLOR color, const float depth)
{
	this->name = name;
	this->color0 = color;
	this->color1 = color;
	this->color2 = color;
	this->color3 = color;
	this->v2Pos = pos;
	this->v2Size = size;
	this->depth = depth;
	this->graphicResources = graphicResources;
	this->provider = provider;
	this->currentPath = currentPath;
}

bool ETHSpriteDrawer::Draw(const unsigned long time)
{
	GS2D_UNUSED_ARGUMENT(time);
	SpritePtr pSprite = graphicResources->GetPointer(provider->GetVideo(), name, currentPath, GS_L(""), false);
	if (pSprite)
	{
		const Vector2 size(v2Size == Vector2(0,0) ? pSprite->GetBitmapSizeF() : v2Size);
		provider->GetVideo()->SetSpriteDepth(depth);
		return pSprite->DrawShaped(v2Pos, size, color0, color1, color2, color3, 0.0f);
	}
	return false;
}