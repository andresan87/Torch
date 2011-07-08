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

#include "ETHDirectories.h"

#define ETH_SHADER_PATH				GS_L("data/")
#define ETH_BITMAP_FONT_PATH		GS_L("data/")
#define ETH_SCENES_FOLDER			GS_L("scenes/")
#define ETH_ENTITY_FOLDER			GS_L("entities/")
#define ETH_NORMAL_FOLDER			GS_L("entities/normalmaps/")
#define ETH_HALOS_FOLDER			GS_L("entities/")
#define ETH_PARTICLES_FOLDER		GS_L("particles/")
#define ETH_SOUNDFX_FOLDER			GS_L("soundfx/")

gs2d::str_type::string ETHDirectories::GetShaderPath()
{
	return ETH_SHADER_PATH;
}

gs2d::str_type::string ETHDirectories::GetBitmapFontPath()
{
	return ETH_BITMAP_FONT_PATH;
}

gs2d::str_type::string ETHDirectories::GetScenePath()
{
	return ETH_SCENES_FOLDER;
}

gs2d::str_type::string ETHDirectories::GetEntityPath()
{
	return ETH_ENTITY_FOLDER;
}

gs2d::str_type::string ETHDirectories::GetNormalMapPath()
{
	return ETH_NORMAL_FOLDER;
}

gs2d::str_type::string ETHDirectories::GetHaloPath()
{
	return ETH_HALOS_FOLDER;
}

gs2d::str_type::string ETHDirectories::GetParticlesPath()
{
	return ETH_PARTICLES_FOLDER;
}

gs2d::str_type::string ETHDirectories::GetSoundFXPath()
{
	return ETH_SOUNDFX_FOLDER;
}
