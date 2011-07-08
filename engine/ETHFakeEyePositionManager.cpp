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

#include "ETHFakeEyePositionManager.h"

ETHFakeEyePositionManager::ETHFakeEyePositionManager()
	: m_fakeEyeHeight(768.0f)
{
}

void ETHFakeEyePositionManager::SetFakeEyeHeight(const float f)
{
	m_fakeEyeHeight = Max(f, 0.0f);
}

float ETHFakeEyePositionManager::GetFakeEyeHeight() const
{
	return m_fakeEyeHeight;
}

Vector3 ETHFakeEyePositionManager::ComputeFakeEyePosition(
	VideoPtr video, ShaderPtr pShader, const bool drawToTarget,
	const Vector3 &v3LightPos, const float entityAngle)
{
	const Vector2 &v2CamPos(video->GetCameraPos());
	const Vector2 &v2ScreenDim(video->GetScreenSizeF());
	Vector3 v3RelativeEyePos(0, v2ScreenDim.y*1.5f, GetFakeEyeHeight());
	if (!drawToTarget)
		v3RelativeEyePos.y -= v3LightPos.y-v2CamPos.y;

	Matrix4x4 matRot = RotateZ(-DegreeToRadian(entityAngle));
	v3RelativeEyePos = Multiply(v3RelativeEyePos, matRot);

	//pShader->SetConstant(GS_L("fakeEyePos"), v3RelativeEyePos+Vector3(0, v2CamPos.y, 0)+Vector3(v3LightPos.x,0,0));
	return v3RelativeEyePos+Vector3(0, v2CamPos.y, 0)+Vector3(v3LightPos.x,0,0);
}
