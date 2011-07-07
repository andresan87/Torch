#include "MedievalTorch.h"
#include <iostream>

using namespace gs2d;
using namespace gs2d::math;

gs2d::BaseApplicationPtr gs2d::CreateBaseApplication()
{
	return BaseApplicationPtr(new MedievalTorch);
}

MedievalTorch::MedievalTorch() :
	pTorch(0)
{
}

MedievalTorch::~MedievalTorch()
{
	if (pTorch)
		pTorch->Release();
}

void MedievalTorch::Start(VideoPtr video, InputPtr input, AudioPtr audio)
{
	this->video = video;
	this->input = input;
	this->audio = audio;

	m_props.ambient = Vector3(1,1,1);

	Platform::FileManagerPtr fileManager = video->GetFileManager();
	m_provider = ETHResourceProviderPtr(new ETHResourceProvider(
		ETHGraphicResourceManagerPtr(new ETHGraphicResourceManager()),
		ETHAudioResourceManagerPtr(new ETHAudioResourceManager()),
		boost::shared_ptr<ETHShaderManager>(new ETHShaderManager(video, GS_L("assets/data/"))),
		GS_L("assets/"), video, audio, input));

	const Vector2 screenSize = video->GetScreenSizeF();

	if (pTorch)
		pTorch->Release();
	pTorch = new ETHRenderEntity(GS_L("assets/entities/medieval_torch.ent"), m_provider);
	pTorch->SetOrphanPositionXY(Vector2(screenSize.x / 2.0f, screenSize.y - (screenSize.y / 6.0f)));
	pTorch->ScaleParticleSystem(0, 1.6f);
	pTorch->ScaleParticleSystem(0, screenSize.x / 480.0f);
}

Application::APP_STATUS MedievalTorch::Update(unsigned long lastFrameDeltaTimeMS)
{
	m_lastFrameDeltaTimeMS = lastFrameDeltaTimeMS;

	const Vector3 accel3D(input->GetAccelerometerData() * 4.0f);
	deltaForce = Vector3(lastForce - accel3D);
	Vector3 userForce(accel3D);
	lastForce = accel3D;

	if (Abs(deltaForce.x) > 0.8f && DP3(Normalize(accel3D), Vector3(0, 1, 0)) > 0.7f)
	{
		userForce.x *= -4.0f;
	}

	const Vector2 sumForce(userForce.x, userForce.y);

	/*Vector3 accel3D(input->GetAccelerometerData());

	accel3D.x *= (static_cast<float>(lastFrameDeltaTimeMS)/1000.0f);
	velocityX += accel3D.x + ((accel3D.x - lastAccelX) / 2.0f);
	lastAccelX = accel3D.x;

	if (Abs(velocityX) > 2.0f)
	{
		velocityX = 0.0f;
	}

	const Vector2 sumForce(velocityX * 40.0f, accel3D.y * 4.0f);*/

	pTorch->SetOrphanPositionXY(sumForce + pTorch->GetPositionXY());
	video->SetCameraPos(sumForce + video->GetCameraPos());
	pTorch->UpdateParticleSystems(m_props.zAxisDirection);

	return Application::APP_OK;
}

void MedievalTorch::RenderFrame()
{
	const Vector2 screenSize = video->GetScreenSizeF();
	video->BeginSpriteScene();

	if (m_provider)
	{
		m_provider->GetShaderManager()->BeginParticlePass();
			pTorch->DrawParticles(0, 3000,-3000, m_props);
		m_provider->GetShaderManager()->EndParticlePass();
	}

	str_type::stringstream ss;
	ss << video->GetFPSRate() << GS_L("\n");

	/*ss << GS_L("Accel.: ")
	   << input->GetAccelerometerData().x << GS_L(", ")
	   << input->GetAccelerometerData().y << GS_L(", ")
	   << input->GetAccelerometerData().z << std::endl;*/

	video->DrawBitmapText(Vector2(0, 0), ss.str(), GS_L("Verdana20_shadow.fnt"), GS_COLOR(40,255,255,255));

	video->EndSpriteScene();
}

void MedievalTorch::Destroy()
{
}