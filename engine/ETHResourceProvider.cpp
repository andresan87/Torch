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

#include "ETHResourceProvider.h"
#include "../gs2d/src/Platform/Platform.h"

Platform::FileLoggerPtr ETHResourceProvider::m_logger(new Platform::FileLogger(
#ifdef _DEBUG
Platform::GetCurrentDirectoryPath()
#else
Platform::GetModulePath()
#endif
+ Platform::FileLogger::GetLogPath() + GS_L("eth.log.txt")));

ETHResourceProvider::ETHResourceProvider(
		ETHGraphicResourceManagerPtr graphicResources, ETHAudioResourceManagerPtr audioResources,
		boost::shared_ptr<ETHShaderManager> shaderManager, const str_type::string& resourcePath,
		VideoPtr video, AudioPtr audio, InputPtr input)
{
	m_graphicResources = graphicResources;
	m_audioResources = audioResources;
	m_shaderManager = shaderManager;
	m_resourcePath = resourcePath;
	m_video = video;
	m_audio = audio;
	m_input = input;
}

void ETHResourceProvider::Log(const str_type::string& str, const Platform::Logger::TYPE& type)
{
	m_logger->Log(str, type);
}

#ifdef _DEBUG
str_type::string ETHResourceProvider::m_programPath = Platform::GetCurrentDirectoryPath();
#else
str_type::string ETHResourceProvider::m_programPath = Platform::GetModulePath();
#endif

ETHGraphicResourceManagerPtr ETHResourceProvider::m_graphicResources;
ETHAudioResourceManagerPtr ETHResourceProvider::m_audioResources;
boost::shared_ptr<ETHShaderManager> ETHResourceProvider::m_shaderManager;
str_type::string ETHResourceProvider::m_resourcePath;
VideoPtr ETHResourceProvider::m_video;
AudioPtr ETHResourceProvider::m_audio;
InputPtr ETHResourceProvider::m_input;
