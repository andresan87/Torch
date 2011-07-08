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

#include "ETHBinaryStream.h"
#include <assert.h>

ETHBinaryStream::ETHBinaryStream()
{
	m_out = 0;
	m_in = 0;
}

void ETHBinaryStream::Read(void *ptr, asUINT size)
{
	assert(m_in != 0);
	if (m_in)
	{
		fread(ptr, 1, size, m_in);
	}
}

void ETHBinaryStream::Write(const void *ptr, asUINT size)
{
	assert(m_out != 0);
	if (m_out)
	{
		fwrite(ptr, 1, size, m_out);
	}
}

str_type::string ETHBinaryStream::GetFileName() const
{
	return m_fileName;
}

void ETHBinaryStream::SetFileName(const str_type::string& fileName)
{
	m_fileName = fileName;
}

bool ETHBinaryStream::OpenW(const str_type::string& fileName)
{
	return Open(&m_out, fileName);
}

bool ETHBinaryStream::CloseW()
{
	return Close(&m_out);
}

bool ETHBinaryStream::OpenR(const str_type::string& fileName)
{
	return Open(&m_in, fileName);
}

bool ETHBinaryStream::CloseR()
{
	return Close(&m_in);
}

bool ETHBinaryStream::Open(FILE ** file, const str_type::string& fileName)
{
	assert(file == &m_in || file == &m_out);
	Close(file);
	SetFileName(fileName);
	#ifdef GS2D_STR_TYPE_WCHAR
	errno_t error = _wfopen_s( file, GetFileName().c_str(), file == &m_in ? L"rb" : L"wb" );
	#else
	errno_t error = 0; *file = fopen(GetFileName().c_str(), file == &m_in ? "rb" : "wb" );
	#endif
	if ( !error && *file )
		return true;
	*file = 0;
	return false;
}

bool ETHBinaryStream::Close(FILE ** file)
{
	if (*file)
	{
		fclose(*file);
		*file = 0;
	}
	return true;
}
