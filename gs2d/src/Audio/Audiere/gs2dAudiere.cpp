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

#include "gs2dAudiere.h"
#include "../../gs2d.h"
#include "../../unicode/utf8converter.h"

namespace gs2d {
using namespace math;

GS2D_API AudioPtr CreateAudio(boost::any data)
{
	AudiereContextPtr audio = AudiereContext::Create(data);
	if (audio)
	{
		return audio;
	}
	return AudioPtr();
}

AudiereContext::AudiereContext()
{
}

boost::shared_ptr<AudiereContext> AudiereContext::Create(boost::any data)
{
	boost::shared_ptr<AudiereContext> p(new AudiereContext());
	p->weak_this = p;
	if (p->CreateAudioDevice(data))
	{
		return p;
	}
	else
	{
		return AudiereContextPtr();
	}
}

AudioSamplePtr AudiereContext::LoadSampleFromFile(const std::wstring& fileName, const GS_SAMPLE_TYPE type)
{
	AudioSamplePtr audio = AudioSamplePtr(new AudiereSample);
	if (!audio->LoadSampleFromFile(weak_this, fileName, type))
	{
		return AudioSamplePtr();
	}
	return audio;
}

AudioSamplePtr AudiereContext::LoadSampleFromFileInMemory(void *pBuffer, const unsigned int bufferLength, const GS_SAMPLE_TYPE type)
{
	AudioSamplePtr audio = AudioSamplePtr(new AudiereSample);
	if (!audio->LoadSampleFromFileInMemory(weak_this, pBuffer, bufferLength, type))
	{
		return AudioSamplePtr();
	}
	return audio;
}

bool AudiereContext::CreateAudioDevice(boost::any data)
{
	m_device = audiere::OpenDevice();
	if (!m_device)
	{
		ShowMessage(L"Audiere initialization failed - AudiereContext::CreateAudioDevice");
		return false;
	}
	return true;
}

boost::any AudiereContext::GetAudioContext()
{
	return m_device;
}

AudiereSample::AudiereSample()
{
	m_output = NULL;
	m_status = GSSS_STOPPED;
	m_position = 0;
	m_type = GSST_UNKNOWN;
}

AudiereSample::~AudiereSample()
{
	if (m_output)
	{
		m_output->stop();
	}
}

bool AudiereSample::LoadSampleFromFile(AudioWeakPtr audio, const std::wstring& fileName, const GS_SAMPLE_TYPE type)
{
	m_audio = audio;
	m_type = type;

	bool stream;
	switch (m_type)
	{
	case GSST_MUSIC:
	case GSST_AMBIENT_SFX:
	case GSST_SOUNDTRACK:
		stream = true;
		break;
	case GSST_SOUND_EFFECT:
	case GSST_UNKNOWN:
	default:
		stream = false;
		break;
	};

	audiere::AudioDevicePtr device;
	try
	{
		device = boost::any_cast<audiere::AudioDevicePtr>(m_audio.lock()->GetAudioContext());
	}
	catch (const boost::bad_any_cast &)
	{
		std::wstringstream ss;
		ss << L"Invalid device in context: " << fileName;
		ShowMessage(ss);
		return false;
	}

	m_output = audiere::OpenSound(device, fileName.c_str(), stream);

	if (!m_output)
	{
		std::wstringstream ss;
		ss << L"Failed while loading the file: " << fileName;
		ShowMessage(ss);
		return false;
	}

	SetVolume(1.0f);
	return true;
}

bool AudiereSample::LoadSampleFromFileInMemory(AudioWeakPtr audio, void *pBuffer, const unsigned int bufferLength, const GS_SAMPLE_TYPE type)
{
	m_audio = audio;
	m_type = type;

	bool stream;
	switch (m_type)
	{
	case GSST_MUSIC:
	case GSST_AMBIENT_SFX:
	case GSST_SOUNDTRACK:
		stream = true;
		break;
	case GSST_SOUND_EFFECT:
	case GSST_UNKNOWN:
	default:
		stream = false;
		break;
	};

	audiere::AudioDevicePtr device;
	try
	{
		device = boost::any_cast<audiere::AudioDevicePtr>(m_audio.lock()->GetAudioContext());
	}
	catch (const boost::bad_any_cast &)
	{
		std::wstringstream ss;
		ss << L"Invalid device in context: " << L"buffer";
		ShowMessage(ss);
		return false;
	}

	audiere::FilePtr file = audiere::CreateMemoryFile(pBuffer, bufferLength);
	m_output = audiere::OpenSound(device, file, stream);

	if (!m_output)
	{
		std::wstringstream ss;
		ss << L"Failed while loading the buffer";
		ShowMessage(ss);
		return false;
	}

	SetVolume(1.0f);
	return true;
}

GS_SAMPLE_TYPE AudiereSample::GetType() const
{
	return m_type;
}

GS_SAMPLE_STATUS AudiereSample::GetStatus()
{
	if (!m_output->isPlaying() && m_status == GSSS_PLAYING)
		m_status = GSSS_STOPPED;

	return m_status;
}

bool AudiereSample::IsPlaying()
{
	return (GetStatus() == GSSS_PLAYING);
}

bool AudiereSample::SetLoop(const bool enable)
{
	m_output->setRepeat(enable);
	return true;
}

bool AudiereSample::GetLoop() const
{
	return m_output->getRepeat();
}

bool AudiereSample::Play()
{
	if (m_status == GSSS_PAUSED)
	{
		m_output->setPosition(m_position);
	}
	else
	{
		if (m_output->isPlaying())
			m_output->reset();
	}
	m_output->play();
	m_status = GSSS_PLAYING;
	return true;
}

bool AudiereSample::Pause()
{
	if (m_output->isPlaying())
	{
		m_position = m_output->getPosition();
		m_output->stop();
	}
	m_status = GSSS_PAUSED;
	return true;
}

bool AudiereSample::Stop()
{
	m_output->stop();
	m_output->reset();
	m_position = 0;
	m_status = GSSS_STOPPED;
	return true;
}

bool AudiereSample::SetSpeed(const float speed)
{
	m_output->setPitchShift(Min(Max(speed, 0.5f), 2.0f));
	return true;
}

float AudiereSample::GetSpeed() const
{
	return m_output->getPitchShift();
}

bool AudiereSample::SetPan(const float pan)
{
	m_output->setPan(Min(Max(pan, -1.0f), 1.0f));
	return true;
}

float AudiereSample::GetPan() const
{
	return m_output->getPan();
}

bool AudiereSample::SetVolume(const float volume)
{
	m_output->setVolume(volume);
	return true;
}

float AudiereSample::GetVolume() const
{
	return m_output->getVolume();
}

} // namespace gs2d
