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

#include "ETHShaders.h"

gs2d::str_type::string ETHShaders::DefaultVS()
{
	return GS_L("default.vs");
}

gs2d::str_type::string ETHShaders::DefaultPS()
{
	return GS_L("");
}

// ambient pass
gs2d::str_type::string ETHShaders::Ambient_VS_Hor()
{
	return GS_L("hAmbient.vs");
}

gs2d::str_type::string ETHShaders::Ambient_VS_Ver()
{
	return GS_L("vAmbient.vs");
}

// particles
gs2d::str_type::string ETHShaders::Particle_VS()
{
	//return GS_L("defaultVS.cg");
	return GS_L("particle.vs");
}

// shadow
gs2d::str_type::string ETHShaders::Shadow_VS_Ver()
{
	//return GS_L("dynaShadowVS.cg");
	return GS_L("default.vs");
}

// pixel light
gs2d::str_type::string ETHShaders::PL_PS_Hor_Diff()
{
	//return GS_L("hPixelLight.cg");
	return GS_L("default.ps");
}

gs2d::str_type::string ETHShaders::PL_PS_Ver_Diff()
{
	//return GS_L("vPixelLight.cg");
	return GS_L("default.ps");
}

gs2d::str_type::string ETHShaders::PL_PS_Hor_Spec()
{
	//return GS_L("hPixelLight.cg");
	return GS_L("default.ps");
}

gs2d::str_type::string ETHShaders::PL_PS_Ver_Spec()
{
	//return GS_L("vPixelLight.cg");
	return GS_L("default.ps");
}

gs2d::str_type::string ETHShaders::PL_VS_Hor_Light()
{
	//return GS_L("pixelLightVS.cg");
	return GS_L("default.vs");
}

gs2d::str_type::string ETHShaders::PL_VS_Ver_Light()
{
	//return GS_L("pixelLightVS.cg");
	return GS_L("default.vs");
}

gs2d::str_type::string ETHShaders::VL_VS_Hor_Diff()
{
	//return GS_L("hVertexLightShader.cg");
	return GS_L("default.vs");
}

gs2d::str_type::string ETHShaders::VL_VS_Ver_Diff()
{
	//return GS_L("vVertexLightShader.cg");
	return GS_L("default.vs");
}

