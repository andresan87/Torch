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

#include "../FileListing.h"
#include <windows.h>

namespace Platform {

bool FileListing::ListDirectoryFiles(const wchar_t *directory, const wchar_t *extension)
{
	m_fileName.clear();
	std::wstring dirExt;
	dirExt = directory;
	dirExt += L"*.";
	dirExt += extension;

	WIN32_FIND_DATAW findFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;

	hFind = FindFirstFileW(dirExt.c_str(), &findFileData);
	
	if (hFind == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	FILE_NAME filename;
	filename.dir = directory;
	filename.file = findFileData.cFileName;
	m_fileName.push_back(filename);

	while (FindNextFileW(hFind, &findFileData) != 0)
	{
		filename.dir = directory;
		filename.file = findFileData.cFileName;
		m_fileName.push_back(filename);
	}
	FindClose(hFind);
	return true;
}

} // namespace Platform
