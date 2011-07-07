/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package net.asantee.gs2d;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGLDisplay;
import javax.microedition.khronos.opengles.GL10;

import android.app.Activity;
import android.graphics.PixelFormat;
import android.opengl.GLSurfaceView;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.MotionEvent;

class GL2JNIView extends GLSurfaceView {
	private static String TAG = "GL2JNIView";
	protected static int displayWidth, displayHeight;
	private static final boolean DEBUG = true;
	public final String apkPath;
	private static int MAXIMUM_TOUCHES = 5;
	protected static Vector2[] currentTouch = new Vector2[MAXIMUM_TOUCHES];
	protected static int touchCount = 0;
	protected SoundCommandListener soundCmdListener;
	private FrameHandler frameHandler;
	private static AccelerometerListener accelerometerListener;

	static class Vector2 {
		Vector2(float x, float y) {
			this.x = x;
			this.y = y;
		}

		public float x, y;
	}

	private void retrieveScreenSize(Activity activity) {
		DisplayMetrics metrics = new DisplayMetrics();
		activity.getWindowManager().getDefaultDisplay().getMetrics(metrics);
		displayWidth = metrics.widthPixels;
		displayHeight = metrics.heightPixels;
	}

	public GL2JNIView(Activity context, String apkPath, SoundCommandListener soundCmdListener, AccelerometerListener accelerometerListener) {
		super(context);
		GL2JNIView.accelerometerListener = accelerometerListener;
		this.apkPath = apkPath;
		this.soundCmdListener = soundCmdListener;
		init(false, 1, 0, context);
		retrieveScreenSize(context);
		setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);
		frameHandler = new FrameHandler(this, 30);
		frameHandler.start();
	}

	private void init(boolean translucent, int depth, int stencil, Activity activity) {
		if (translucent) {
			this.getHolder().setFormat(PixelFormat.TRANSLUCENT);
		}
		setEGLContextFactory(new ContextFactory());
		setEGLConfigChooser(translucent ? new ConfigChooser(8, 8, 8, 8, depth, stencil) : new ConfigChooser(5, 6, 5, 0, depth, stencil));
		setRenderer(new Renderer(apkPath, soundCmdListener, activity));
	}

	private static class ContextFactory implements GLSurfaceView.EGLContextFactory {
		private static int EGL_CONTEXT_CLIENT_VERSION = 0x3098;

		public EGLContext createContext(EGL10 egl, EGLDisplay display, EGLConfig eglConfig) {
			Log.w(TAG, "creating OpenGL ES 2.0 context");
			checkEglError("Before eglCreateContext", egl);
			int[] attrib_list = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL10.EGL_NONE };
			EGLContext context = egl.eglCreateContext(display, eglConfig, EGL10.EGL_NO_CONTEXT, attrib_list);
			checkEglError("After eglCreateContext", egl);
			return context;
		}

		public void destroyContext(EGL10 egl, EGLDisplay display, EGLContext context) {
			egl.eglDestroyContext(display, context);
		}
	}

	private static void checkEglError(String prompt, EGL10 egl) {
		int error;
		while ((error = egl.eglGetError()) != EGL10.EGL_SUCCESS) {
			Log.e(TAG, String.format("%s: EGL error: 0x%x", prompt, error));
		}
	}

	private static class ConfigChooser implements GLSurfaceView.EGLConfigChooser {
		public ConfigChooser(int r, int g, int b, int a, int depth, int stencil) {
			mRedSize = r;
			mGreenSize = g;
			mBlueSize = b;
			mAlphaSize = a;
			mDepthSize = depth;
			mStencilSize = stencil;
		}

		private static int EGL_OPENGL_ES2_BIT = 4;
		private static int[] s_configAttribs2 = { EGL10.EGL_RED_SIZE, 4, EGL10.EGL_GREEN_SIZE, 4, EGL10.EGL_BLUE_SIZE, 4,
				EGL10.EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT, EGL10.EGL_NONE };

		public EGLConfig chooseConfig(EGL10 egl, EGLDisplay display) {

			int[] num_config = new int[1];
			egl.eglChooseConfig(display, s_configAttribs2, null, 0, num_config);

			int numConfigs = num_config[0];

			if (numConfigs <= 0) {
				throw new IllegalArgumentException("No configs match configSpec");
			}

			EGLConfig[] configs = new EGLConfig[numConfigs];
			egl.eglChooseConfig(display, s_configAttribs2, configs, numConfigs, num_config);

			if (DEBUG) {
				printConfigs(egl, display, configs);
			}

			return chooseConfig(egl, display, configs);
		}

		public EGLConfig chooseConfig(EGL10 egl, EGLDisplay display, EGLConfig[] configs) {
			for (EGLConfig config : configs) {
				int d = findConfigAttrib(egl, display, config, EGL10.EGL_DEPTH_SIZE, 0);
				int s = findConfigAttrib(egl, display, config, EGL10.EGL_STENCIL_SIZE, 0);

				// We need at least mDepthSize and mStencilSize bits
				if (d < mDepthSize || s < mStencilSize)
					continue;

				// We want an *exact* match for red/green/blue/alpha
				int r = findConfigAttrib(egl, display, config, EGL10.EGL_RED_SIZE, 0);
				int g = findConfigAttrib(egl, display, config, EGL10.EGL_GREEN_SIZE, 0);
				int b = findConfigAttrib(egl, display, config, EGL10.EGL_BLUE_SIZE, 0);
				int a = findConfigAttrib(egl, display, config, EGL10.EGL_ALPHA_SIZE, 0);

				if (r == mRedSize && g == mGreenSize && b == mBlueSize && a == mAlphaSize)
					return config;
			}
			return null;
		}

		private int findConfigAttrib(EGL10 egl, EGLDisplay display, EGLConfig config, int attribute, int defaultValue) {
			if (egl.eglGetConfigAttrib(display, config, attribute, mValue)) {
				return mValue[0];
			}
			return defaultValue;
		}

		private void printConfigs(EGL10 egl, EGLDisplay display, EGLConfig[] configs) {
			int numConfigs = configs.length;
			Log.w(TAG, String.format("%d configurations", numConfigs));
			for (int i = 0; i < numConfigs; i++) {
				Log.w(TAG, String.format("Configuration %d:\n", i));
				printConfig(egl, display, configs[i]);
			}
		}

		private void printConfig(EGL10 egl, EGLDisplay display, EGLConfig config) {
			int[] attributes = { EGL10.EGL_BUFFER_SIZE, EGL10.EGL_ALPHA_SIZE, EGL10.EGL_BLUE_SIZE, EGL10.EGL_GREEN_SIZE,
					EGL10.EGL_RED_SIZE, EGL10.EGL_DEPTH_SIZE, EGL10.EGL_STENCIL_SIZE, EGL10.EGL_CONFIG_CAVEAT, EGL10.EGL_CONFIG_ID,
					EGL10.EGL_LEVEL, EGL10.EGL_MAX_PBUFFER_HEIGHT, EGL10.EGL_MAX_PBUFFER_PIXELS, EGL10.EGL_MAX_PBUFFER_WIDTH,
					EGL10.EGL_NATIVE_RENDERABLE, EGL10.EGL_NATIVE_VISUAL_ID, EGL10.EGL_NATIVE_VISUAL_TYPE,
					0x3030, // EGL10.EGL_PRESERVED_RESOURCES,
					EGL10.EGL_SAMPLES, EGL10.EGL_SAMPLE_BUFFERS, EGL10.EGL_SURFACE_TYPE, EGL10.EGL_TRANSPARENT_TYPE,
					EGL10.EGL_TRANSPARENT_RED_VALUE, EGL10.EGL_TRANSPARENT_GREEN_VALUE, EGL10.EGL_TRANSPARENT_BLUE_VALUE, 0x3039, // EGL10.EGL_BIND_TO_TEXTURE_RGB,
					0x303A, // EGL10.EGL_BIND_TO_TEXTURE_RGBA,
					0x303B, // EGL10.EGL_MIN_SWAP_INTERVAL,
					0x303C, // EGL10.EGL_MAX_SWAP_INTERVAL,
					EGL10.EGL_LUMINANCE_SIZE, EGL10.EGL_ALPHA_MASK_SIZE, EGL10.EGL_COLOR_BUFFER_TYPE, EGL10.EGL_RENDERABLE_TYPE, 0x3042 // EGL10.EGL_CONFORMANT
			};
			String[] names = { "EGL_BUFFER_SIZE", "EGL_ALPHA_SIZE", "EGL_BLUE_SIZE", "EGL_GREEN_SIZE", "EGL_RED_SIZE", "EGL_DEPTH_SIZE",
					"EGL_STENCIL_SIZE", "EGL_CONFIG_CAVEAT", "EGL_CONFIG_ID", "EGL_LEVEL", "EGL_MAX_PBUFFER_HEIGHT",
					"EGL_MAX_PBUFFER_PIXELS", "EGL_MAX_PBUFFER_WIDTH", "EGL_NATIVE_RENDERABLE", "EGL_NATIVE_VISUAL_ID",
					"EGL_NATIVE_VISUAL_TYPE", "EGL_PRESERVED_RESOURCES", "EGL_SAMPLES", "EGL_SAMPLE_BUFFERS", "EGL_SURFACE_TYPE",
					"EGL_TRANSPARENT_TYPE", "EGL_TRANSPARENT_RED_VALUE", "EGL_TRANSPARENT_GREEN_VALUE", "EGL_TRANSPARENT_BLUE_VALUE",
					"EGL_BIND_TO_TEXTURE_RGB", "EGL_BIND_TO_TEXTURE_RGBA", "EGL_MIN_SWAP_INTERVAL", "EGL_MAX_SWAP_INTERVAL",
					"EGL_LUMINANCE_SIZE", "EGL_ALPHA_MASK_SIZE", "EGL_COLOR_BUFFER_TYPE", "EGL_RENDERABLE_TYPE", "EGL_CONFORMANT" };
			int[] value = new int[1];
			for (int i = 0; i < attributes.length; i++) {
				int attribute = attributes[i];
				String name = names[i];
				if (egl.eglGetConfigAttrib(display, config, attribute, value)) {
					Log.w(TAG, String.format("  %s: %d\n", name, value[0]));
				} else {
					// Log.w(TAG, String.format("  %s: failed\n", name));
					while (egl.eglGetError() != EGL10.EGL_SUCCESS)
						;
				}
			}
		}

		protected int mRedSize;
		protected int mGreenSize;
		protected int mBlueSize;
		protected int mAlphaSize;
		protected int mDepthSize;
		protected int mStencilSize;
		private int[] mValue = new int[1];
	}

	private static class Renderer implements GLSurfaceView.Renderer {
		String apkPath;
		SoundCommandListener cmdListener;
		ApplicationCommandListener appListener;

		Renderer(String apkPath, SoundCommandListener cmdListener, Activity activity) {
			this.apkPath = apkPath;
			this.cmdListener = cmdListener;
			this.appListener = new ApplicationCommandListener(activity);
		}

		public void onDrawFrame(GL10 gl) {
			String commands = GS2DJNI.mainLoop(GL2JNIView.getTouchDataString() + GL2JNIView.getAccelerometerDataString());
			if (!commands.equals("")) {
				cmdListener.parseAndExecuteCommands(commands);
				appListener.parseAndExecuteCommands(commands);
				Log.i("GS2D Native command", commands);
			}
		}

		public void onSurfaceChanged(GL10 gl, int width, int height) {
			GS2DJNI.resize(width, height);
		}

		public void onSurfaceCreated(GL10 gl, EGLConfig config) {
			GS2DJNI.start(apkPath, GL2JNIView.displayWidth, GL2JNIView.displayHeight);
		}
	}

	protected static String dataSegment(float v) {
		return "" + v + " ";
	}

	protected static String getTouchDataString() {
		String out = "" + touchCount + " ";
		for (int t = 0; t < touchCount; t++) {
			Vector2 touchPos = currentTouch[t];
			if (touchPos != null) {
				out += dataSegment(touchPos.x);
				out += dataSegment(touchPos.y);
			}
		}
		return out;
	}

	protected static String getAccelerometerDataString() {
		String out = "";
		out += dataSegment(accelerometerListener.getX());
		out += dataSegment(accelerometerListener.getY());
		out += dataSegment(accelerometerListener.getZ());
		return out;
	}

	@Override
	public boolean onTouchEvent(final MotionEvent event) {
		int momentTouchCount = Math.min(MAXIMUM_TOUCHES, event.getPointerCount());
		touchCount = 0;
		for (int t = 0; t < momentTouchCount; t++) {
			final float x = event.getX(t);
			final float y = event.getY(t);

			switch (event.getAction()) {
			case MotionEvent.ACTION_DOWN:
				currentTouch[t] = new Vector2(x, y);
				break;

			case MotionEvent.ACTION_UP:
				currentTouch[t] = null;
				break;

			case MotionEvent.ACTION_MOVE:
				currentTouch[t] = new Vector2(x, y);
				break;
			}

			if (currentTouch[t] != null) {
				touchCount++;
			}
		}
		return true;
	}

	public void destroy() {
		GS2DJNI.destroy();
	}
}
