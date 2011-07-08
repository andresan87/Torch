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

#include "ETHEntityProperties.h"
#include <iostream>

void ETHEntityMaterial::Reset()
{
	emissiveColor = Vector4(0,0,0,0);
	castShadow = false;
	applyLight = true;
	spriteFile = GS_L("");
	normalFile = GS_L("");
	glossFile = GS_L("");
	shadowScale = 0.0f;
	shadowOpacity = 1.0f;
	specularPower = 50.0f;
	specularBrightness = 1.0f;
	shadowLengthScale = 1.0f;
	blendMode = GSAM_PIXEL;
}

ETHEntityProperties::ETHEntityProperties()
{
	ETHEntityMaterial::Reset();
	Reset();
}

ETHEntityProperties::ETHEntityProperties(TiXmlElement *pElement)
{
	ETHEntityMaterial::Reset();
	Reset();
	ReadFromXMLFile(pElement);
}

ETHEntityProperties::ETHEntityProperties(const str_type::string& filePath, const Platform::FileManagerPtr& fileManager)
{
	ETHEntityMaterial::Reset();
	Reset();

	TiXmlDocument doc(filePath);
	str_type::string content;
	fileManager->GetUTF16FileString(filePath, content);
	if (!doc.LoadFile(content, TIXML_ENCODING_LEGACY))
	{
		ETH_STREAM_DECL(ss) << GS_L("Couldn't load file: ") << filePath;
		ETHResourceProvider::Log(ss.str(), Platform::Logger::ERROR);
		return;
	}

	TiXmlHandle hDoc(&doc);
	TiXmlHandle hRoot(0);

	TiXmlElement *pElem = hDoc.FirstChildElement().Element();
	if (!pElem)
	{
		ETH_STREAM_DECL(ss) << GS_L("The current file seems to be invalid: ") << filePath;
		ETHResourceProvider::Log(ss.str(), Platform::Logger::ERROR);
		return;
	}

	hRoot = TiXmlHandle(pElem);
	entityName = ETHGlobal::GetFileName(filePath);
	ReadFromXMLFile(hRoot.FirstChildElement().Element());
}

void ETHEntityProperties::Reset()
{
	entityName = GS_L("");
	spriteCut = Vector2i(1,1);
	pivotAdjust = Vector2(0,0);
	startFrame = 0;
	staticEntity = false;
	type = ETH_HORIZONTAL;
	soundVolume = 1.0f;
	layerDepth = 0.0f;
}

bool ETHEntityProperties::SaveToFile(const str_type::string& filePath)
{
	TiXmlDocument doc;
	TiXmlDeclaration *pDecl = new TiXmlDeclaration(GS_L("1.0"), GS_L(""), GS_L(""));
	doc.LinkEndChild(pDecl);

	TiXmlElement *pElement = new TiXmlElement(GS_L("Ethanon"));
	doc.LinkEndChild(pElement);

	WriteToXMLFile(doc.RootElement());
	doc.SaveFile(filePath);
	return true;
}

