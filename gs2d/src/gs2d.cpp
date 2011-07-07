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

#include "gs2d.h"

namespace gs2d {
using namespace math;

MTRand Randomizer::m_mt;

bool BitmapFont::ParseFNTString(const str_type::string& str)
{
	if (str == GS_L(""))
		return false;

	str_type::stringstream stream;
	stream << str;

	str_type::string line;
	str_type::string read, key, value;
	std::size_t i;
	while( !stream.eof() )
	{
		str_type::stringstream lineStream;
		std::getline( stream, line );
		lineStream << line;

		//read the line's type
		lineStream >> read;
		if( read == GS_L("common") )
		{
			//this holds common data
			while( !lineStream.eof() )
			{
				str_type::stringstream converter;
				lineStream >> read;
				i = read.find( GS_L('=') );
				key = read.substr( 0, i );
				value = read.substr( i + 1 );

				//assign the correct value
				converter << value;
				if( key == GS_L("lineHeight") )
					converter >> m_charSet.lineHeight;
				else if( key == GS_L("base") )
					converter >> m_charSet.base;
				else if( key == GS_L("scaleW") )
					converter >> m_charSet.width;
				else if( key == GS_L("scaleH") )
					converter >> m_charSet.height;
				else if( key == GS_L("pages") )
					converter >> m_charSet.pages;
			}
		}
		else if( read == GS_L("char") )
		{
			//this is data for a specific char
			unsigned short charID = 0;

			while( !lineStream.eof() )
			{
				str_type::stringstream converter;
				lineStream >> read;
				i = read.find( GS_L('=') );
				key = read.substr( 0, i );
				value = read.substr( i + 1 );

				//assign the correct value
				converter << value;
				if( key == GS_L("id") )
					converter >> charID;
				else if( key == GS_L("x") )
					converter >> m_charSet.chars[charID].x;
				else if( key == GS_L("y") )
					converter >> m_charSet.chars[charID].y;
				else if( key == GS_L("width") )
					converter >> m_charSet.chars[charID].width;
				else if( key == GS_L("height") )
					converter >> m_charSet.chars[charID].height;
				else if( key == GS_L("xoffset") )
					converter >> m_charSet.chars[charID].xOffset;
				else if( key == GS_L("yoffset") )
					converter >> m_charSet.chars[charID].yOffset;
				else if( key == GS_L("xadvance") )
					converter >> m_charSet.chars[charID].xAdvance;
				else if( key == GS_L("page") )
					converter >> m_charSet.chars[charID].page;
			}
		}
		else if( read == GS_L("page") )
		{
			//this holds page data
			while( !lineStream.eof() )
			{
				str_type::stringstream converter;
				lineStream >> read;
				i = read.find( GS_L('=') );
				key = read.substr( 0, i );
				value = read.substr( i + 1 );

				//assign the correct value
				converter << value;

				// TODO: read also the 'id' for safety. Id's must start from 0 and be incremented 1 by 1
				// if id numbers aren't right, the user must be warned.
				if( key == GS_L("file") )
				{
					str_type::string textureName;
					converter >> textureName;
					m_charSet.textureNames.push_back(textureName);
					break;
				}
			}
		}
	}
	//stream.close();
	return true;
}

bool BitmapFont::IsLoaded() const
{
	return (!m_bitmaps.empty());
}

BitmapFont::BitmapFont(VideoWeakPtr video, const str_type::string& fileName, const str_type::string& str)
{
	if (ParseFNTString(str))
	{
		m_bitmaps.resize(m_charSet.textureNames.size());
		for (unsigned int t=0; t<m_charSet.textureNames.size(); t++)
		{
			str_type::string path = fileName;
			std::size_t found = path.find_last_of(GS_L("/\\"));
			if (found != str_type::string::npos)
				path.resize(found+1);

			// remove "'s from the texture name
			while ((found = m_charSet.textureNames[t].find(GS_L("\""))) != str_type::string::npos)
			{
				m_charSet.textureNames[t].erase(found, 1);
			}

			path += m_charSet.textureNames[t];
			m_bitmaps[t] = video.lock()->CreateSprite(path, GS_BLACK, 0, 0);
			if (!m_bitmaps[t])
			{
				m_bitmaps.clear();
				break;
			}
			else
			{
				m_bitmaps[t]->SetOrigin(GSEO_DEFAULT);
			}
		}
	}
}

unsigned int BitmapFont::FindClosestCarretPosition(const str_type::string& text, const Vector2 &textPos, const Vector2 &reference)
{
	const unsigned int cursors = text.length()+1;
	float distance =-1;
	unsigned int returnCursor = 0;
	for (unsigned int t = 0; t < cursors; ++t)
	{
		const Vector2 pos = ComputeCarretPosition(text, t)+textPos;
		if (distance == -1)
		{
			distance = Distance(reference, pos);
		}
		else
		{
			const float tempDistance = Distance(reference, pos);
			if (tempDistance < distance)
			{
				distance = tempDistance;
				returnCursor = t;
			}
		}
	}
	return returnCursor;
}

template<class TChar>
inline int ConvertCharacterToIndex(TChar character)
{
	static const int bitsInByte = 8;
	static const int maxTCharValue = 1 << bitsInByte*sizeof(TChar);
	int index = static_cast<int>(character);
	if (index < 0)
	{
		index += maxTCharValue;
	}
	assert(index < maxTCharValue);
	return index;
}

Vector2 BitmapFont::ComputeCarretPosition(const str_type::string& text, const unsigned int pos)
{
	if (!IsLoaded())
	{
		return Vector2(0,0);
	}

	// seek the cursor position or the last character
	const unsigned int length = Min(text.size(), pos);

	Vector2 cursor = Vector2(0,0);
	for (unsigned int t=0; t<length; t++)
	{
		if (text[t] == GS_L('\n'))
		{
			cursor.y += m_charSet.lineHeight;
			continue;
		}
		int charId = ConvertCharacterToIndex<str_type::char_t>(text[t]);
		const CHAR_DESCRIPTOR &currentChar = m_charSet.chars[charId];
		cursor.x += currentChar.xAdvance;
	}
	return cursor;
}

Vector2 BitmapFont::ComputeTextBoxSize(const str_type::string& text)
{
	if (!IsLoaded())
	{
		return Vector2(0,0);
	}
	const unsigned int length = text.size();
	Vector2 cursor = Vector2(0,m_charSet.lineHeight);
	float lineWidth = 0.0f;
	for (unsigned int t=0; t<length; t++)
	{
		if (text[t] == GS_L('\n'))
		{
			cursor.x = Max(cursor.x, lineWidth);
			lineWidth = 0.0f;
			cursor.y += m_charSet.lineHeight;
			continue;
		}
		int charId = ConvertCharacterToIndex<str_type::char_t>(text[t]);
		const CHAR_DESCRIPTOR &currentChar = m_charSet.chars[charId];
		lineWidth += currentChar.xAdvance;
	}
	cursor.x = Max(cursor.x, lineWidth);
	return cursor;
}

Vector2 BitmapFont::DrawBitmapText(const Vector2 &pos, const str_type::string& text, const GS_COLOR color)
{
	if (!IsLoaded())
	{
		return Vector2(0,0);
	}

	const unsigned int length = text.size();
	Vector2 cursor = Vector2(floor(pos.x), floor(pos.y));

	std::vector<Sprite*> bitmapsPointers(m_bitmaps.size());
	for (unsigned int t=0; t<bitmapsPointers.size(); t++)
		bitmapsPointers[t] = m_bitmaps[t].get();

	int lastPageUsed =-1;
	for (unsigned int t=0; t<length; t++)
	{
		if (text[t] == GS_L('\n'))
		{
			cursor.x = pos.x;
			cursor.y += m_charSet.lineHeight;
			continue;
		}
		int charId = ConvertCharacterToIndex<str_type::char_t>(text[t]);
		const CHAR_DESCRIPTOR &currentChar = m_charSet.chars[charId];
		if (currentChar.width > 0 && currentChar.height > 0)
		{
			Rect2Df rect;
			rect.pos = Vector2(currentChar.x, currentChar.y);
			rect.size = Vector2(currentChar.width, currentChar.height);

			const Vector2 charPos =	cursor + Vector2(currentChar.xOffset, currentChar.yOffset);

			const int currentPage = currentChar.page;

			assert(currentPage >= 0);
			assert(currentPage < m_charSet.pages);

			// will only swap textures if it has to use a different page for this character
			if (lastPageUsed != currentPage)
			{
				if (lastPageUsed >= 0)
				{
					bitmapsPointers[lastPageUsed]->EndFastRendering();
				}
				bitmapsPointers[currentPage]->BeginFastRendering();
			}

			bitmapsPointers[currentPage]->SetRect(rect);
			bitmapsPointers[currentPage]->DrawShapedFast(charPos, Vector2(currentChar.width, currentChar.height), color);

			lastPageUsed = currentPage;
		}
		cursor.x += currentChar.xAdvance;
	}

	// end the rendering for the last char drawn
	if (lastPageUsed >= 0)
	{
		bitmapsPointers[lastPageUsed]->EndFastRendering();
	}

	cursor.y += m_charSet.lineHeight;
	return cursor;
}

GS2D_API unsigned long ComputeElapsedTime(ApplicationPtr app)
{
	static unsigned long lastTime = 0;
	const unsigned long currentTime = app->GetElapsedTime(Application::TU_MILLISECONDS);
	const unsigned long elapsedTime = currentTime-lastTime;
	lastTime = currentTime;
	return elapsedTime;
}

} // namespace gs2d