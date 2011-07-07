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

#include "FileLogger.h"
#include "Platform.h"

#include <fstream>

#ifdef WIN32
#include <iostream>
#endif

namespace Platform {

bool AppendToFile(const gs2d::str_type::string& fileName, const gs2d::str_type::string& str)
{
	gs2d::str_type::ofstream ofs(fileName.c_str(), std::ios::out | std::ios::app);
	if (ofs.is_open())
	{
		ofs << str << std::endl;
		ofs.close();
		return true;
	}
	else
	{
		return false;
	}
}

FileLogger::FileLogger(const gs2d::str_type::string& fileName) : m_fileName(fileName)
{
	gs2d::str_type::ofstream ofs(fileName.c_str());
	if (ofs.is_open())
	{
		ofs << GS_L("[") << fileName << GS_L("]") << std::endl;
		ofs.close();
	}
}

void FileLogger::WriteToErrorLog(const gs2d::str_type::string& str)
{
	static bool errorLogFileCreated = false;
	if (!errorLogFileCreated)
	{
		gs2d::str_type::ofstream ofs(GetErrorLogFilePath().c_str());
		if (ofs.is_open())
		{
			ofs << GS_L("[") << GetErrorLogFilePath() << GS_L("]") << std::endl;
			ofs.close();
		}
	}
	AppendToFile(GetErrorLogFilePath(), str);
	errorLogFileCreated = true;
}

void FileLogger::WriteToWarningLog(const gs2d::str_type::string& str)
{
	static bool warningLogFileCreated = false;
	if (!warningLogFileCreated)
	{
		gs2d::str_type::ofstream ofs(GetWarningLogFilePath().c_str());
		if (ofs.is_open())
		{
			ofs << GS_L("[") << GetWarningLogFilePath() << GS_L("]") << std::endl;
			ofs.close();
		}
	}
	AppendToFile(GetWarningLogFilePath(), str);
	warningLogFileCreated = true;
}

bool FileLogger::Log(const gs2d::str_type::string& str, const TYPE& type) const
{
	#ifdef WIN32
	std::wcout << str << std::endl;
	if (type == ERROR)
	{
		std::wcerr << "\x07";
	}
	#endif
	if (type == ERROR)
		WriteToErrorLog(str);
	else if (type == WARNING)
		WriteToWarningLog(str);
	return AppendToFile(m_fileName, str);
}

gs2d::str_type::string FileLogger::GetErrorLogFilePath()
{
	return GetLogPath() + GS_L("_error.log.txt");
}

gs2d::str_type::string FileLogger::GetWarningLogFilePath()
{
	return GetLogPath() + GS_L("_warning.log.txt");
}

} // namespace Platform