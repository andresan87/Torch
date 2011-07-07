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

#include "Platform.h"
#include <iostream>

namespace Platform {

gs2d::str_type::string GetFileName(const gs2d::str_type::string &source)
{
	const unsigned int len = source.length();
	gs2d::str_type::string r = source;
	unsigned int t;
	for (t = len-1; t > 0; t--)
	{
		if (r[t] == GS_L('\\') || r[t] == GS_L('/'))
		{
			r = r.substr(t+1);
			break;
		}
	}
	return r;
}

} // namespace Platform