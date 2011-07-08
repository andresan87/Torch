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

#ifndef ETH_RESOURCE_PROVIDER_H_
#define ETH_RESOURCE_PROVIDER_H_

#include "ETHResourceManager.h"
#include "Platform/ETHPlatform.h"
#include "../gs2d/src/Platform/Platform.h"
#include "../gs2d/src/Platform/FileLogger.h"

class ETHShaderManager;

class ETHResourceProvider
{
	static ETHGraphicResourceManagerPtr m_graphicResources;
	static ETHAudioResourceManagerPtr m_audioResources;
	static boost::shared_ptr<ETHShaderManager> m_shaderManager;
	static str_type::string m_programPath, m_resourcePath;
	static VideoPtr m_video;
	static AudioPtr m_audio;
	static InputPtr m_input;
	static Platform::FileLoggerPtr m_logger;

public:
	ETHResourceProvider(ETHGraphicResourceManagerPtr graphicResources, ETHAudioResourceManagerPtr audioResources,
				boost::shared_ptr<ETHShaderManager> shaderManager, const str_type::string& resourcePath,
				VideoPtr video, AudioPtr audio, InputPtr input);

	static void Log(const str_type::string& str, const Platform::Logger::TYPE& type);

	ETH_INLINE const Platform::FileLogger* GetLogger() const { return m_logger.get(); }
	ETH_INLINE ETHGraphicResourceManagerPtr GetGraphicResourceManager() { return m_graphicResources; }
	ETH_INLINE ETHAudioResourceManagerPtr GetAudioResourceManager() { return m_audioResources; }
	ETH_INLINE boost::shared_ptr<ETHShaderManager> GetShaderManager() { return m_shaderManager; }
	ETH_INLINE str_type::string GetProgramPath() { return m_programPath; }
	ETH_INLINE str_type::string GetResourcePath() { return m_resourcePath; }
	ETH_INLINE void SetResourcePath(const str_type::string& path) { m_resourcePath = Platform::AddLastSlash(path); }
	ETH_INLINE VideoPtr GetVideo() { return m_video; }
	ETH_INLINE AudioPtr GetAudio() { return m_audio; }
	ETH_INLINE InputPtr GetInput() { return m_input; }
};

typedef boost::shared_ptr<ETHResourceProvider> ETHResourceProviderPtr;

#endif