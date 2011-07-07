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

import java.io.File;

import android.app.Activity;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.os.Bundle;
import android.os.Environment;
import android.view.Gravity;
import android.view.WindowManager;
import android.widget.Toast;

public class GS2DActivity extends Activity {

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
		createLogPath();
	}

	@Override
	protected void onStart() {
		super.onStart();
		soundCmdListener = new SoundCommandListener(this);
		accelerometerListener = new AccelerometerListener(this);
		surfaceView = new GL2JNIView(this, retrieveApkPath(), soundCmdListener, accelerometerListener);
		setContentView(surfaceView);
		/*this.runOnUiThread(new Runnable() {
			public void run() {
				Log.i("GS2D", GS2DJNI.runOnUIThread("null"));
			} });*/
	}

	@Override
	protected void onPause() {
		super.onPause();
		accelerometerListener.onPause();
		surfaceView.onPause();

		surfaceView.destroy();
		soundCmdListener.clearAll();
	}

	@Override
	protected void onResume() {
		super.onResume();
		accelerometerListener.onResume();
		surfaceView.onResume();
	}

	String retrieveApkPath() {
		String apkFilePath = null;
		ApplicationInfo appInfo = null;
		PackageManager packMgmr = getPackageManager();
		try {
			appInfo = packMgmr.getApplicationInfo(getPackageName(), 0);
		} catch (NameNotFoundException e) {
			e.printStackTrace();
			throw new RuntimeException("Unable to locate assets, aborting...");
		}
		apkFilePath = appInfo.sourceDir;
		return (apkFilePath);
	}

	private void createLogPath() {
		File dir = new File(Environment.getExternalStorageDirectory() + "/" + FOLDER_NAME);
		dir.mkdirs();
	}

	public static void toast(final String str, final Activity context) {
		context.runOnUiThread(new Runnable() {
			public void run() {
				Toast toast = Toast.makeText(context, str, Toast.LENGTH_LONG);
				toast.setGravity(Gravity.CENTER, toast.getXOffset() / 2, toast.getYOffset() / 2);
				toast.show();
			}
		});
	}

	private AccelerometerListener accelerometerListener;
	private SoundCommandListener soundCmdListener;
	private static final String FOLDER_NAME = "gs2dlog";
	private GL2JNIView surfaceView;
}