bool ETHEntityProperties::ReadFromXMLFile(TiXmlElement *pElement)
{
	pElement->QueryIntAttribute(GS_L("type"), (int*)&type);

	int nStaticEntity;
	pElement->QueryIntAttribute(GS_L("static"), &nStaticEntity);
	staticEntity = static_cast<ETH_BOOL>(nStaticEntity);

	int nApplyLight;
	pElement->QueryIntAttribute(GS_L("applyLight"), &nApplyLight);
	applyLight = static_cast<ETH_BOOL>(nApplyLight);

	int nCastShadow;
	pElement->QueryIntAttribute(GS_L("castShadow"), &nCastShadow);
	castShadow = static_cast<ETH_BOOL>(nCastShadow);

	pElement->QueryIntAttribute(GS_L("startFrame"), &startFrame);
	pElement->QueryIntAttribute(GS_L("blendMode"), (int*)&blendMode);
	pElement->QueryFloatAttribute(GS_L("layerDepth"), &layerDepth);
	pElement->QueryFloatAttribute(GS_L("soundVolume"), &soundVolume);
	pElement->QueryFloatAttribute(GS_L("shadowScale"), &shadowScale);
	pElement->QueryFloatAttribute(GS_L("shadowLengthScale"), &shadowLengthScale);
	pElement->QueryFloatAttribute(GS_L("shadowOpacity"), &shadowOpacity);
	pElement->QueryFloatAttribute(GS_L("specularPower"), &specularPower);
	pElement->QueryFloatAttribute(GS_L("specularBrightness"), &specularBrightness);

	TiXmlElement *pIter;
	TiXmlNode *pNode;

	pNode = pElement->FirstChild(GS_L("EmissiveColor"));
	if (pNode)
	{
		pIter = pNode->ToElement();
		if (pIter)
		{
			pIter->QueryFloatAttribute(GS_L("r"), &emissiveColor.x);
			pIter->QueryFloatAttribute(GS_L("g"), &emissiveColor.y);
			pIter->QueryFloatAttribute(GS_L("b"), &emissiveColor.z);
			pIter->QueryFloatAttribute(GS_L("a"), &emissiveColor.w);
		}
	}

	pNode = pElement->FirstChild(GS_L("SpriteCut"));
	if (pNode)
	{
		pIter = pNode->ToElement();
		if (pIter)
		{
			pIter->QueryIntAttribute(GS_L("x"), &spriteCut.x);
			pIter->QueryIntAttribute(GS_L("y"), &spriteCut.y);
		}
	}

	pNode = pElement->FirstChild(GS_L("PivotAdjust"));
	if (pNode)
	{
		pIter = pNode->ToElement();
		if (pIter)
		{
			pIter->QueryFloatAttribute(GS_L("x"), &pivotAdjust.x);
			pIter->QueryFloatAttribute(GS_L("y"), &pivotAdjust.y);
		}
	}

	pNode = pElement->FirstChild(GS_L("Sprite"));
	if (pNode)
	{
		pIter = pNode->ToElement();
		if (pIter)
		{
			spriteFile = pIter->GetText();
		}
	}

	pNode = pElement->FirstChild(GS_L("Normal"));
	if (pNode)
	{
		pIter = pNode->ToElement();
		if (pIter)
		{
			normalFile = pIter->GetText();
		}
	}

	pNode = pElement->FirstChild(GS_L("Gloss"));
	if (pNode)
	{
		pIter = pNode->ToElement();
		if (pIter)
		{
			glossFile = pIter->GetText();
		}
	}

	pNode = pElement->FirstChild(GS_L("Particles"));
	if (pNode)
	{
		TiXmlElement *pParticles = pNode->ToElement();
		if (pParticles)
		{
			pNode = pParticles->FirstChild(GS_L("ParticleSystem"));
			if (pNode)
			{
				TiXmlElement *pParticleIter = pNode->ToElement();
				if (pParticleIter)
				{
					// TODO: use unlimited amount of systems 
					for (unsigned int t=0; t<_ETH_MAX_PARTICLE_SYS_PER_ENTITY; t++)
					{
						if (pParticleIter)
						{
							boost::shared_ptr<ETH_PARTICLE_SYSTEM> newSystem = boost::shared_ptr<ETH_PARTICLE_SYSTEM>(new ETH_PARTICLE_SYSTEM);
							newSystem->ReadFromXMLFile(pParticleIter);
							if (newSystem->nParticles > 0)
							{
								particleSystems.push_back(newSystem);
							}
							pParticleIter = pParticleIter->NextSiblingElement();
						}
					}
				}
			}
		}
	}

	pNode = pElement->FirstChild(GS_L("Light"));
	if (pNode)
	{
		TiXmlElement *pLight = pNode->ToElement();
		if (pLight)
		{
			boost::shared_ptr<ETHLight> newLight = boost::shared_ptr<ETHLight>(new ETHLight(true));
			newLight->ReadFromXMLFile(pLight);
			if (newLight->IsActive() && newLight->range > 0.0f)
			{
				light = newLight;
				// forces the light 'static' flag to be the same as its owner's
				light->staticLight = staticEntity;
			}
		}
	}

	int nCollidable;
	pElement->QueryIntAttribute(GS_L("collidable"), &nCollidable);
	if (nCollidable)
	{
		collision = boost::shared_ptr<ETH_COLLISION_BOX>(new ETH_COLLISION_BOX);
		pNode = pElement->FirstChild(GS_L("Collision"));
		if (pNode)
		{
			TiXmlElement *pCollision = pNode->ToElement();
			if (pCollision)
			{
				collision->ReadFromXMLFile(pCollision);
			}
		}
	}

	ReadDataFromXMLFile(pElement);
	return true;
}

