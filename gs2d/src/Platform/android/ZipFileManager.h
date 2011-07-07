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

#ifndef ZIP_FILE_MANAGER_H_
#define ZIP_FILE_MANAGER_H_

#include <zip.h>
#include "../FileManager.h"

namespace Platform {

class ZipFileManager : public FileManager
{
public:
	ZipFileManager(const gs2d::str_type::char_t *filePath);
	~ZipFileManager();

	bool IsLoaded() const;
	bool GetFileBuffer(const gs2d::str_type::string &fileName, FileBuffer &out);
    bool GetAnsiFileString(const gs2d::str_type::string &fileName, gs2d::str_type::string &out);
    bool GetUTF8BOMFileString(const gs2d::str_type::string &fileName, gs2d::str_type::string &out);
	bool GetUTF16FileString(const gs2d::str_type::string &fileName, gs2d::str_type::string &out);
	zip *GetZip();

private:
	zip *m_archive;
};

typedef boost::shared_ptr<ZipFileManager> ZipFileManagerPtr;

} // namespace Platform

#endif