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

#include <string>
#include <sstream>
#include <jni.h>

#include "../../../../../src/gs2dframework.h"
#include "../../../../../src/Platform/android/Platform.android.h"

using namespace gs2d;
using namespace gs2d::math;

extern "C" {
	JNIEXPORT jstring JNICALL Java_net_asantee_gs2d_GS2DJNI_mainLoop(JNIEnv* env, jobject thiz, jstring inputStr);
	JNIEXPORT void    JNICALL Java_net_asantee_gs2d_GS2DJNI_resize(JNIEnv* env, jobject thiz, jint width, jint height);
	JNIEXPORT void    JNICALL Java_net_asantee_gs2d_GS2DJNI_start(JNIEnv* env, jobject thiz, jstring apkPath, jint width, jint height);
	JNIEXPORT jstring JNICALL Java_net_asantee_gs2d_GS2DJNI_destroy(JNIEnv* env, jobject thiz);
	JNIEXPORT jstring JNICALL Java_net_asantee_gs2d_GS2DJNI_runOnUIThread(JNIEnv* env, jobject thiz, jstring inputStr);
};

BaseApplicationPtr application = CreateBaseApplication();

VideoPtr video;
InputPtr input;
AudioPtr audio;
boost::shared_ptr<Platform::ZipFileManager> zip;

std::string g_inputStr;

JNIEXPORT void JNICALL Java_net_asantee_gs2d_GS2DJNI_start(JNIEnv* env, jobject thiz, jstring apkPath, jint width, jint height)
{
	jboolean isCopy;
	const char* str = env->GetStringUTFChars(apkPath, &isCopy);
	zip = boost::shared_ptr<Platform::ZipFileManager>(new Platform::ZipFileManager(str));

	video = CreateVideo(320, 240, GS_L("assets/fonts/"), zip);
	input = CreateInput(&g_inputStr, true);
	audio = CreateAudio(0);
	video->ResetVideoMode(width, height, GSPF_DEFAULT, false);
	application->Start(video, input, audio);
}

JNIEXPORT void JNICALL Java_net_asantee_gs2d_GS2DJNI_resize(JNIEnv* env, jobject thiz, jint width, jint height)
{
	video->ResetVideoMode(width, height, GSPF_DEFAULT, false);
}

str_type::string AssembleCommands()
{
	str_type::stringstream ss;
	try
	{
		ss
		<< boost::any_cast<str_type::string>(audio->GetAudioContext())
		<< boost::any_cast<str_type::string>(video->GetGraphicContext());
		return ss.str();
	}
	catch (const boost::bad_any_cast& e)
	{
		video->Message(GS_L("Invalid type for command assembling"), GSMT_ERROR);
		return GS_L("");
	}
}

JNIEXPORT jstring JNICALL Java_net_asantee_gs2d_GS2DJNI_mainLoop(JNIEnv* env, jobject thiz, jstring inputStr)
{
	jboolean isCopy;
	g_inputStr = env->GetStringUTFChars(inputStr, &isCopy);

	video->HandleEvents();
	input->Update();
	application->Update(Min(static_cast<unsigned long>(1000), ComputeElapsedTime(video)));
	application->RenderFrame();

	return env->NewStringUTF(AssembleCommands().c_str());
}

JNIEXPORT jstring JNICALL Java_net_asantee_gs2d_GS2DJNI_destroy(JNIEnv* env, jobject thiz)
{
	application->Destroy();
	video->Message(GS_L("Application resources destroyed"), GSMT_INFO);
	return env->NewStringUTF(GS_L(""));
}

JNIEXPORT jstring JNICALL Java_net_asantee_gs2d_GS2DJNI_runOnUIThread(JNIEnv* env, jobject thiz, jstring inputStr)
{
	jboolean isCopy;
	const std::string str = env->GetStringUTFChars(inputStr, &isCopy);
	const std::string outStr = application->RunOnUIThread(str);
	return env->NewStringUTF(outStr.c_str());
}
