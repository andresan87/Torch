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

#ifndef NATIVE_COMMAND_FORWARDER_H_
#define NATIVE_COMMAND_FORWARDER_H_

#include "../../gs2dtypes.h"

namespace Platform {

class NativeCommandForwarder
{
protected:
	gs2d::str_type::string m_commands;
	NativeCommandForwarder();
	void ForwardCommands(gs2d::str_type::string& out);
public:
	void Command(const gs2d::str_type::string& commandSt, const bool onTop = false);
};

} // namespace Platform

#endif