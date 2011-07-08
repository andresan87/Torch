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

#ifndef ETH_LIGHT_H_
#define ETH_LIGHT_H_

#include "ETHCommon.h"

struct ETHLight
{
	ETHLight(const bool active);
	bool ReadFromXMLFile(TiXmlElement *pElement);
	bool WriteToXMLFile(TiXmlElement *pRoot) const;
	bool IsActive() const;
	ETH_BOOL staticLight;
	Vector3 pos;
	Vector3 color;
	float range;
	float haloSize;
	float haloBrightness;
	ETH_BOOL castShadows;
	str_type::string haloBitmap;
private:
	ETH_BOOL active;
};

#endif