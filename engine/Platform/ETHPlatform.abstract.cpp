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

#ifdef ABSTRACT_PLATFORM

#include "../Platform.h"
#include "../../../angelscript/include/angelscript.h"

namespace ETHPlatform 
{
	bool LoadDynamicLibrary(const str_type::char_t *filePath, asIScriptEngine *pASEngine)
	{
		// TODO: implement other platform versions
		return false;
	}
} 
// namespace ETHPlatform

#endif // ABSTRACT_PLATFORM
