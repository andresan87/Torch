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

#ifndef GS2D_AUDIO_H_
#define GS2D_AUDIO_H_

#include "gs2dtypes.h"

namespace gs2d {

enum GS_SAMPLE_STATUS
{
	GSSS_PLAYING = 0,
	GSSS_PAUSED = 1,
	GSSS_STOPPED = 2,
	GSSS_UNKNOWN = 3
};

enum GS_SAMPLE_TYPE
{
	GSST_SOUND_EFFECT = 0,
	GSST_MUSIC = 1,
	GSST_SOUNDTRACK = 2,
	GSST_AMBIENT_SFX = 3,
	GSST_UNKNOWN = 4
};

class AudioSample;
typedef boost::shared_ptr<AudioSample> AudioSamplePtr;
typedef boost::weak_ptr<AudioSample> AudioSampleWeakPtr;

class Audio;
typedef boost::shared_ptr<Audio> AudioPtr;
typedef boost::weak_ptr<Audio> AudioWeakPtr;

/**
 * \brief Abstracts an audio device context
 *
 * This class controls all audio device operations.
 * It may also be used to instantiate audio samples.
 */
class Audio
{
	friend GS2D_API AudioPtr CreateAudio(boost::any data);

	virtual bool CreateAudioDevice(boost::any data) = 0;
public:
	virtual AudioSamplePtr LoadSampleFromFile(const str_type::string& fileName, const GS_SAMPLE_TYPE type = GSST_UNKNOWN) = 0;
	virtual AudioSamplePtr LoadSampleFromFileInMemory(void *pBuffer, const unsigned int bufferLength, const GS_SAMPLE_TYPE type = GSST_UNKNOWN) = 0;
	virtual boost::any GetAudioContext() = 0;
};

/**
 * \brief Abstracts an audio sample
 *
 * Stores and plays audio samples as soundtrack, sound effects or ambient sound.
 */
class AudioSample
{
public:
	virtual bool LoadSampleFromFile(AudioWeakPtr audio, const str_type::string& fileName, const GS_SAMPLE_TYPE type = GSST_UNKNOWN) = 0;
	virtual bool LoadSampleFromFileInMemory(AudioWeakPtr audio, void *pBuffer, const unsigned int bufferLength, const GS_SAMPLE_TYPE type = GSST_UNKNOWN) = 0;

	virtual bool SetLoop(const bool enable) = 0;
	virtual bool GetLoop() const = 0;

	virtual bool Play() = 0;
	virtual GS_SAMPLE_STATUS GetStatus() = 0;

	virtual bool IsPlaying() = 0;

	virtual bool Pause() = 0;
	virtual bool Stop() = 0;

	virtual GS_SAMPLE_TYPE GetType() const = 0;

	virtual bool SetSpeed(const float speed) = 0;
	virtual float GetSpeed() const = 0;

	virtual bool SetVolume(const float volume) = 0;
	virtual float GetVolume() const = 0;

	virtual bool SetPan(const float pan) = 0;
	virtual float GetPan() const = 0;
};

/// Instantiates an Input object
GS2D_API AudioPtr CreateAudio(boost::any data);

} // namespace gs2d

#endif