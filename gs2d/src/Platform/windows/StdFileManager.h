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

#ifndef STD_FILE_MANAGER_H_
#define STD_FILE_MANAGER_H_

#include "../FileManager.h"

namespace Platform {

class StdFileManager : public FileManager
{
public:
	bool IsLoaded() const;
	bool GetFileBuffer(const gs2d::str_type::string &fileName, FileBuffer &out);
	bool GetAnsiFileString(const gs2d::str_type::string &fileName, gs2d::str_type::string &out);
	bool GetUTF8BOMFileString(const gs2d::str_type::string &fileName, gs2d::str_type::string &out);
	bool GetUTF16FileString(const gs2d::str_type::string &fileName, gs2d::str_type::string &out);
};

typedef boost::shared_ptr<StdFileManager> StdFileManagerPtr;

}

#endif