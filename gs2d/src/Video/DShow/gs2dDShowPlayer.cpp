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

#include "gs2dDShowPlayer.h"
#include "../../Video/Direct3D9/gs2dD3D9.h"
#include "../../unicode/utf8converter.h"

namespace gs2d {
using namespace math;

GS2D_API PlayerPtr CreatePlayer(VideoPtr pVideo, const std::wstring& fileName)
{
	PlayerPtr player = PlayerPtr(new DShowPlayer);
	
	if (!player->LoadVideo(pVideo, fileName))
	{
		return PlayerPtr();
	}
	return player;
}

DShowPlayer::DShowPlayer()
{
	m_continue = true;
}

DShowPlayer::~DShowPlayer()
{
	//m_data.pVW->put_Owner((OAHWND)NULL);
	if (m_data.pGB)
	{
		m_data.pGB->Release();
		m_data.pGB = NULL;
	}
	if (m_data.pMC)
	{
		m_data.pMC->Release();
		m_data.pMC = NULL;
	}
	if (m_data.pME)
	{
		m_data.pME->Release();
		m_data.pME = NULL;
	}
	if (m_data.pMS)
	{
		m_data.pMS->Release();
		m_data.pMS = NULL;
	}
	if (m_data.pBA)
	{
		m_data.pBA->Release();
		m_data.pBA = NULL;
	}
	if (m_data.pVW)
	{
		m_data.pVW->Release();
		m_data.pVW = NULL;
	}

	if (m_pVideo)
	{
		D3D9VideoInfoPtr videoInfo;
		try
		{
			videoInfo = boost::any_cast<D3D9VideoInfoPtr>(m_pVideo->GetVideoInfo());
		}
		catch (const boost::bad_any_cast &)
		{
			return;
		}
		BringWindowToTop(videoInfo->m_hWnd);
	}
	//CoUninitialize();
}

bool DShowPlayer::LoadVideo(VideoPtr pVideo, const std::wstring& fileName)
{
	if (pVideo->Rendering())
	{
		ShowMessage(L"Videos can't be loaded while rendering - DShowPlayer::LoadVideo", GSMT_ERROR);
		return false;
	}

	m_pVideo = pVideo;

	D3D9VideoInfoPtr videoInfo;
	try
	{
		videoInfo = boost::any_cast<D3D9VideoInfoPtr>(m_pVideo->GetVideoInfo());
	}
	catch (const boost::bad_any_cast &)
	{
		ShowMessage(L"Invalid D3D9VideoInfoPtr object - DShowPlayer::LoadVideo", GSMT_ERROR);
		return false;
	}

	m_data.hWnd = videoInfo->m_hWnd;

	if (FAILED(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED)))
	{
		m_pVideo->Message(L"Failed while trying to start COM interface - DShowPlayer::LoadVideo", GSMT_ERROR);
		return false;
	}

	bool failed = false;
	if (FAILED(CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC, 
						 IID_IGraphBuilder, (void **)&m_data.pGB)))					failed = true;
	if (FAILED(m_data.pGB->QueryInterface(IID_IMediaControl, (void **)&m_data.pMC)))	failed = true;
	if (FAILED(m_data.pGB->QueryInterface(IID_IVideoWindow,  (void **)&m_data.pVW)))	failed = true;
	if (FAILED(m_data.pGB->QueryInterface(IID_IMediaEventEx, (void **)&m_data.pME)))	failed = true;
	if (FAILED(m_data.pGB->QueryInterface(IID_IMediaSeeking, (void**)&m_data.pMS)))		failed = true;
	if (FAILED(m_data.pGB->QueryInterface(IID_IBasicAudio,	 (void**)&m_data.pBA)))		failed = true;

	if (failed)
	{
		m_pVideo->Message(L"Failed while trying to query COM interfaces - DShowPlayer::LoadVideo", GSMT_ERROR);
		return false;
	}

	if (FAILED(m_data.pGB->RenderFile(fileName.c_str(), NULL)))
	{
		std::wstring message = fileName;
		message += L"\nFailed while trying to load and decode the video - DShowPlayer::LoadVideo";
		m_pVideo->Message(message, GSMT_ERROR);
		return false;
	}

	if (FAILED(m_data.pVW->put_MessageDrain((OAHWND)m_data.hWnd)))
	{
		std::wstring message = fileName;
		message += L" is not a video file. - DShowPlayer::LoadVideo";
		m_pVideo->Message(message, GSMT_ERROR);
		return false;
	}

	//m_data.pVW->put_WindowState(SW_SHOWNOACTIVATE);
	m_data.pVW->put_Owner((OAHWND)m_data.hWnd);
	m_data.pVW->put_WindowStyle(WS_CHILD);
	m_data.pVW->put_WindowStyleEx(WS_EX_NOACTIVATE | WS_EX_TOPMOST);

	m_data.pMC->Pause();

	long lWidth, lHeight;
	m_data.pVW->GetMinIdealImageSize(&lWidth, &lHeight);
	m_size.x = lWidth * (long)m_pVideo->GetScreenSize().x/16;
	m_size.y = lHeight * (long)m_pVideo->GetScreenSize().x/16;

	Vector2i pos(0, 0);
	m_pos = pos;
	m_data.pVW->SetWindowPosition((long)m_pos.x,(long)m_pos.y, (long)m_size.x, (long)m_size.y);

	return true;
}

