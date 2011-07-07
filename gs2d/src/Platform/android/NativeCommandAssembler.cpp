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

#include "NativeCommandAssembler.h"

namespace Platform {

using namespace gs2d;

const str_type::string NativeCommandAssembler::CMD_PLAY_SOUND = "play_sound";
const str_type::string NativeCommandAssembler::CMD_LOAD_SOUND = "load_sound";
const str_type::string NativeCommandAssembler::CMD_DELETE_SOUND = "delete_sound";
const str_type::string NativeCommandAssembler::CMD_QUIT_APP = "quit_app";

gs2d::str_type::string  NativeCommandAssembler::QuitApplication()
{
	return CMD_QUIT_APP;
}

str_type::string NativeCommandAssembler::LoadSound(const str_type::string& fileName)
{
	str_type::stringstream ss;
	ss << CMD_LOAD_SOUND << " " << fileName;
	return ss.str();
}

str_type::string NativeCommandAssembler::DeleteSound(const str_type::string& fileName)
{
	str_type::stringstream ss;
	ss << CMD_DELETE_SOUND << " " << fileName;
	return ss.str();
}

str_type::string NativeCommandAssembler::PlaySound(const str_type::string& fileName, const float volume, const bool loop, const float speed)
{
	str_type::stringstream ss;
	ss << CMD_PLAY_SOUND << " " << fileName << " " << volume << " " << ((loop) ? "1" : "0") << " " << speed;
	return ss.str();
}

} // namespace Platform