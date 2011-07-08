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

#include "ETHASUtil.h"
#include <iostream>

#include "ETHResourceProvider.h"

namespace ETHGlobal {
void ExecuteContext(asIScriptContext *pContext, const int id, const bool prepare)
{
	if (prepare)
	{
		if (pContext->Prepare(id) < 0)
		{
			ETH_STREAM_DECL(ss) << GS_L("(ExecuteContext) Couldn't prepare context for function  ID ") << id;
			ETHResourceProvider::Log(ss.str(), Platform::Logger::ERROR);
			return;
		}
	}

	const int r = pContext->Execute();
	if (r != asEXECUTION_FINISHED)
	{
		if (r == asEXECUTION_EXCEPTION)
		{
			PrintException(pContext);
			ETH_STREAM_DECL(ss) << GS_L("Exception: ") << pContext->GetExceptionString();
			#ifdef _DEBUG
			ss << static_cast<str_type::char_t>(0x07);
			#endif
			ShowMessage(ss.str(), ETH_ERROR);
		}
	}
}

void CheckFunctionSeekError(const int id, const str_type::string& function)
{
	str_type::stringstream ss;
	switch (id)
	{
	case asERROR:
		ss << GS_L("Callback function seeking error - Invalid module (") << function << GS_L(").");
		ETHResourceProvider::Log(ss.str(), Platform::Logger::ERROR);
		break;
	case asMULTIPLE_FUNCTIONS:
		ss << GS_L("\n*Script error:\nCallback function seeking error - there are multiple functions with this name (") << function << GS_L(").");
		ETHResourceProvider::Log(ss.str(), Platform::Logger::ERROR);
		break;
	};
}

void ShowMessage(str_type::string message, const ETH_MESSAGE type)
{
	str_type::stringstream ss;
	switch (type)
	{
	case ETH_ERROR:
		ss << GS_L("ERROR - ") << message;
		ETHResourceProvider::Log(ss.str(), Platform::Logger::ERROR);
		break;
	case ETH_WARNING:
		ss << GS_L("Warning - ") << message;
		ETHResourceProvider::Log(ss.str(), Platform::Logger::ERROR);
		break;
	case ETH_INFO:
		ss << message;
		ETHResourceProvider::Log(ss.str(), Platform::Logger::INFO);
		break;
	};
	#ifndef ANDROID
	std::wcout << std::endl;
	#endif
}

void PrintException(asIScriptContext *pContext)
{
	asIScriptEngine *m_pASEngine = pContext->GetEngine();
	const int funcId = pContext->GetExceptionFunction();
	const asIScriptFunction *pFunction = m_pASEngine->GetFunctionDescriptorById(funcId);
	ETH_STREAM_DECL(ss) << GS_L("Function: ") << pFunction->GetDeclaration() << std::endl
						<< GS_L("Section: ") << pFunction->GetScriptSectionName() << std::endl
						<< GS_L("Line: ") << pContext->GetExceptionLineNumber() << std::endl
						<< GS_L("Description: ") << pContext->GetExceptionString() << std::endl;
	ETHResourceProvider::Log(ss.str(), Platform::Logger::ERROR);
}

} // namespace ETHGlobal