bool DShowPlayer::Play()
{
	if (GetStatus() == GSPS_PLAYING)
		Rewind();
	m_data.pMC->Run();
	m_continue = true;
	return true;
}

bool DShowPlayer::SetAudioVolume(const float volume)
{
	if (m_data.pBA)
	{
		long lVol = (long)((1.0f-Max(Min(volume, 1.0f), 0.0f))*-10000.0f);
		m_data.pBA->put_Volume(lVol);
	}
	else
	{
		return false;
	}
	return true;
}

bool DShowPlayer::Rewind()
{
	REFERENCE_TIME newTime = 0;
	m_data.pMS->SetPositions(&newTime, AM_SEEKING_AbsolutePositioning, NULL, AM_SEEKING_NoPositioning);
	return true;
}

bool DShowPlayer::Stop()
{
	Pause();
	Rewind();
	m_data.pMC->Stop();
	return true;
}

bool DShowPlayer::Pause()
{
	m_data.pMC->Pause();
	return true;
}

bool DShowPlayer::SetVideoPos(const Vector2i &pos)
{
	m_pos = pos;
	m_data.pVW->SetWindowPosition((long)m_pos.x,(long)m_pos.y, (long)m_size.x, (long)m_size.y);
	return true;
}

bool DShowPlayer::SetVideoSize(const Vector2i &size)
{
	m_size = size;
	m_data.pVW->SetWindowPosition((long)m_pos.x,(long)m_pos.y, (long)m_size.x, (long)m_size.y);
	return true;
}

Vector2i DShowPlayer::GetVideoPos() const
{
	return m_pos;
}

Vector2i DShowPlayer::GetVideoSize() const
{
	return m_size;
}

bool DShowPlayer::SetFullscreen()
{
	m_pos = Vector2i(0,0);
	m_size = m_pVideo->GetScreenSize();
	m_data.pVW->SetWindowPosition((long)m_pos.x,(long)m_pos.y, (long)m_size.x, (long)m_size.y);
	return true;
}

bool DShowPlayer::IsFullscreen() const
{
	return (m_pos == Vector2i(0,0) && m_size == m_pVideo->GetScreenSize());
}

void DShowPlayer::UpdateVideo()
{
	//m_pVideo->EnableMediaPlaying(true);
	m_data.pVW->SetWindowForeground(OATRUE);
	m_data.pVW->HideCursor(m_pVideo->IsCursorHidden());

	long lEventCode;
	LONG_PTR lpParam1, lpParam2;

	if (E_ABORT != m_data.pME->GetEvent(&lEventCode, &lpParam1, 
								&lpParam2, 0))
	{
		if (lEventCode == EC_COMPLETE)
		{
			Stop();
			m_continue = false;
		}
		m_data.pME->FreeEventParams(lEventCode, lpParam1, lpParam2);
	}
}

bool DShowPlayer::IsFinished() const
{
	return !m_continue;
}

GS_PLAYER_STATUS DShowPlayer::GetStatus()
{
	static const LONG QUERY_STATUS_TIMEOUT = 1000;

	OAFilterState state;
	m_data.pMC->GetState(QUERY_STATUS_TIMEOUT, &state);

	switch (state)
	{
	case State_Stopped:
		return GSPS_STOPPED;
	case State_Paused:
		return GSPS_PAUSED;
	case State_Running:
		return GSPS_PLAYING;
	default:
		break;
	};
	return GSPS_UNKNOWN;
}

void DShowPlayer::ShowVideo(const bool show)
{
	//m_pVideo->EnableMediaPlaying(show);
	m_data.pVW->put_Visible((show) ? OATRUE : OAFALSE);
}

bool DShowPlayer::IsVideoVisible()
{
	long lVisible;
	m_data.pVW->get_Visible(&lVisible);
	return (lVisible == OATRUE);
}

GS_PLAYER_INFO PlayCutscene(VideoPtr pVideo, const std::wstring& fileName,
							InputPtr pInput, const GS_KEY escapeKey)
{
	if (!pVideo)
	{
		ShowMessage(L"Invalid video handler - gs2d::PlayCutscene", GSMT_ERROR);
		return GSPI_FAILED;
	}

	GS_PLAYER_INFO info = GSPI_FINISHED;
	//pVideo->TurnLoopManagerOff();

	const bool rendering = pVideo->Rendering();
	if (rendering)
		pVideo->EndSpriteScene();

	PlayerPtr player = CreatePlayer(pVideo, fileName);
	if (!player)
	{
		ShowMessage(L"Failed while trying to load the video - gs2d::PlayCutscene", GSMT_ERROR);
		if (rendering)
			pVideo->BeginSpriteScene();
		return GSPI_FAILED;
	}

	pVideo->EnableMediaPlaying(true);
	player->SetFullscreen();
	player->Rewind();
	player->Play();

	while (!player->IsFinished())
	{
		player->UpdateVideo();
		const Video::APP_STATUS status = pVideo->HandleEvents();
		if (status == Video::APP_QUIT)
		{
			info = GSPI_CLOSE_WINDOW;
			break;
		}
		else
		{
			if (status == Video::APP_SKIP)
				continue;
		}

		if (pInput)
		{
			pInput->Update();
			if (pInput->GetKeyState(escapeKey) == GSKS_HIT)
			{
				info = GSPI_SKIPPED;
				break;
			}
		}
	}

	if (rendering)
		pVideo->BeginSpriteScene();

	pVideo->EnableMediaPlaying(false);
	return info;
}

} // namespace gs2d