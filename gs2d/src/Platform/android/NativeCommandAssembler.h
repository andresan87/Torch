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

#ifndef NATIVE_COMMAND_ASSEMBLER_H_
#define NATIVE_COMMAND_ASSEMBLER_H_

#include "../../gs2dtypes.h"

namespace Platform {

class NativeCommandAssembler
{
public:
	static const gs2d::str_type::string CMD_PLAY_SOUND;
	static const gs2d::str_type::string CMD_LOAD_SOUND;
	static const gs2d::str_type::string CMD_DELETE_SOUND;
    static const gs2d::str_type::string CMD_QUIT_APP;

	/*
	load_sound <string:sample_name>
	play_sound <string:sample_name> <float:volume> <int:loop> <float:speed>
	delete_sound <string:sample_name>
	*/

	static gs2d::str_type::string QuitApplication();
	static gs2d::str_type::string LoadSound(const gs2d::str_type::string& fileName);
	static gs2d::str_type::string DeleteSound(const gs2d::str_type::string& fileName);
	static gs2d::str_type::string PlaySound(const gs2d::str_type::string& fileName, const float volume, const bool loop, const float speed);
};

} // namespace Platform

#endif