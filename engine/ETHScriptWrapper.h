/*-----------------------------------------------------------------------

 Ethanon Engine (C) Copyright 2009-2011 Andre Santee
 http://www.asantee.net/ethanon/

    This file is part of Ethanon Engine.

    Ethanon Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    Ethanon Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with Ethanon Engine. If not, see
    <http://www.gnu.org/licenses/>.

-----------------------------------------------------------------------*/

#ifndef ETH_SCRIPT_WRAPPER_H_
#define ETH_SCRIPT_WRAPPER_H_

#include "ETHScene.h"
#include "ETHPrimitiveDrawer.h"
#include "../angelscript/include/angelscript.h"
#include "ETHInput.h"

class ETHScriptWrapper
{
	static str_type::string m_sceneFileName;
	static bool m_roundUpPosition;
	static bool m_runningMainFunction;

protected:
	static ETHResourceProviderPtr m_provider;

	//static Vector2 GetBGMin() { return m_v2BGMin; }
	//static Vector2 GetBGMax() { return m_v2BGMax; }
	//static str_type::string GetBGBitmapName() { return m_bgBimapName; }
	static bool IsRoundingUpPosition() { return m_roundUpPosition; }
	static bool IsRunningMainFunction() { return m_runningMainFunction; }

	static str_type::string GetSceneFileName();
	static ETHSpeedTimer m_timer;
	static std::list<boost::shared_ptr<ETHPrimitiveDrawer>> m_primitiveList;
	static ETHScenePtr m_pScene;
	static bool m_abort;
	static bool m_useLightmaps;
	static int m_argc;
	static str_type::char_t **m_argv;
	static ETHInput m_ethInput;
	static asIScriptModule *m_pASModule;
	static Vector2 m_v2LastCamPos;
	static void LoadSceneScripts();
	static Vector2 GetLastCameraPos();

	static asIScriptContext *m_pScriptContext;
	static int m_loopFunctionId;

	class ETH_NEXT_SCENE
	{
		str_type::string sceneName;
		str_type::string loopFunc;
		str_type::string preLoopFunc;
		Vector2 bucketSize;

	public:
		ETH_NEXT_SCENE();
		str_type::string GetSceneName() const;
		str_type::string GetPreLoopFunc() const;
		str_type::string GetLoopFunc() const;
		Vector2 GetBucketSize() const;
		void SetNextScene(const str_type::string &sceneName, const str_type::string &preLoopFunc, const str_type::string &loopFunc, const Vector2 &bucketSize);
		void Reset();
		bool HasNextScene() const;
	};

	static class Math
	{
	public:
		static void RegisterGlobals(asIScriptEngine *pASEngine);
	private:
		static float Length(const Vector3 &v);
		static float Length(const Vector2 &v);
		static float Sign(const float v);
		static int Sign(const int v);
		static float Distance(const Vector2 &a, const Vector2 &b);
		static float Distance(const Vector3 &a, const Vector3 &b);
	} m_math;

	void RegisterGlobalFunctions(asIScriptEngine *pASEngine);
	static bool RunMainFunction(const int mainFuncId);
	static bool WarnIfRunsInMainFunction(const str_type::string &functionName);

	static void LoadSceneInScript(const str_type::string &escFile);
	static void LoadSceneInScript(const str_type::string &escFile, const str_type::string &preLoopFunc, const str_type::string &loopFunc);
	static void LoadSceneInScript(const str_type::string &escFile, const str_type::string &preLoopFunc, const str_type::string &loopFunc, const Vector2 &v2BucketSize);
	static bool SaveScene(const str_type::string &escFile);
	static bool LoadScene(const str_type::string &escFile, const Vector2 &v2BucketSize);

	/// temporarily store the names of the next functions to load them after
	/// the script is finished
	static ETH_NEXT_SCENE m_nextScene;

	/// Shows a message to the programmer or user. If the message is ETH_ERROR, the program will be aborted
	static void ShowMessage(str_type::string message, const ETH_MESSAGE type, const bool abort = true);

	// ETHEntity methods wrappers
	static void SetPosition(ETHEntity *pEntity, const Vector3 &v3Pos);
	static void SetPositionXY(ETHEntity *pEntity, const Vector2 &v2Pos);
	static void AddToPosition(ETHEntity *pEntity, const Vector3 &v3Pos);
	static void AddToPositionXY(ETHEntity *pEntity, const Vector2 &v2Pos);
	static Vector2 GetCurrentBucket(ETHEntity *pEntity);
	// global wraps 

