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

#ifndef ETH_BINARY_STREAM_H_
#define ETH_BINARY_STREAM_H_

#include "../angelscript/include/angelscript.h"
#include <string>
#include <iostream>
#include <fstream>
#include "../gs2d/src/gs2dtypes.h"

using namespace gs2d;

class ETHBinaryStream : public asIBinaryStream
{
public:
	ETHBinaryStream();

	virtual void Read(void *ptr, asUINT size);
	virtual void Write(const void *ptr, asUINT size);

	str_type::string GetFileName() const;
	bool OpenW(const str_type::string& fileName);
	bool CloseW();
	bool OpenR(const str_type::string& fileName);
	bool CloseR();

private:
	void SetFileName(const str_type::string& fileName);
	bool Open(FILE ** file, const str_type::string& fileName);
	bool Close(FILE ** file);

	FILE* m_out;
	FILE* m_in;
	str_type::string m_fileName;
};

#endif
