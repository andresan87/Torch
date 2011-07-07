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

#ifndef GS2D_PLAYER_H_
#define GS2D_PLAYER_H_

#include "gs2dinput.h"

namespace gs2d {

enum GS_PLAYER_INFO
{
	GSPI_FAILED = 0,
	GSPI_SKIPPED = 1,
	GSPI_CLOSE_WINDOW = 2,
	GSPI_FINISHED = 3,
};

enum GS_PLAYER_STATUS
{
	GSPS_PLAYING = 0,
	GSPS_PAUSED = 1,
	GSPS_STOPPED = 2,
	GSPS_UNKNOWN = 3
};

class Player
{
public:
	virtual bool LoadVideo(VideoPtr pVideo, const std::wstring& fileName) = 0;
	virtual void UpdateVideo() = 0;

	virtual bool Play() = 0;
	virtual bool Rewind() = 0;
	virtual bool Stop() = 0;
	virtual bool Pause() = 0;

	virtual bool SetVideoPos(const math::Vector2i &pos) = 0;
	virtual bool SetVideoSize(const math::Vector2i &size) = 0;
	virtual bool SetAudioVolume(const float volume) = 0;
	virtual math::Vector2i GetVideoPos() const = 0;
	virtual math::Vector2i GetVideoSize() const  = 0;

	virtual bool SetFullscreen() = 0;
	virtual bool IsFullscreen() const = 0;

	virtual void ShowVideo(const bool show) = 0;
	virtual bool IsVideoVisible() = 0;
	virtual bool IsFinished() const = 0;
	virtual GS_PLAYER_STATUS GetStatus() = 0;
};

GS_PLAYER_INFO PlayCutscene(VideoPtr pVideo, const std::wstring& fileName,
							InputPtr pInput, const GS_KEY escapeKey = GSK_ESC);


typedef boost::shared_ptr<Player> PlayerPtr;
typedef boost::weak_ptr<Player> PlayerWeakPtr;

GS2D_API PlayerPtr CreatePlayer(VideoPtr pVideo, const std::wstring& fileName);

} // namespace gs2d

#endif