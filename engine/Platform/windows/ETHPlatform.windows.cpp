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

#ifdef WIN32

#include "../../ETHEngine.h"
#include "../ETHPlatform.h"
#include "../../../angelscript/include/angelscript.h"
#include "../../../gs2d/src/Platform/Platform.h"

#include <assert.h>
#include <windows.h>

namespace ETHPlatform {

bool LoadDynamicLibrary(const str_type::char_t *filePath, asIScriptEngine *pASEngine)
{
	HINSTANCE hinstLib; 
	ETH_PLUGIN_PROC currentProc; 

	#ifdef GS2D_STR_TYPE_WCHAR
		hinstLib = LoadLibraryW(filePath);
	#else
		hinstLib = LoadLibraryA(filePath);
	#endif
 
	if (hinstLib != NULL) 
	{
		#ifdef GS2D_STR_TYPE_WCHAR
			currentProc = (ETH_PLUGIN_PROC)GetProcAddress(hinstLib, Platform::ConvertUnicodeToAscii(_ETH_PLUGIN_FUNCTION_NAME).c_str());
		#else
			currentProc = (ETH_PLUGIN_PROC)GetProcAddress(hinstLib, _ETH_PLUGIN_FUNCTION_NAME);
		#endif

		if (currentProc == NULL) 
		{
			std::cerr << "Invalid or non-existent function: " << _ETH_PLUGIN_FUNCTION_NAME << std::endl;
			return false;
		}
		else
		{
			(currentProc)(pASEngine); 
		}
		FreeLibrary(hinstLib); 
	}
	else
	{
		return false;
	}
	return true;
}

}
// namespace ETHPlatform

#endif // WIN32
