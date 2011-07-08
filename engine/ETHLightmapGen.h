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

#ifndef ETH_LIGHTMAP_GEN_H_
#define ETH_LIGHTMAP_GEN_H_

#include "ETHRenderEntity.h"

class ETHLightmapGen
{
public:
	ETHLightmapGen(ETHRenderEntity* entity,
				   boost::shared_ptr<ETHShaderManager> shaderManager, std::list<ETHLight>::iterator iBegin, std::list<ETHLight>::iterator iEnd,
				   ETHBucketManager& buckets, const Vector3& oldPos, const Vector3& newPos, const float minHeight, const float maxHeight,
				   const ETHSceneProperties &sceneProps);
};

#endif