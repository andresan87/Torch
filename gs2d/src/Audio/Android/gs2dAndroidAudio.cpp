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

#include "gs2dAndroidAudio.h"
#include <exception>
#include "../../Platform/android/Platform.android.h"

namespace gs2d {

GS2D_API AudioPtr CreateAudio(boost::any data)
{
	AudioPtr audio = AndroidAudioContext::Create(data);
	if (audio)
	{
		return audio;
	}
	else
	{
		return AudioPtr();
	}
}

boost::shared_ptr<AndroidAudioContext> AndroidAudioContext::Create(boost::any data)
{
	boost::shared_ptr<AndroidAudioContext> p(new AndroidAudioContext());
	p->weak_this = p;
	if (p->CreateAudioDevice(data))
	{
		return p;
	}
	else
	{
		return AndroidAudioContextPtr();
	}
}

AndroidAudioContext::AndroidAudioContext() :
	m_logger(Platform::FileLogger::GetLogPath() + "AndroidAudioContext.log.txt")
{
}

bool AndroidAudioContext::CreateAudioDevice(boost::any data)
{
	m_logger.Log("Audio device initialized", Platform::FileLogger::INFO);
	return true;
}

AudioSamplePtr AndroidAudioContext::LoadSampleFromFile(const str_type::string& fileName, const GS_SAMPLE_TYPE type)
{
	AudioSamplePtr sample = AudioSamplePtr(new AndroidAudioSample);
	sample->LoadSampleFromFile(weak_this, fileName, type);
	return sample;
}

AudioSamplePtr AndroidAudioContext::LoadSampleFromFileInMemory(void *pBuffer, const unsigned int bufferLength, const GS_SAMPLE_TYPE type)
{
	return AudioSamplePtr();
}

boost::any AndroidAudioContext::GetAudioContext()
{
	str_type::string out;
	ForwardCommands(out);
	return out;
}

bool AndroidAudioContext::IsStreamable(const GS_SAMPLE_TYPE type)
{
	switch (type)
	{
	case GSST_SOUND_EFFECT:
		return false;
	case GSST_MUSIC:
		return true;
	case GSST_SOUNDTRACK:
		return true;
	case GSST_AMBIENT_SFX:
		return false;
	case GSST_UNKNOWN:
	default:
		return false;
	}
}

// Audio sample

Platform::FileLogger AndroidAudioSample::m_logger(Platform::FileLogger::GetLogPath() + "AndroidAudioSample.log.txt");

AndroidAudioSample::AndroidAudioSample() :
	m_volume(1.0f),
	m_speed(1.0f),
	m_loop(false)
{
}

AndroidAudioSample::~AndroidAudioSample()
{
	if (!AndroidAudioContext::IsStreamable(m_type))
	{
		const str_type::string cmd = Platform::NativeCommandAssembler::DeleteSound(m_fileName);
		m_audio->Command(cmd, true);
		m_logger.Log(cmd, Platform::FileLogger::INFO);
	}
}

bool AndroidAudioSample::LoadSampleFromFile(AudioWeakPtr audio, const str_type::string& fileName, const GS_SAMPLE_TYPE type)
{
	m_audio = static_cast<AndroidAudioContext*>(audio.lock().get());
	m_fileName = fileName;
	m_type = type;

	if (!AndroidAudioContext::IsStreamable(m_type))
	{
		const str_type::string cmd = Platform::NativeCommandAssembler::LoadSound(m_fileName);
		m_audio->Command(cmd);
		m_logger.Log(cmd, Platform::FileLogger::INFO);
	}
	return true;
}

bool AndroidAudioSample::LoadSampleFromFileInMemory(AudioWeakPtr audio, void *pBuffer, const unsigned int bufferLength, const GS_SAMPLE_TYPE type)
{
	// TODO
	return false;
}

bool AndroidAudioSample::SetLoop(const bool enable)
{
	m_loop = enable;
	return true;
}

bool AndroidAudioSample::GetLoop() const
{
	return m_loop;
}

bool AndroidAudioSample::Play()
{
	if (!AndroidAudioContext::IsStreamable(m_type))
	{
		m_audio->Command(
			Platform::NativeCommandAssembler::PlaySound(m_fileName, m_volume, m_loop, m_speed)
		);
	}
	return false;
}

GS_SAMPLE_STATUS AndroidAudioSample::GetStatus()
{
	return GSSS_UNKNOWN;
}

bool AndroidAudioSample::IsPlaying()
{
	// TODO
	return false;
}

bool AndroidAudioSample::Pause()
{
	// TODO
	return false;
}

bool AndroidAudioSample::Stop()
{
	// TODO
	return false;
}

GS_SAMPLE_TYPE AndroidAudioSample::GetType() const
{
	return m_type;
}

bool AndroidAudioSample::SetSpeed(const float speed)
{
	m_speed = speed;
	return true;
}

float AndroidAudioSample::GetSpeed() const
{
	return m_speed;
}

bool AndroidAudioSample::SetVolume(const float volume)
{
	m_volume = volume;
	return true;
}

float AndroidAudioSample::GetVolume() const
{
	return m_volume;
}

bool AndroidAudioSample::SetPan(const float pan)
{
	// TODO
	return false;
}

float AndroidAudioSample::GetPan() const
{
	// TODO
	return 0.0f;
}

} // namespace gs2d
