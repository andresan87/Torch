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

#ifndef ETH_AS_UTIL_H_
#define ETH_AS_UTIL_H_

#include "../angelscript/include/angelscript.h"
#include "ETHCommon.h"

namespace ETHGlobal {
	void CheckFunctionSeekError(const int id, const str_type::string& function);
	void PrintException(asIScriptContext *pContext);
	void ShowMessage(str_type::string message, const ETH_MESSAGE type);
	void ExecuteContext(asIScriptContext *pContext, const int id, const bool prepare = true);
} // namespace ETHGlobal

#endif