	static void HideCursor(const bool hide);
	static Vector2 GetCursorPos();
	static Vector2 GetCursorAbsolutePos();
	static str_type::string GetLastCharInput();
	static bool SetCursorPos(const Vector2 &v2Pos);
	static float GetTimeF();
	static unsigned long GetTime();
	static float UnitsPerSecond(const float speed);
	static void Exit();
	static void SetAmbientLight(const Vector3 &v3Color);
	static Vector3 GetAmbientLight();
	static unsigned int GetScreenWidth();
	static unsigned int GetScreenHeight();
	static Vector2 GetScreenSize();
	static void SetCameraPos(const Vector2 &v2Pos);
	static void AddToCameraPos(const Vector2 &v2Add);
	static void EnableLightmaps(const bool enable);
	static Vector2 GetCameraPos();
	static float GetFPSRate();
	static Vector2 ComputeCarretPosition(const str_type::string &font, const str_type::string &text, const unsigned int pos);
	static Vector2 ComputeTextBoxSize(const str_type::string &font, const str_type::string &text);
	static void DrawText(const Vector2 &v2Pos, const str_type::string &text, const str_type::string &font, const GS_DWORD color);
	static void DrawFadingText(const Vector2 &v2Pos, const str_type::string &text, const str_type::string &font, const GS_DWORD color, unsigned long time);
	static void AddLight(const Vector3 &v3Pos, const Vector3 &v3Color, const float range, const bool castShadows);
	static int AddEntity(const str_type::string &file, const Vector3 &v3Pos, const float angle, ETHEntity **ppOutEntity, const str_type::string &alternativeName);
	static int AddEntity(const str_type::string &file, const Vector3 &v3Pos, const float angle);
	static int AddEntity(const str_type::string &file, const Vector3 &v3Pos, ETHEntity **ppOutEntity);
	static int AddEntity(const str_type::string &file, const Vector3 &v3Pos, const str_type::string &alternativeName);
	static ETHEntity *DeleteEntity(ETHEntity *pEntity);
	static bool GenerateLightmaps();
	static ETHEntity *SeekEntity(const int id);
	static ETHEntity *SeekEntity(const str_type::string &name);
	static bool LoadMusic(const str_type::string &file);
	static bool LoadSoundEffect(const str_type::string &file);
	static bool PlaySample(const str_type::string &file);
	static bool LoopSample(const str_type::string &file, const bool loop);
	static bool StopSample(const str_type::string &file);
	static bool PauseSample(const str_type::string &file);
	static bool SetSampleVolume(const str_type::string &file, const float volume);
	static bool SetSamplePan(const str_type::string &file, const float pan);
	static bool SampleExists(const str_type::string &file);
	static bool IsSamplePlaying(const str_type::string &file);
	static unsigned int GetNumEntities();
	static int GetLastID();
	static bool AddFloatData(const str_type::string &entity, const str_type::string &name, const float value);
	static bool AddIntData(const str_type::string &entity, const str_type::string &name, const int value);
	static bool AddUIntData(const str_type::string &entity, const str_type::string &name, const unsigned int value);
	static bool AddStringData(const str_type::string &entity, const str_type::string &name, const str_type::string &value);
	static bool AddVector2Data(const str_type::string &entity, const str_type::string &name, const Vector2 &value);
	static bool AddVector3Data(const str_type::string &entity, const str_type::string &name, const Vector3 &value);
	static float GetMaxHeight();
	static float GetMinHeight();
	static void LoadSprite(const str_type::string &name);
	static void DrawSprite(const str_type::string &name, const Vector2 &v2Pos, const GS_DWORD color);
	static void DrawShaped(const str_type::string &name, const Vector2 &v2Pos, const Vector2 &v2Size, const GS_DWORD color);
	static void DrawSpriteZ(const str_type::string &name, const float depth, const Vector2 &v2Pos, const GS_DWORD color);
	static void DrawShapedZ(const str_type::string &name, const float depth, const Vector2 &v2Pos, const Vector2 &v2Size, const GS_DWORD color);
	static Vector2 GetSpriteSize(const str_type::string &name);
	static void DrawLine(const Vector2 &v2A, const Vector2 &v2B, const GS_COLOR a, const GS_COLOR b, const float width);
	// static void PlayCutscene(const str_type::string &fileName);
	static bool Windowed();
	static ETH_VIDEO_MODE GetVideoMode(const unsigned int mode);
	static unsigned int GetVideoModeCount();
	static void DrawRectangle(const Vector2 &v2Pos, const Vector2 &v2Size,
							  const GS_DWORD color0, const GS_DWORD color1,
							  const GS_DWORD color2, const GS_DWORD color3);
	static GS_DWORD GetBackgroundColor();
	static void SetBackgroundColor(const GS_DWORD color);
	//static bool SetBackgroundImage(const str_type::string &name);
	//static void PositionBackgroundImage(const Vector2 &v2Min, const Vector2 &v2Max);
	static Vector2 GetSystemScreenSize();
	static bool GetEntityArrayByName(const str_type::string &name, ETHEntityArray &outVector);
	static bool GetEntityArrayFromBucket(const Vector2 &v2Bucket, ETHEntityArray &outVector);
	static Vector2 GetBucket(const Vector2 &v2);
	static void SetPositionRoundUp(const bool roundUp);
	static bool GetPositionRoundUp();
	static void SetHaloRotation(const bool enable);
	static void EnableQuitKeys(const bool enable);
	static void EnableRealTimeShadows(const bool enable);
	static void GetVisibleEntities(ETHEntityArray &entityArray);
	static void GetIntersectingEntities(const Vector2 &v2Here, ETHEntityArray &outVector, const bool screenSpace);
	static int GetNumRenderedEntities();
	static void SetBorderBucketsDrawing(const bool enable);
	static bool IsDrawingBorderBuckets();
	static int GetArgc();
	static str_type::string GetArgv(const int n);
	static Vector2 GetWorldSpaceCursorPos2();
	static Vector3 GetWorldSpaceCursorPos3();
	//static void SetBackgroundAlphaAdd();
	//static void SetBackgroundAlphaPixel();
	//static void SetBackgroundAlphaModulate();
	static void UsePixelShaders(const bool enable);
	static bool IsPixelShaderSupported();

	static void Abort();

	static str_type::string GetProgramPath();
	static str_type::string GetAbsolutePath(const str_type::string &fileName);

	static ETHInput *GetInputHandle();
	static void SetWindowProperties(const str_type::string &winTitle, const unsigned int width, const unsigned int height, const bool windowed, const bool sync, const GS_PIXEL_FORMAT gsPF);

	static void Print(const str_type::string &str);
	static void PrintFloat(const float f);
	static void PrintInt(const int n);
	static void PrintUInt(const unsigned int n);

public:
	static bool Aborted();
	static ETHResourceProviderPtr GetProvider();
};

#endif