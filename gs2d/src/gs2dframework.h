/*-----------------------------------------------------------------------

 gameSpace2d (C) Copyright 2008-2011 Andre Santee
 http://www.asantee.net/gamespace/
 http://groups.google.com/group/gamespacelib

    This file is part of gameSpace2d.

    gameSpace2d is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    gameSpace2d is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with gameSpace2d. If not, see
    <http://www.gnu.org/licenses/>.

-----------------------------------------------------------------------*/

#ifndef GS2D_FRAMEWORK_H_
#define GS2D_FRAMEWORK_H_

#include "gs2d.h"
#include "gs2dinput.h"
#include "gs2daudio.h"

namespace gs2d {

class BaseApplication
{
public:
	virtual void Start(VideoPtr video, InputPtr input, AudioPtr audio) = 0;
	virtual Application::APP_STATUS Update(unsigned long lastFrameDeltaTimeMS) = 0;
	virtual void RenderFrame() = 0;
	virtual void Destroy() = 0;
	virtual std::string RunOnUIThread(const std::string& input) { return (""); }
};

typedef boost::shared_ptr<BaseApplication> BaseApplicationPtr;

BaseApplicationPtr CreateBaseApplication();

} // namespace gs2d

#endif
