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

#ifndef GS2D_DSHOW_PLAYER_H_
#define GS2D_DSHOW_PLAYER_H_

#include "../../gs2dplayer.h"
#include <dshow.h>
#pragma warning( push )
#pragma warning( disable : 4201 )
#include <mmsystem.h>
#pragma warning( pop )

namespace gs2d {

class DShowPlayer : public Player
{
	friend PlayerPtr CreatePlayer(VideoPtr pVideo, const std::wstring& fileName);

	struct VIDEO_DATA
	{
		VIDEO_DATA()
		{
			pGB = NULL;
			pMC = NULL;
			pVW = NULL;
			pME = NULL;
			pMS = NULL;
			pBA = NULL;
		}
		IGraphBuilder  *pGB;
		IMediaControl  *pMC;
		IVideoWindow   *pVW;
		IMediaEvent    *pME;
		IMediaSeeking  *pMS;
		IBasicAudio    *pBA;
		HWND hWnd;
	} m_data;

	bool m_continue;
	VideoPtr m_pVideo;

	math::Vector2i m_size;
	math::Vector2i m_pos;

	DShowPlayer();
	bool LoadVideo(VideoPtr pVideo, const std::wstring& fileName);

public:
	~DShowPlayer();

	void UpdateVideo();

	bool Play();
	bool Rewind();
	bool Stop();
	bool Pause();

	bool SetVideoPos(const math::Vector2i &pos);
	bool SetVideoSize(const math::Vector2i &size);
	bool SetAudioVolume(const float volume);
	math::Vector2i GetVideoPos() const;
	math::Vector2i GetVideoSize() const ;

	bool SetFullscreen();
	bool IsFullscreen() const;

	void ShowVideo(const bool show);
	bool IsVideoVisible();
	bool IsFinished() const;
	GS_PLAYER_STATUS GetStatus();
};

typedef boost::shared_ptr<DShowPlayer> DShowPlayerPtr;
typedef boost::weak_ptr<DShowPlayer> DShowPlayerWeakPtr;

} // namespace gs2d

#endif