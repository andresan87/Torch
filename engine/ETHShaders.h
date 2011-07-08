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

#ifndef ETH_SHADERS_H_
#define ETH_SHADERS_H_

#include "../gs2d/src/gs2dtypes.h"

class ETHShaders
{
public:
	static gs2d::str_type::string DefaultVS();
	static gs2d::str_type::string DefaultPS();

	// ambient pass
	static gs2d::str_type::string Ambient_VS_Hor();
	static gs2d::str_type::string Ambient_VS_Ver();

	// particles
	static gs2d::str_type::string Particle_VS();

	// shadow
	static gs2d::str_type::string Shadow_VS_Ver();

	// pixel light
	static gs2d::str_type::string PL_PS_Hor_Diff();
	static gs2d::str_type::string PL_PS_Ver_Diff();
	static gs2d::str_type::string PL_PS_Hor_Spec();
	static gs2d::str_type::string PL_PS_Ver_Spec();

	static gs2d::str_type::string PL_VS_Hor_Light();
	static gs2d::str_type::string PL_VS_Ver_Light();

	// vertex light
	static gs2d::str_type::string VL_VS_Hor_Diff();
	static gs2d::str_type::string VL_VS_Ver_Diff();
};

#endif