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

#include "ZipFileManager.h"
#include "AndroidLog.h"

using namespace gs2d;

namespace Platform {

ZipFileManager::ZipFileManager(const str_type::char_t *filePath)
{
	m_archive = zip_open(filePath, 0, NULL);
	if (m_archive == NULL)
	{
		LOGE("Error loading APK");
	}
}

ZipFileManager::~ZipFileManager()
{
	zip_close(m_archive);
}

bool ZipFileManager::IsLoaded() const
{
	return m_archive != NULL;
}

bool ZipFileManager::GetFileBuffer(const str_type::string &fileName, FileBuffer &out)
{
	if (!IsLoaded())
		return false;

	zip_file *file = zip_fopen(m_archive, fileName.c_str(), 0);

	if (file == NULL)
		return false;

	struct zip_stat stat;
	zip_stat(m_archive, fileName.c_str(), 0, &stat);

	out = FileBuffer(new _FileBuffer<unsigned char>(stat.size));
	zip_fread(file, out->GetAddress(), stat.size);

	zip_fclose(file);
	return true;
}

bool ZipFileManager::GetAnsiFileString(const str_type::string &fileName, str_type::string &out)
{
	FileBuffer buffer;
	if (GetFileBuffer(fileName, buffer))
	{
		// TODO optimize it
		str_type::stringstream ss;
		const unsigned char *adr = buffer->GetAddress();
		for (int t=0; t<buffer->GetBufferSize(); t++)
		{
			ss << adr[t];
		}
		out = ss.str();
		return true;
	}
	else
	{
		return false;
	}
}

bool ZipFileManager::GetUTF8BOMFileString(const str_type::string &fileName, str_type::string &out)
{
	FileBuffer buffer;
	if (GetFileBuffer(fileName, buffer))
	{
		// TODO optimize it
		str_type::stringstream ss;
		const unsigned char *adr = buffer->GetAddress();
		for (int t=3; t<buffer->GetBufferSize(); t++)
		{
			ss << adr[t];
		}
		out = ss.str();
		return true;
	}
	else
	{
		return false;
	}
}

bool ZipFileManager::GetUTF16FileString(const str_type::string &fileName, str_type::string &out)
{
	FileBuffer utf16Buffer;
	if (GetFileBuffer(fileName, utf16Buffer))
	{
		// TODO/TO-DO: handle big-endian encoding too
		str_type::stringstream ss;
		const unsigned char *adr = utf16Buffer->GetAddress();
		for (int t = 2; t < utf16Buffer->GetBufferSize(); t += 2)
		{
			ss << adr[t];
		}
		out = ss.str();
		return true;
	}
	else
	{
		return false;
	}
	return true;
}

zip *ZipFileManager::GetZip()
{
	return m_archive;
}

} // namespace Platform