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
    License along with GameSpaceLib. If not, see
    <http://www.gnu.org/licenses/>.

-----------------------------------------------------------------------*/

#ifndef GS2D_DEFAULT_SHADER_H_
#define GS2D_DEFAULT_SHADER_H_

namespace gs2d {
namespace gs2dglobal {

// the default vertex shader
const char defaultVSCode[] = 
"uniform float4x4 viewMatrix;\n" \
"uniform float4x4 rotationMatrix;\n" \
"uniform float2 screenSize;\n" \
"uniform float2 size;\n" \
"uniform float2 entityPos;\n" \
"uniform float2 center;\n" \
"uniform float2 bitmapSize;\n" \
"uniform float2 rectPos;\n" \
"uniform float2 rectSize;\n" \
"uniform float2 scroll;\n" \
"uniform float2 multiply;\n" \
"uniform float4 color0;\n" \
"uniform float4 color1;\n" \
"uniform float4 color2;\n" \
"uniform float4 color3;\n" \
"uniform float2 flipAdd;\n" \
"uniform float2 flipMul;\n" \
"uniform float2 cameraPos;\n" \
"uniform float depth;\n" \
"float4 transformSprite(float3 position)\n" \
"{\n" \
"	float4 newPos = float4(position, 1) * float4(flipMul,1,1) + float4(flipAdd,0,0);\n" \
"	newPos = newPos * float4(size,1,1) - float4(center, 0, 0);\n" \
"	newPos = mul(rotationMatrix, newPos);\n" \
"	newPos += float4(entityPos,0,0)-float4(screenSize/2,0,0)-float4(cameraPos,0,0);\n" \
"	newPos *= float4(1,-1,1,1);\n" \
"	return mul(viewMatrix, newPos);\n" \
"}\n" \
"float4 transformRectangle(float3 position)\n" \
"{\n" \
"	float4 newPos = float4(position, 1) * float4(size,1,1) - float4(center, 0, 0);\n" \
"	newPos = mul(rotationMatrix, newPos);\n" \
"	newPos += float4(entityPos,0,0)-float4(screenSize/2,0,0);\n" \
"	newPos *= float4(1,-1,1,1);\n" \
"	return mul(viewMatrix, newPos);\n" \
"}\n" \
"float2 transformCoord(float2 texCoord)\n" \
"{\n" \
"	float2 newCoord = texCoord * (rectSize/bitmapSize);\n" \
"	newCoord += (rectPos/bitmapSize);\n" \
"	return (newCoord+(scroll/bitmapSize))*multiply;\n" \
"}\n" \
"float4 getVertexColor(float3 position)\n" \
"{\n" \
"	float4 vertex0 = color0 * (1-position.x) * (1-position.y);\n" \
"	float4 vertex1 = color1 * (position.x) * (1-position.y);\n" \
"	float4 vertex2 = color2 * (1-position.x) * (position.y);\n" \
"	float4 vertex3 = color3 * (position.x) * (position.y);\n" \
"	return vertex0 + vertex1 + vertex2 + vertex3;\n" \
"}\n" \
"void sprite(float3 position : POSITION,\n" \
"			float2 texCoord : TEXCOORD0,\n" \
"			out float4 oPosition : POSITION,\n" \
"			out float4 oColor    : COLOR0,\n" \
"			out float2 oTexCoord0 : TEXCOORD0)\n" \
"{\n" \
"	float4 spritePos = transformSprite(position);\n" \
"	spritePos.z = depth;\n" \
"	oPosition = spritePos;\n" \
"	oTexCoord0 = transformCoord(texCoord);\n" \
"	oColor = getVertexColor(position);\n" \
"}\n" \
"void rectangle(float3 position : POSITION,\n" \
"			   out float4 oPosition : POSITION,\n" \
"			   out float4 oColor : COLOR0)\n" \
"{\n" \
"	oPosition = transformRectangle(position);\n" \
"	oColor = getVertexColor(position);\n" \
"}";


const char fastSimpleVSCode[] = 
"uniform float4x4 viewMatrix;\n" \
"uniform float2 screenSize;\n" \
"uniform float2 size;\n" \
"uniform float2 entityPos;\n" \
"uniform float2 bitmapSize;\n" \
"uniform float2 rectPos;\n" \
"uniform float2 rectSize;\n" \
"uniform float4 color0;\n" \
"float4 transformFast(float3 position)\n" \
"{\n" \
"	float4 newPos = float4(position, 1) * float4(size,1,1);\n" \
"	newPos += float4(entityPos,0,0)-float4(screenSize/2,0,0);\n" \
"	newPos *= float4(1,-1,1,1);\n" \
"	return mul(viewMatrix, newPos);\n" \
"}\n" \
"float2 transformFastCoord(float2 texCoord)\n" \
"{\n" \
"	float2 newCoord = texCoord * (rectSize/bitmapSize);\n" \
"	newCoord += (rectPos/bitmapSize);\n" \
"	return newCoord;\n" \
"}\n" \
"void fast(float3 position : POSITION, float2 texCoord : TEXCOORD0,\n" \
"		   out float4 oPosition : POSITION, out float2 oTexCoord : TEXCOORD0,\n" \
"		   out float4 oColor : COLOR0)\n" \
"{\n" \
"	oPosition = transformFast(position);\n" \
"	oTexCoord = transformFastCoord(texCoord);\n" \
"	oColor = color0;\n" \
"}";

} // namespace gs2dglobal
} // namespace gs2d

#endif
