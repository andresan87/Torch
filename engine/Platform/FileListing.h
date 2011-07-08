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

#ifndef FILE_LISTING_H_
#define FILE_LISTING_H_

#include <iostream>
#include <vector>
#include <string>

namespace Platform {

class FileListing
{
public:
	struct FILE_NAME
	{
		inline void GetFullFileName(std::wstring &sOut)
		{
			sOut = dir+file;
		}
		std::wstring dir;
		std::wstring file;
	};

	bool ListDirectoryFiles(const wchar_t *directory, const wchar_t *extension);

	inline unsigned int GetNumFiles()
	{
		return m_fileName.size();
	}

	inline bool GetFileName(const unsigned int index, FILE_NAME &fileName)
	{
		if (index >= GetNumFiles())
		{
			std::wcerr << L"FileListing::GetFileName - index >= m_fileName.size().\n";
			return false;
		}
		fileName = m_fileName[index];
		return true;
	}

private:
	std::vector<FILE_NAME> m_fileName;
};

} // namespace Platform

#endif