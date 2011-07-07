#include "../gs2d.h"
#include "../gs2dinput.h"
#include "../gs2daudio.h"
#include "../gs2dutil.h"

#include "../Platform/Platform.h"

using namespace gs2d;
using namespace gs2d::math;

void DrawToTarget(VideoPtr video, SpritePtr target, SpritePtr sprite)
{
	video->SetRenderTarget(target);
	video->BeginTargetScene(GS_RED, false);

	video->SetAlphaMode(GSAM_PIXEL);
	sprite->Draw(Vector2(Randomizer::Float(256.0f), Randomizer::Float(256.0f)), Randomizer::Int(0xFFFFFFFF), 0.0f);

	video->EndTargetScene();
	video->SetRenderTarget(SpritePtr());
}

int main()
{
	VideoPtr video;

	if ((video = CreateVideo(800, 600, L"Primeira janela", true, true, L"fonts/")))
	{
		InputPtr input = CreateInput(0, true);
		AudioPtr audio = CreateAudio(0);

		SpritePtr square = video->CreateSprite(GS_L("square.png"));
		SpritePtr circle = video->CreateSprite(GS_L("circle.png"));
		circle->SetOrigin(GSEO_CENTER);

		SpritePtr target = video->CreateRenderTarget(256, 256, GSTF_ARGB);

		Video::APP_STATUS status;
		while ((status = video->HandleEvents()) != Video::APP_QUIT)
		{
			if (status == Video::APP_SKIP)
				continue;

			input->Update();

			DrawToTarget(video, target, circle);
			video->BeginSpriteScene();

			target->Draw(Vector2(200, 100));

			video->EndSpriteScene();
		}
	}
	return 0;
}