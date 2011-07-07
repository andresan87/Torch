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

#ifndef GS2D_ANDROID_AUDIO_H_
#define GS2D_ANDROID_AUDIO_H_

#include "../../gs2daudio.h"
#include "../../Platform/FileLogger.h"
#include "../../Platform/android/Platform.android.h"

namespace gs2d {

class AndroidAudioContext : public Audio, public Platform::NativeCommandForwarder
{
	bool CreateAudioDevice(boost::any data);
	Platform::FileLogger m_logger;

	boost::weak_ptr<AndroidAudioContext> weak_this;

	AndroidAudioContext();

public:
	static boost::shared_ptr<AndroidAudioContext> Create(boost::any data);

	static bool IsStreamable(const GS_SAMPLE_TYPE type);

	AudioSamplePtr LoadSampleFromFile(const str_type::string& fileName, const GS_SAMPLE_TYPE type = GSST_UNKNOWN);
	AudioSamplePtr LoadSampleFromFileInMemory(void *pBuffer, const unsigned int bufferLength, const GS_SAMPLE_TYPE type = GSST_UNKNOWN);
	boost::any GetAudioContext();
};

typedef boost::shared_ptr<AndroidAudioContext> AndroidAudioContextPtr;
typedef boost::shared_ptr<AndroidAudioContext> AndroidAudioContextWeakPtr;

class AndroidAudioSample : public AudioSample
{
	AndroidAudioContext* m_audio;

	str_type::string m_fileName;
	float m_volume, m_speed;
	bool m_loop;
	GS_SAMPLE_TYPE m_type;
	static Platform::FileLogger m_logger;

public:
	AndroidAudioSample();
	~AndroidAudioSample();

	bool LoadSampleFromFile(AudioWeakPtr audio, const str_type::string& fileName, const GS_SAMPLE_TYPE type = GSST_UNKNOWN);
	bool LoadSampleFromFileInMemory(AudioWeakPtr audio, void *pBuffer, const unsigned int bufferLength, const GS_SAMPLE_TYPE type = GSST_UNKNOWN);

	bool SetLoop(const bool enable);
	bool GetLoop() const;

	bool Play();
	GS_SAMPLE_STATUS GetStatus();

	bool IsPlaying();

	bool Pause();
	bool Stop();

	GS_SAMPLE_TYPE GetType() const;

	bool SetSpeed(const float speed);
	float GetSpeed() const;

	bool SetVolume(const float volume);
	float GetVolume() const;

	bool SetPan(const float pan);
	float GetPan() const;
};

} // namespace gs2d

#endif