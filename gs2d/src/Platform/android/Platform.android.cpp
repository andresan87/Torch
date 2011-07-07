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

#include "Platform.android.h"
#include "../Platform.h"
#include "../FileLogger.h"

namespace Platform {

gs2d::str_type::string FileLogger::GetLogPath()
{
	return GS_L("/sdcard/gs2dlog/");
}

gs2d::str_type::string AddLastSlash(const gs2d::str_type::string& path)
{
	if (path.empty())
	{
		return GS_L("");
	}
	gs2d::str_type::string r = (path);
	FixSlashes(r);
	const std::size_t lastChar = r.size()-1;

	if (r.at(lastChar) == GS_L('\\'))
	{
		r[lastChar] = GS_L('/');
		return r;
	}
	else if (r.at(lastChar) != GS_L('/'))
	{
		return r + GS_L("/");
	}
	else
	{
		return r;
	}
}

gs2d::str_type::string& FixSlashes(gs2d::str_type::string& path)
{
	return path;
}

gs2d::str_type::string GetModulePath()
{
	return GS_L("");
}


} // namespace
