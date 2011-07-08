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

#ifndef ETH_FAKE_EYE_POSITION_MANAGER_H_
#define ETH_FAKE_EYE_POSITION_MANAGER_H_

#include "../gs2d/src/gs2d.h"

using namespace gs2d;
using namespace gs2d::math;

class ETHFakeEyePositionManager
{
public:
	ETHFakeEyePositionManager();
	void SetFakeEyeHeight(const float f);
	float GetFakeEyeHeight() const;
	Vector3 ComputeFakeEyePosition(VideoPtr video, ShaderPtr pShader, const bool drawToTarget,
							const Vector3 &v3LightPos, const float entityAngle);

private:
	float m_fakeEyeHeight;
};

typedef boost::shared_ptr<ETHFakeEyePositionManager> ETHFakeEyePositionManagerPtr;

#endif