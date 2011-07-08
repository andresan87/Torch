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

#ifndef ETH_ENTITY_PROPERTIES_H_
#define ETH_ENTITY_PROPERTIES_H_

#include "ETHLight.h"
#include "ETHParticleManager.h"
#include "ETHCustomDataManager.h"
#include <vector>

class ETHEntityMaterial
{
public:
	void Reset();
	Vector4 emissiveColor;
	ETH_BOOL castShadow;
	ETH_BOOL applyLight;
	str_type::string glossFile;
	str_type::string normalFile;
	str_type::string spriteFile;
	float shadowOpacity;
	float shadowScale;
	float specularPower;
	float specularBrightness;
	boost::shared_ptr<ETHLight> light;
	float shadowLengthScale;
	GS_ALPHA_MODE blendMode;
};

class ETHEntitySpriteProperties : public ETHEntityMaterial
{
public:
	int startFrame;
	Vector2 pivotAdjust;
	Vector2i spriteCut;
};

class ETHEntityProperties : public ETHEntitySpriteProperties, public ETHCustomDataManager
{
	friend class ETHEntity;
	friend class ETHSpriteEntity;
	friend class ETHRenderEntity;
	friend class ETHScene;
	friend class ETHLightmapGen;
public:
	ETHEntityProperties(const str_type::string& filePath, const Platform::FileManagerPtr& fileManager);
	ETHEntityProperties(TiXmlElement *pElement);
	ETHEntityProperties();
	bool IsCollidable() const;
	void Reset();
	bool SaveToFile(const str_type::string& filePath);
	bool ReadFromXMLFile(TiXmlElement *pElement);
	bool WriteToXMLFile(TiXmlElement *pHeadRoot) const;

#if !defined(_ETHANON_EDITOR) && !defined(_ETH_MAKE_PROPS_ALL_PUBLIC)
protected:
#endif
	str_type::string entityName;
	ETH_BOOL staticEntity;
	ETH_ENTITY_TYPE type;
	float layerDepth;
	float soundVolume;
	boost::shared_ptr<ETH_COLLISION_BOX> collision;
	std::vector<boost::shared_ptr<ETH_PARTICLE_SYSTEM> > particleSystems;
};

#endif