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

#include "ETHResourceManager.h"
#include "../gs2d/src/Platform/Platform.h"

#include "ETHResourceProvider.h"

void ETHGraphicResourceManager::ReleaseResources()
{
	m_resource.clear();
}

SpritePtr ETHGraphicResourceManager::GetPointer(
	VideoPtr video,
	const str_type::string &fileRelativePath, const str_type::string &programPath,
	const str_type::string &searchPath, const bool cutOutBlackPixels)
{
	if (fileRelativePath == GS_L(""))
		return SpritePtr();

	if (!m_resource.empty())
	{
		str_type::string fileName = ETHGlobal::GetFileName(fileRelativePath);
		std::map<str_type::string, SpritePtr>::iterator iter = m_resource.find(fileName);
		if (iter != m_resource.end())
			return iter->second;
	}

	// we can set a search path to search the file in case
	// it hasn't been loaded yet
	if (searchPath != GS_L(""))
	{
		str_type::string fileName = ETHGlobal::GetFileName(fileRelativePath);

		str_type::string path = programPath;
		path += searchPath;
		path += fileName;
		AddFile(video, path, cutOutBlackPixels);
		return GetPointer(video, fileName, programPath, GS_L(""), false);
	}
	return SpritePtr();
}

bool ETHGraphicResourceManager::AddFile(VideoPtr video, const str_type::string &path, const bool cutOutBlackPixels)
{
	{
		str_type::string fileName = ETHGlobal::GetFileName(path);
		std::map<str_type::string, SpritePtr>::iterator iter = m_resource.find(fileName);
		if (iter != m_resource.end())
			return true;
	}

	SpritePtr pBitmap;
	str_type::string fixedName(path);
	Platform::FixSlashes(fixedName);
	if (!(pBitmap = video->CreateSprite(fixedName, (cutOutBlackPixels)? 0xFF000000 : 0xFFFF00FF)))
	{
		pBitmap.reset();
		ETH_STREAM_DECL(ss) << GS_L("(Not loaded) ") << path;
		ETHResourceProvider::Log(ss.str(), Platform::Logger::ERROR);
		return false;
	}
	//#ifdef _DEBUG
	str_type::string fileName = ETHGlobal::GetFileName(path);
	ETH_STREAM_DECL(ss) << GS_L("(Loaded) ") << fileName;
	ETHResourceProvider::Log(ss.str(), Platform::Logger::INFO);
	//#endif
	m_resource[fileName] = pBitmap;
	return true;
}

int ETHGraphicResourceManager::GetNumResources()
{
	return m_resource.size();
}

void ETHAudioResourceManager::ReleaseResources()
{
	m_resource.clear();
}

AudioSamplePtr ETHAudioResourceManager::GetPointer(AudioPtr audio,
										  const str_type::string &fileRelativePath, const str_type::string &programPath,
										  const str_type::string &searchPath, const GS_SAMPLE_TYPE type)
{
	if (fileRelativePath == GS_L(""))
		return AudioSamplePtr();

	if (!m_resource.empty())
	{
		str_type::string fileName = ETHGlobal::GetFileName(fileRelativePath);
		std::map<str_type::string, AudioSamplePtr>::iterator iter = m_resource.find(fileName);
		if (iter != m_resource.end())
			return iter->second;
	}

	// we can set a search path to search the file in case
	// it hasn't been loaded yet
	if (searchPath != GS_L(""))
	{
		str_type::string fileName = ETHGlobal::GetFileName(fileRelativePath);

		str_type::string path = programPath;
		path += searchPath;
		path += fileName;
		AddFile(audio, path, type);
		return GetPointer(audio, fileName, programPath, GS_L(""), type);
	}
	return AudioSamplePtr();
}

bool ETHAudioResourceManager::AddFile(AudioPtr audio, const str_type::string &path, const GS_SAMPLE_TYPE type)
{
	if (!m_resource.empty())
	{
		str_type::string fileName = ETHGlobal::GetFileName(path);
		std::map<str_type::string, AudioSamplePtr>::iterator iter = m_resource.find(fileName);
		if (iter != m_resource.end())
			return true;
	}

	AudioSamplePtr pSample;
	str_type::string fixedName(path);
	Platform::FixSlashes(fixedName);
	if (!(pSample = audio->LoadSampleFromFile(fixedName, type)))
	{
		pSample.reset();
		ETH_STREAM_DECL(ss) << GS_L("(Not loaded) \"") << fixedName << GS_L("\"");
		ETHResourceProvider::Log(ss.str(), Platform::Logger::ERROR);
		return false;
	}
	//#ifdef _DEBUG
	str_type::string fileName = ETHGlobal::GetFileName(path);
	ETH_STREAM_DECL(ss) << GS_L("(Loaded) ") << fileName;
	ETHResourceProvider::Log(ss.str(), Platform::Logger::INFO);
	//#endif
	m_resource[fileName] = pSample;
	return true;
}

int ETHAudioResourceManager::GetNumResources()
{
	return m_resource.size();
}
