package br.com.jera.medievaltorch;

import net.asantee.gs2d.GS2DActivity;
import android.media.MediaPlayer;

public class TorchActivity extends GS2DActivity {

	@Override
	protected void onResume() {
		super.onResume();
		mp = MediaPlayer.create(this, R.raw.fire);
		mp.start();
		mp.setLooping(true);
	}
	
	@Override
	protected void onPause() {
		super.onPause();
		mp.stop();
		mp.release();
	}
	
	private MediaPlayer mp;
}
