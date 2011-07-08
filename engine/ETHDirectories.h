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

#ifndef ETH_DIRECTORIES_H_
#define ETH_DIRECTORIES_H_

#include "../gs2d/src/gs2dtypes.h"

class ETHDirectories
{
public:
	static gs2d::str_type::string GetShaderPath();
	static gs2d::str_type::string GetBitmapFontPath();
	static gs2d::str_type::string GetScenePath();
	static gs2d::str_type::string GetEntityPath();
	static gs2d::str_type::string GetNormalMapPath();
	static gs2d::str_type::string GetHaloPath();
	static gs2d::str_type::string GetParticlesPath();
	static gs2d::str_type::string GetSoundFXPath();
};

#endif