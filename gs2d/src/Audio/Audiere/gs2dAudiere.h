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

#ifndef GS2D_AUDIERE_H_
#define GS2D_AUDIERE_H_

#include "../../gs2daudio.h"
#include "audiere/src/audiere.h"

namespace gs2d {

class AudiereContext : public Audio
{
	audiere::AudioDevicePtr m_device;
	bool CreateAudioDevice(boost::any data);

	boost::weak_ptr<AudiereContext> weak_this;

	AudiereContext();

public:
	static boost::shared_ptr<AudiereContext> Create(boost::any data);

	AudioSamplePtr LoadSampleFromFile(const std::wstring& fileName, const GS_SAMPLE_TYPE type = GSST_UNKNOWN);
	AudioSamplePtr LoadSampleFromFileInMemory(void *pBuffer, const unsigned int bufferLength, const GS_SAMPLE_TYPE type = GSST_UNKNOWN);

	boost::any GetAudioContext();
};

typedef boost::shared_ptr<AudiereContext> AudiereContextPtr;

class AudiereSample : public AudioSample
{
	audiere::OutputStreamPtr m_output;
	int m_position;
	GS_SAMPLE_STATUS m_status;

	GS_SAMPLE_TYPE m_type;
	AudioWeakPtr m_audio;

	bool LoadSampleFromFile(AudioWeakPtr audio, const std::wstring& fileName, const GS_SAMPLE_TYPE type = GSST_UNKNOWN);
	bool LoadSampleFromFileInMemory(AudioWeakPtr audio, void *pBuffer, const unsigned int bufferLength, const GS_SAMPLE_TYPE type = GSST_UNKNOWN);

public:
	AudiereSample();
	~AudiereSample();

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