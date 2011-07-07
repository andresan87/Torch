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

#include "NativeCommandForwarder.h"

namespace Platform {

using namespace gs2d;

NativeCommandForwarder::NativeCommandForwarder() :
	m_commands(GS_L(""))
{
}

void NativeCommandForwarder::ForwardCommands(str_type::string& out)
{
	out = m_commands;
	m_commands = GS_L("");
}

void NativeCommandForwarder::Command(const str_type::string& commandStr, const bool onTop)
{
	if (!onTop)
	{
		m_commands += (commandStr + GS_L("\n"));
	}
	else
	{
		m_commands = (commandStr + GS_L("\n")) + m_commands;
	}
}


} // namespace Platform