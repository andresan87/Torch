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

package net.asantee.gs2d;

import java.io.IOException;
import java.util.HashMap;

import android.app.Activity;
import android.content.Context;
import android.content.res.AssetFileDescriptor;
import android.content.res.AssetManager;
import android.media.AudioManager;
import android.media.SoundPool;

public class SoundEffectManager {

	private static final int MAXIMUM_SIMULTANEOUS_SFX = 8;

	public SoundEffectManager(Activity activity) {
		this.pool = new SoundPool(MAXIMUM_SIMULTANEOUS_SFX, AudioManager.STREAM_MUSIC, 0);
		this.manager = (AudioManager) activity.getSystemService(Context.AUDIO_SERVICE);
		this.assets = activity.getAssets();
		this.activity = activity;
	}

	protected String removePrefix(String str, String prefix) {
		int idx = str.indexOf(prefix);
		if (idx == 0) {
			return str.substring(prefix.length());
		} else {
			return str;
		}
	}

	public boolean load(String fileName) {
		if (samples.get(fileName) == null) {
			String relativeFileName = removePrefix(fileName, PREFIX);

			AssetFileDescriptor afd;
			try {
				afd = assets.openFd(relativeFileName);
				samples.put(fileName, pool.load(afd, 1));
				if (afd != null) {
					try {
						afd.close();
					} catch (IOException e) {
						GS2DActivity.toast(fileName + " couldn't close asset", activity);
					}
				}
			} catch (IOException e) {
				GS2DActivity.toast(fileName + " file not found", activity);
				return false;
			}
		}
		return true;
	}

	public void play(String fileName, float volume, float speed) {
		float streamVolume = manager.getStreamVolume(AudioManager.STREAM_MUSIC);
		streamVolume /= manager.getStreamMaxVolume(AudioManager.STREAM_MUSIC);
		streamVolume *= globalVolume * volume;

		Integer id = samples.get(fileName);
		if (id != null) {
			pool.play(id, streamVolume, streamVolume, 1, 0, speed);
		} else {
			GS2DActivity.toast(fileName + " file has not been loaded", activity);
		}
	}

	public void setGlobalVolume(float volume) {
		globalVolume = volume;
	}

	public float getGlobalVolume() {
		return globalVolume;
	}

	public boolean release(String fileName) {
		Integer id = samples.get(fileName);
		if (id != null) {
			samples.remove(fileName);
			return pool.unload(id);
		} else {
			return false;
		}
	}

	public void clearAll() {
		pool.release();
		samples.clear();
	}

	private static String PREFIX = "assets/";
	private Activity activity;
	private static float globalVolume = 1.0f;
	private SoundPool pool;
	private AudioManager manager;
	private HashMap<String, Integer> samples = new HashMap<String, Integer>();
	private AssetManager assets;
}
