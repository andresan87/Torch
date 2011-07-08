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

#ifndef ETH_RENDER_ENTITY_H_
#define ETH_RENDER_ENTITY_H_

#include "ETHSpriteEntity.h"
#include <boost/unordered/unordered_map.hpp>

class ETHRenderEntity : public ETHSpriteEntity
{
	friend class ETHLightmapGen;
public:
	ETHRenderEntity(const str_type::string& filePath, ETHResourceProviderPtr provider, const int nId =-1);
	ETHRenderEntity(TiXmlElement *pElement, ETHResourceProviderPtr provider, const int nId =-1);
	ETHRenderEntity(ETHResourceProviderPtr provider, const ETHEntityProperties& properties);
	ETHRenderEntity(ETHResourceProviderPtr provider);

	// rendering methods
	bool DrawLightPass(const Vector2 &zAxisDirection, const bool drawToTarget = false);
	bool DrawHalo(const float maxHeight, const float minHeight, const bool rotateHalo, const Vector2 &zAxisDirection);
	bool DrawAmbientPass(const float maxHeight, const float minHeight, const bool enableLightmaps, const ETHSceneProperties& sceneProps);
	bool DrawParticles(const unsigned int n, const float maxHeight, const float minHeight, const ETHSceneProperties& sceneProps);
	void DrawCollisionBox(const bool drawBase, SpritePtr pOutline, const GS_COLOR dwColor, const Vector2 &zAxisDirection);
	bool DrawShadow(const float maxHeight, const float minHeight, const ETHSceneProperties& sceneProps, const ETHLight& light, ETHSpriteEntity *pParent,
					const bool maxOpacity = false, const bool drawToTarget = false, const float targetAngle = 0.0f,
					const Vector3& v3TargetPos = Vector3(0,0,0));
	bool DrawProjShadow(const float maxHeight, const float minHeight, const ETHSceneProperties& sceneProps, const ETHLight& light,
						 ETHSpriteEntity *pParent, const bool maxOpacity, const bool drawToTarget,
						 const float targetAngle, const Vector3& v3TargetPos);
};

#endif