#ifndef MEDIEVAL_TORCH_H_
#define MEDIEVAL_TORCH_H_

#include "../gs2d/src/gs2dframework.h"

#define _ETH_MAKE_PROPS_ALL_PUBLIC

#include "../engine/ETHRenderEntity.h"
#include "../engine/ETHShaderManager.h"

namespace gs2d {

class MedievalTorch : public gs2d::BaseApplication
{
	gs2d::InputPtr input;
	gs2d::VideoPtr video;
	gs2d::AudioPtr audio;
	unsigned long m_lastFrameDeltaTimeMS;

	Vector3 lastForce, deltaForce;

	ETHRenderEntity *pTorch;
	ETHResourceProviderPtr m_provider;
	ETHSceneProperties m_props;

public:
	MedievalTorch();
	~MedievalTorch();
	void Start(gs2d::VideoPtr video, gs2d::InputPtr input, gs2d::AudioPtr audio);
	gs2d::Application::APP_STATUS Update(unsigned long lastFrameDeltaTimeMS);
	void RenderFrame();
	void Destroy();	
};

} // namespace gs2d
#endif