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

#ifndef FILE_LOGGER_H_
#define FILE_LOGGER_H_

#include "Logger.h"

namespace Platform {

class FileLogger : public Logger
{
public:
	FileLogger(const gs2d::str_type::string& fileName);
	bool Log(const gs2d::str_type::string& str, const TYPE& type) const;
	static gs2d::str_type::string GetLogPath();
private:
	static gs2d::str_type::string GetWarningLogFilePath();
	static gs2d::str_type::string GetErrorLogFilePath();
	static void WriteToErrorLog(const gs2d::str_type::string& str);
	static void WriteToWarningLog(const gs2d::str_type::string& str);
	gs2d::str_type::string m_fileName;
};

typedef boost::shared_ptr<FileLogger> FileLoggerPtr;

} // namespace Platform

#endif