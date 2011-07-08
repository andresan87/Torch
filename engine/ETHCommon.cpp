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

#include "ETHCommon.h"
#include <string.h>
#include "../gs2d/src/enml/enml.h"

ETH_VIDEO_MODE::ETH_VIDEO_MODE()
{
	width = height = 0;
	pf = GSPF_UNKNOWN;
}

ETH_COLLISION_BOX::ETH_COLLISION_BOX()
{
	pos = size = Vector3(0,0,0);
}

ETH_COLLISION_BOX::ETH_COLLISION_BOX(const Vector3 &v3Pos, const Vector3 &v3Size)
{
	pos = v3Pos;
	size = v3Size;
}

bool ETH_COLLISION_BOX::ReadFromXMLFile(TiXmlElement *pElement)
{
	TiXmlElement *pIter;
	pIter = pElement->FirstChild(GS_L("Position"))->ToElement();
	if (pIter)
	{
		pIter->QueryFloatAttribute(GS_L("x"), &pos.x);
		pIter->QueryFloatAttribute(GS_L("y"), &pos.y);
		pIter->QueryFloatAttribute(GS_L("z"), &pos.z);
	}
	pIter = pElement->FirstChild(GS_L("Size"))->ToElement();
	if (pIter)
	{
		pIter->QueryFloatAttribute(GS_L("x"), &size.x);
		pIter->QueryFloatAttribute(GS_L("y"), &size.y);
		pIter->QueryFloatAttribute(GS_L("z"), &size.z);
	}
	return true;
}

bool ETH_COLLISION_BOX::WriteToXMLFile(TiXmlElement *pRoot) const
{
	TiXmlElement *pLightRoot = new TiXmlElement(GS_L("Collision"));
	pRoot->LinkEndChild(pLightRoot); 

	TiXmlElement *pElement;
	pElement = new TiXmlElement(GS_L("Position"));
	pLightRoot->LinkEndChild(pElement);
	pElement->SetDoubleAttribute(GS_L("x"), pos.x);
	pElement->SetDoubleAttribute(GS_L("y"), pos.y);
	pElement->SetDoubleAttribute(GS_L("z"), pos.z);

	pElement = new TiXmlElement(GS_L("Size"));
	pLightRoot->LinkEndChild(pElement);
	pElement->SetDoubleAttribute(GS_L("x"), size.x);
	pElement->SetDoubleAttribute(GS_L("y"), size.y);
	pElement->SetDoubleAttribute(GS_L("z"), size.z);
	return true;
}

ETH_COLLISION_IN_SCENE::ETH_COLLISION_IN_SCENE()
{
	pos = size = Vector3(0,0,0);
	ownerID = 0;
}

ETH_COLLISION_IN_SCENE::ETH_COLLISION_IN_SCENE(const Vector3 &v3Pos, const Vector3 &v3Size, const int id)
{
	ownerID = id;
	pos = v3Pos;
	size = v3Size;
}

ETH_COLLISION_IN_SCENE::ETH_COLLISION_IN_SCENE(const ETH_COLLISION_BOX &box, const int id)
{
	ownerID = id;
	pos = box.pos;
	size = box.size;
}

bool ETH_COLLISION_BOX::Collide(const ETH_COLLISION_BOX &other, const Vector3 &entityPos0, const Vector3 &entityPos1) const
{
	// TODO: more robust collision implementation for line intersection, rotation and sliding
	const Vector3 halfSize0 = size/2;
	const Vector3 halfSize1 = other.size/2;

	const Vector3 v3Min0 = pos+entityPos0-halfSize0;
	const Vector3 v3Max0 = pos+entityPos0+halfSize0;

	const Vector3 v3Min1 = other.pos+entityPos1-halfSize1;
	const Vector3 v3Max1 = other.pos+entityPos1+halfSize1;

	if (v3Min0.x > v3Max1.x)
		return false;
	if (v3Min0.y > v3Max1.y)
		return false;

	if (v3Max0.x < v3Min1.x)
		return false;
	if (v3Max0.y < v3Min1.y)
		return false;

	// peform the z-axis comparision at the end because it's less likely to happen
	if (v3Max0.z < v3Min1.z)
		return false;
	if (v3Min0.z > v3Max1.z)
		return false;

	return true;
}

ETH_WINDOW_ENML_FILE::ETH_WINDOW_ENML_FILE(const str_type::string& fileName, const Platform::FileManagerPtr& fileManager)
{
	width = 640;
	height = 480;
	windowed = true;
	vsync = true;
	title = GS_L("Ethanon Engine");

	str_type::string out;
	fileManager->GetAnsiFileString(fileName, out);
	enml::File file(out);
	if (file.getError() == enml::enmlevSUCCESS)
	{
		file.getUInt(GS_L("window"), GS_L("width"), &width);
		file.getUInt(GS_L("window"), GS_L("height"), &height);
		windowed = (file.get(GS_L("window"), GS_L("windowed")) == GS_L("false")) ? false : true;
		vsync = (file.get(GS_L("window"), GS_L("vsync")) == GS_L("false")) ? false : true;

		const str_type::string newTitle = file.get(GS_L("window"), GS_L("title"));
		title = newTitle.empty() ? title : newTitle;
	}
	else
	{
		#ifdef GS2D_STR_TYPE_WCHAR
		std::wcerr
		#else
		std::cerr
		#endif
		 << file.getErrorString() << std::endl;
	}
}

ETH_STARTUP_RESOURCES_ENML_FILE::ETH_STARTUP_RESOURCES_ENML_FILE(const str_type::string& fileName, const Platform::FileManagerPtr& fileManager)
{
	emtprojFilename = GS_L("");
	escFilename = GS_L("");

	str_type::string out;
	fileManager->GetAnsiFileString(fileName, out);
	enml::File file(out);
	if (file.getError() == enml::enmlevSUCCESS)
	{
		emtprojFilename = file.get(GS_L("startup"), GS_L("project"));
		escFilename = file.get(GS_L("startup"), GS_L("scene"));
	}
	else
	{
		#ifdef GS2D_STR_TYPE_WCHAR
		std::wcerr
		#else
		std::cerr
		#endif
		 << file.getErrorString() << std::endl;
	}
}
