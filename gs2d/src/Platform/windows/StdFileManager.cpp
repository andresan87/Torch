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

#include "StdFileManager.h"
#include <fstream>

using namespace gs2d;

namespace Platform {

bool StdFileManager::IsLoaded() const
{
	return true;
}

bool StdFileManager::GetFileBuffer(const str_type::string &fileName, FileBuffer &out)
{
	#if _MSC_VER >= 1500
		FILE *f = 0;
		_wfopen_s(&f, fileName.c_str(), GS_L("rb"));
	#else
		FILE *f = _wfopen(fileName.c_str(), GS_L("rb"));
	#endif

	if (!f)
	{
		return false;
	}
	
	fseek(f, 0, SEEK_END);
	const std::size_t len = ftell(f);
	fseek(f, 0, SEEK_SET);

	out = FileBuffer(new _FileBuffer<unsigned char>(len));
	fread(out->GetAddress(), len, 1, f);
	fclose(f);
	return true;
}

bool StdFileManager::GetUTF8BOMFileString(const str_type::string &fileName, str_type::string &out)
{
	FileBuffer buffer;
	if (GetFileBuffer(fileName, buffer))
	{
		// TODO optimize it
		str_type::stringstream ss;
		unsigned char *adr = buffer->GetAddress();
		for (int t=3; t<buffer->GetBufferSize(); t++)
		{
			ss << static_cast<char>(adr[t]);
		}
		out = ss.str();
		return true;
	}
	else
	{
		return false;
	}
}

bool StdFileManager::GetAnsiFileString(const str_type::string &fileName, str_type::string &out)
{
	FileBuffer buffer;
	if (GetFileBuffer(fileName, buffer))
	{
		// TODO optimize it
		str_type::stringstream ss;
		unsigned char *adr = buffer->GetAddress();
		for (int t=0; t<buffer->GetBufferSize(); t++)
		{
			ss << static_cast<char>(adr[t]);
		}
		out = ss.str();
		return true;
	}
	else
	{
		return false;
	}
}

bool StdFileManager::GetUTF16FileString(const str_type::string &fileName, str_type::string &out)
{
	FILE* file = 0;
	errno_t err = _wfopen_s(&file, fileName.c_str(), L"rb");
	if (err || !file)
	{
		return false;
	}

	long length = 0;

	// const std::size_t wcharSize = sizeof(wchar_t);
	const std::size_t BOMsize = 2;

	fseek(file, 0, SEEK_END);
	length = ftell(file);
	fseek(file, 0, SEEK_SET);

	if (length <= 0)
	{
		return false;
	}

	out.resize(length);
	unsigned short usOrder;
	fread(&usOrder, BOMsize, 1, file);
	if (fread(&out[0], length - BOMsize, 1, file ) != 1)
	{
		fclose(file);
		return false;
	}
	fclose(file);
	return true;
}

}