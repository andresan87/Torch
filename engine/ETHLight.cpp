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

#include "ETHLight.h"

ETHLight::ETHLight(const bool active) :
	active(active),
	staticLight(false),
	pos(Vector3(0,0,0)),
	color(Vector3(1,1,1)),
	range(256.0f),
	haloSize(64.0f),
	haloBrightness(1.0f),
	castShadows(false),
	haloBitmap(GS_L(""))
{
}

bool ETHLight::ReadFromXMLFile(TiXmlElement *pElement)
{
	int nActive;
	pElement->QueryIntAttribute(GS_L("active"), &nActive);
	active = static_cast<ETH_BOOL>(nActive);

	int nStatic;
	pElement->QueryIntAttribute(GS_L("static"), &nStatic);
	staticLight = static_cast<ETH_BOOL>(nStatic);

	int nCastShadows;
	pElement->QueryIntAttribute(GS_L("castShadows"), &nCastShadows);
	castShadows = static_cast<ETH_BOOL>(nCastShadows);

	pElement->QueryFloatAttribute(GS_L("range"), &range);
	pElement->QueryFloatAttribute(GS_L("haloBrightness"), &haloBrightness);
	pElement->QueryFloatAttribute(GS_L("haloSize"), &haloSize);

	TiXmlNode *pNode;
	TiXmlElement *pIter;

	pNode = pElement->FirstChild(GS_L("Position"));
	pIter = pNode->ToElement();
	if (pNode)
	{
		if (pIter)
		{
			pIter->QueryFloatAttribute(GS_L("x"), &pos.x);
			pIter->QueryFloatAttribute(GS_L("y"), &pos.y);
			pIter->QueryFloatAttribute(GS_L("z"), &pos.z);
		}
	}

	pNode = pElement->FirstChild(GS_L("Color"));
	if (pNode)
	{
		pIter = pNode->ToElement();
		if (pIter)
		{
			pIter->QueryFloatAttribute(GS_L("r"), &color.x);
			pIter->QueryFloatAttribute(GS_L("g"), &color.y);
			pIter->QueryFloatAttribute(GS_L("b"), &color.z);
		}
	}

	pNode = pElement->FirstChild(GS_L("HaloBitmap"));
	if (pNode)
	{
		TiXmlElement *pStringElement = pNode->ToElement();
		if (pStringElement)
		{
			haloBitmap = pStringElement->GetText();
		}
	}
	return true;
}

bool ETHLight::WriteToXMLFile(TiXmlElement *pRoot) const
{
	TiXmlElement *pLightRoot = new TiXmlElement(GS_L("Light"));
	pRoot->LinkEndChild(pLightRoot); 

	TiXmlElement *pElement;
	pElement = new TiXmlElement(GS_L("Position"));
	pLightRoot->LinkEndChild(pElement);
	pElement->SetDoubleAttribute(GS_L("x"), pos.x);
	pElement->SetDoubleAttribute(GS_L("y"), pos.y);
	pElement->SetDoubleAttribute(GS_L("z"), pos.z);

	pElement = new TiXmlElement(GS_L("Color"));
	pLightRoot->LinkEndChild(pElement);
	pElement->SetDoubleAttribute(GS_L("r"), color.x);
	pElement->SetDoubleAttribute(GS_L("g"), color.y);
	pElement->SetDoubleAttribute(GS_L("b"), color.z);

	if (haloBitmap != GS_L(""))
	{
		pElement = new TiXmlElement(GS_L("HaloBitmap"));
		pElement->LinkEndChild(new TiXmlText(haloBitmap));
		pLightRoot->LinkEndChild(pElement);
	}

	pLightRoot->SetAttribute(GS_L("active"), active);
	pLightRoot->SetAttribute(GS_L("static"), staticLight);
	pLightRoot->SetAttribute(GS_L("castShadows"), castShadows);
	pLightRoot->SetDoubleAttribute(GS_L("range"), range);
	pLightRoot->SetDoubleAttribute(GS_L("haloBrightness"), haloBrightness);
	pLightRoot->SetDoubleAttribute(GS_L("haloSize"), haloSize);
	return true;
}

bool ETHLight::IsActive() const
{
	return ETHGlobal::ToBool(active);
}