bool ETHEntityProperties::WriteToXMLFile(TiXmlElement *pHeadRoot) const
{
	TiXmlElement *pRoot = new TiXmlElement(GS_L("Entity"));
	pHeadRoot->LinkEndChild(pRoot); 

	TiXmlElement *pElement;
	pElement = new TiXmlElement(GS_L("EmissiveColor"));
	pRoot->LinkEndChild(pElement); 
	pElement->SetDoubleAttribute(GS_L("r"), emissiveColor.x);
	pElement->SetDoubleAttribute(GS_L("g"), emissiveColor.y);
	pElement->SetDoubleAttribute(GS_L("b"), emissiveColor.z);
	pElement->SetDoubleAttribute(GS_L("a"), emissiveColor.w);

	pElement = new TiXmlElement(GS_L("SpriteCut"));
	pRoot->LinkEndChild(pElement);
	pElement->SetDoubleAttribute(GS_L("x"), spriteCut.x);
	pElement->SetDoubleAttribute(GS_L("y"), spriteCut.y);

	pElement = new TiXmlElement(GS_L("PivotAdjust"));
	pRoot->LinkEndChild(pElement);
	pElement->SetDoubleAttribute(GS_L("x"), pivotAdjust.x);
	pElement->SetDoubleAttribute(GS_L("y"), pivotAdjust.y);

	if (spriteFile != GS_L(""))
	{
		pElement = new TiXmlElement(GS_L("Sprite"));
		pElement->LinkEndChild(new TiXmlText(spriteFile));
		pRoot->LinkEndChild(pElement);
	}

	if (normalFile != GS_L(""))
	{
		pElement = new TiXmlElement(GS_L("Normal"));
		pElement->LinkEndChild(new TiXmlText(normalFile));
		pRoot->LinkEndChild(pElement);
	}

	if (glossFile != GS_L(""))
	{
		pElement = new TiXmlElement(GS_L("Gloss"));
		pElement->LinkEndChild(new TiXmlText(glossFile));
		pRoot->LinkEndChild(pElement);
	}

	TiXmlElement *pParticles = new TiXmlElement(GS_L("Particles"));
	pRoot->LinkEndChild(pParticles);
	for (unsigned int t=0; t<particleSystems.size(); t++)
	{
		if (particleSystems[t]->nParticles > 0)
			particleSystems[t]->WriteToXMLFile(pParticles);
	}

	if (light)
	{
		light->WriteToXMLFile(pRoot);
	}

	if (collision)
	{
		collision->WriteToXMLFile(pRoot);
	}

	pRoot->SetAttribute(GS_L("type"), type);
	pRoot->SetAttribute(GS_L("static"), staticEntity);
	pRoot->SetAttribute(GS_L("startFrame"), startFrame);
	pRoot->SetAttribute(GS_L("applyLight"), applyLight);
	pRoot->SetAttribute(GS_L("castShadow"), castShadow);
	pRoot->SetAttribute(GS_L("blendMode"), blendMode);
	pRoot->SetDoubleAttribute(GS_L("layerDepth"), layerDepth);
	pRoot->SetDoubleAttribute(GS_L("soundVolume"), soundVolume);
	pRoot->SetDoubleAttribute(GS_L("shadowScale"), shadowScale);
	pRoot->SetDoubleAttribute(GS_L("shadowLengthScale"), shadowLengthScale);
	pRoot->SetDoubleAttribute(GS_L("shadowOpacity"), shadowOpacity);
	pRoot->SetDoubleAttribute(GS_L("specularPower"), specularPower);
	pRoot->SetDoubleAttribute(GS_L("specularBrightness"), specularBrightness);

	WriteDataToFile(pRoot);
	return true;
}