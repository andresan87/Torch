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

#ifndef ETH_RESOURCE_MANAGER_H_
#define ETH_RESOURCE_MANAGER_H_

#include "ETHCommon.h"
#include <list>
#include <iostream>
#include <string>
#include <map>

class ETHGraphicResourceManager
{
public:
	SpritePtr GetPointer(VideoPtr video, const str_type::string &fileRelativePath,
						  const str_type::string &programPath, const str_type::string &searchPath, const bool cutOutBlackPixels);
	bool AddFile(VideoPtr video, const str_type::string &path, const bool cutOutBlackPixels);
	int GetNumResources();

	void ReleaseResources();

private:
	std::map<str_type::string, SpritePtr> m_resource;
};

typedef boost::shared_ptr<ETHGraphicResourceManager> ETHGraphicResourceManagerPtr;

class ETHAudioResourceManager
{
public:
	AudioSamplePtr GetPointer(AudioPtr audio, const str_type::string &fileRelativePath,
						  const str_type::string &programPath, const str_type::string &alternative, const GS_SAMPLE_TYPE type);
	bool AddFile(AudioPtr audio, const str_type::string &path, const GS_SAMPLE_TYPE type);
	int GetNumResources();

	void ReleaseResources();

private:
	std::map<str_type::string, AudioSamplePtr> m_resource;
};

typedef boost::shared_ptr<ETHAudioResourceManager> ETHAudioResourceManagerPtr;

#endif