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

#include "ETHScriptWrapper.h"
#include "ETHShaderManager.h"
#include "ETHRenderEntity.h"

#ifdef GS2D_STR_TYPE_WCHAR
#include "../addons/utf16/scriptbuilder.h"
#else
#include "../addons/ansi/scriptbuilder.h"
#endif

ETHResourceProviderPtr ETHScriptWrapper::m_provider;
std::list<boost::shared_ptr<ETHPrimitiveDrawer>> ETHScriptWrapper::m_primitiveList;
ETHScenePtr ETHScriptWrapper::m_pScene;
bool ETHScriptWrapper::m_abort = false;
bool ETHScriptWrapper::m_useLightmaps = true;
ETHSpeedTimer ETHScriptWrapper::m_timer;
bool ETHScriptWrapper::m_roundUpPosition = true;
int ETHScriptWrapper::m_argc = 0;
str_type::char_t **ETHScriptWrapper::m_argv = 0;
ETHScriptWrapper::ETH_NEXT_SCENE ETHScriptWrapper::m_nextScene;
str_type::string ETHScriptWrapper::m_sceneFileName = GS_L("");
ETHInput ETHScriptWrapper::m_ethInput;
asIScriptModule *ETHScriptWrapper::m_pASModule = 0;
Vector2 ETHScriptWrapper::m_v2LastCamPos(0,0);
int ETHScriptWrapper::m_loopFunctionId =-1;
asIScriptContext *ETHScriptWrapper::m_pScriptContext = 0;
bool ETHScriptWrapper::m_runningMainFunction = false;
ETHScriptWrapper::Math ETHScriptWrapper::m_math;

ETHScriptWrapper::ETH_NEXT_SCENE::ETH_NEXT_SCENE()
{
	Reset();
}
void ETHScriptWrapper::ETH_NEXT_SCENE::Reset()
{
	sceneName = GS_L("");
	loopFunc = GS_L("");
	preLoopFunc = GS_L("");
	bucketSize = Vector2(_ETH_DEFAULT_BUCKET_SIZE,_ETH_DEFAULT_BUCKET_SIZE);
}

bool ETHScriptWrapper::ETH_NEXT_SCENE::HasNextScene() const
{
	return (sceneName != GS_L(""));
}

str_type::string ETHScriptWrapper::ETH_NEXT_SCENE::GetSceneName() const
{
	return sceneName;
}

str_type::string ETHScriptWrapper::ETH_NEXT_SCENE::GetPreLoopFunc() const
{
	return preLoopFunc;
}

str_type::string ETHScriptWrapper::ETH_NEXT_SCENE::GetLoopFunc() const
{
	return loopFunc;
}

Vector2 ETHScriptWrapper::ETH_NEXT_SCENE::GetBucketSize() const
{
	return bucketSize;
}

void ETHScriptWrapper::ETH_NEXT_SCENE::SetNextScene(const str_type::string &sceneName, const str_type::string &preLoopFunc, const str_type::string &loopFunc, const Vector2 &bucketSize)
{
	this->sceneName = sceneName;
	this->preLoopFunc = preLoopFunc;
	this->loopFunc = loopFunc;
	this->bucketSize = bucketSize;
}

bool ETHScriptWrapper::RunMainFunction(const int mainFuncId)
{
	if (mainFuncId < 0)
		return false;

	m_runningMainFunction = true;
	ETHGlobal::ExecuteContext(m_pScriptContext, mainFuncId, true);
	m_runningMainFunction = false;
	return true;
}

bool ETHScriptWrapper::WarnIfRunsInMainFunction(const str_type::string &functionName)
{
	if (IsRunningMainFunction())
	{
		ETH_STREAM_DECL(ss) << std::endl << GS_L("WARNING: do not load resources or do scene-related operations inside the main() function.") << std::endl
			<< GS_L("Use pre-loop or loop functions instead.") << std::endl
			<< GS_L("Function used: ") << functionName << std::endl;
		m_provider->Log(ss.str(), Platform::Logger::ERROR);
		return true;
	}
	return false;
}

void ETHScriptWrapper::RegisterGlobalFunctions(asIScriptEngine *pASEngine)
{
	int r;

	// Register input
	r = pASEngine->RegisterObjectMethod("ETHInput", "vector2 GetCursorPos()", asFUNCTION(GetCursorPos), asCALL_CDECL_OBJLAST); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHInput", "vector2 GetCursorAbsolutePos()", asFUNCTION(GetCursorAbsolutePos), asCALL_CDECL_OBJLAST); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHInput", "void SetCursorPos(const vector2 &in pos)", asFUNCTION(SetCursorPos), asCALL_CDECL_OBJLAST); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHInput", "string GetLastCharInput() const", asFUNCTION(GetLastCharInput), asCALL_CDECL_OBJLAST); assert(r >= 0);
	// ETHInput retriever
	r = pASEngine->RegisterGlobalFunction("ETHInput @GetInputHandle()", asFUNCTION(GetInputHandle), asCALL_CDECL); assert(r >= 0);

	// register ETHEntity retrievers
	r = pASEngine->RegisterObjectMethod("ETHEntity", "void SetPosition(const vector3 &in)", asFUNCTION(SetPosition), asCALL_CDECL_OBJFIRST); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "void SetPositionXY(const vector2 &in)", asFUNCTION(SetPositionXY), asCALL_CDECL_OBJFIRST); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "void AddToPosition(const vector3 &in)", asFUNCTION(AddToPosition), asCALL_CDECL_OBJFIRST); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "void AddToPositionXY(const vector2 &in)", asFUNCTION(AddToPositionXY), asCALL_CDECL_OBJFIRST); assert(r >= 0);
	r = pASEngine->RegisterObjectMethod("ETHEntity", "vector2 GetCurrentBucket() const", asFUNCTION(GetCurrentBucket), asCALL_CDECL_OBJFIRST); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("ETHEntity @SeekEntity(const string &in)", asFUNCTIONPR(SeekEntity, (const str_type::string&), ETHEntity*), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("ETHEntity @SeekEntity(const int)", asFUNCTIONPR(SeekEntity, (const int), ETHEntity*), asCALL_CDECL); assert(r >= 0);

	r = pASEngine->RegisterGlobalFunction("void print(const string &in)", asFUNCTION(Print), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("void print(const float)", asFUNCTION(PrintFloat), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("void print(const int)", asFUNCTION(PrintInt), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("void print(const uint)", asFUNCTION(PrintUInt), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("void LoadScene(const string &in)", asFUNCTIONPR(LoadSceneInScript, (const str_type::string &), void), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("void LoadScene(const string &in, const string &in, const string &in)", asFUNCTIONPR(LoadSceneInScript, (const str_type::string&,const str_type::string&,const str_type::string&), void), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("void LoadScene(const string &in, const string &in, const string &in, const vector2 &in)", asFUNCTIONPR(LoadSceneInScript, (const str_type::string&,const str_type::string&,const str_type::string&, const Vector2&), void), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("float GetTimeF()", asFUNCTION(GetTimeF), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("uint GetTime()", asFUNCTION(GetTime), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("float UnitsPerSecond(const float)", asFUNCTION(UnitsPerSecond), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("void Exit()", asFUNCTION(Exit), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("int AddEntity(const string &in, const vector3 &in, const float)", asFUNCTIONPR(AddEntity, (const str_type::string &file, const Vector3 &v3Pos, const float angle), int), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("int AddEntity(const string &in, const vector3 &in, ETHEntity@ &out)", asFUNCTIONPR(AddEntity, (const str_type::string &file, const Vector3 &v3Pos, ETHEntity **), int), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("int AddEntity(const string &in, const vector3 &in, const string &in)", asFUNCTIONPR(AddEntity, (const str_type::string &file, const Vector3 &v3Pos, const str_type::string &alternativeName), int), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("ETHEntity @DeleteEntity(ETHEntity @)", asFUNCTION(DeleteEntity), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("bool GenerateLightmaps()", asFUNCTION(GenerateLightmaps), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("int rand(const int)", asFUNCTIONPR(Randomizer::Int, (const int), int), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("int rand(const int, const int)", asFUNCTIONPR(Randomizer::Int, (const int, const int), int), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("float randF(const float)", asFUNCTIONPR(Randomizer::Float, (const float), float), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("float randF(const float, const float)", asFUNCTIONPR(Randomizer::Float, (const float, const float), float), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("void SetAmbientLight(const vector3 &in)", asFUNCTION(SetAmbientLight), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("vector3 GetAmbientLight()", asFUNCTION(GetAmbientLight), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction(
		"void SetWindowProperties(const string &in, const uint, const uint, const bool, const bool, const PIXEL_FORMAT)",
		asFUNCTION(SetWindowProperties), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("uint GetScreenWidth()", asFUNCTION(GetScreenWidth), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("uint GetScreenHeight()", asFUNCTION(GetScreenHeight), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("void SetCameraPos(const vector2 &in)", asFUNCTION(SetCameraPos), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("void AddToCameraPos(const vector2 &in)", asFUNCTION(AddToCameraPos), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("vector2 GetCameraPos()", asFUNCTION(GetCameraPos), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("void EnableLightmaps(const bool)", asFUNCTION(EnableLightmaps), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("void UsePixelShaders(const bool)", asFUNCTION(UsePixelShaders), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("void DrawText(const vector2 &in, const string &in, const string &in, const uint)", asFUNCTION(DrawText), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("void DrawFadingText(const vector2 &in, const string &in, const string &in, const uint, const uint)", asFUNCTION(DrawFadingText), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("float GetFPSRate()", asFUNCTION(GetFPSRate), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("void AddLight(const vector3 &in, const vector3 &in, const float, const bool)", asFUNCTION(AddLight), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("void HideCursor(const bool)", asFUNCTION(HideCursor), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("bool LoadMusic(const string &in)", asFUNCTION(LoadMusic), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("bool LoadSoundEffect(const string &in)", asFUNCTION(LoadSoundEffect), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("bool PlaySample(const string &in)", asFUNCTION(PlaySample), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("bool LoopSample(const string &in, const bool)", asFUNCTION(LoopSample), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("bool StopSample(const string &in)", asFUNCTION(StopSample), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("bool PauseSample(const string &in)", asFUNCTION(PauseSample), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("bool SetSampleVolume(const string &in, const float)", asFUNCTION(SetSampleVolume), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("bool SetSamplePan(const string &in, const float)", asFUNCTION(SetSamplePan), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("bool SampleExists(const string &in)", asFUNCTION(SampleExists), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("bool IsSamplePlaying(const string &in)", asFUNCTION(IsSamplePlaying), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("uint GetNumEntities()", asFUNCTION(GetNumEntities), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("int GetLastID()", asFUNCTION(GetLastID), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("bool AddFloatData(const string &in, const string &in, const float)", asFUNCTION(AddFloatData), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("bool AddIntData(const string &in, const string &in, const int)", asFUNCTION(AddIntData), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("bool AddUIntData(const string &in, const string &in, const uint)", asFUNCTION(AddUIntData), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("bool AddStringData(const string &in, const string &in, const string &in)", asFUNCTION(AddStringData), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("bool AddVector2Data(const string &in, const string &in, const vector2 &in)", asFUNCTION(AddVector2Data), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("bool AddVector3Data(const string &in, const string &in, const vector3 &in)", asFUNCTION(AddVector3Data), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("bool SaveScene(const string &in)", asFUNCTION(SaveScene), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("float GetMaxHeight()", asFUNCTION(GetMaxHeight), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("float GetMinHeight()", asFUNCTION(GetMinHeight), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("vector3 normalize(const vector3 &in)", asFUNCTIONPR(Normalize, (const Vector3&), Vector3), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("vector2 normalize(const vector2 &in)", asFUNCTIONPR(Normalize, (const Vector2&), Vector2), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("float radianToDegree(const float)", asFUNCTION(RadianToDegree), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("float degreeToRadian(const float)", asFUNCTION(DegreeToRadian), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("uint ARGB(const uint8, const uint8, const uint8, const uint8)", asFUNCTION(ARGB), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("void LoadSprite(const string &in)", asFUNCTION(LoadSprite), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("void DrawSprite(const string &in, const vector2 &in, const uint)", asFUNCTION(DrawSprite), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("void DrawShapedSprite(const string &in, const vector2 &in, const vector2 &in, const uint)", asFUNCTION(DrawShaped), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("vector2 GetSpriteSize(const string &in)", asFUNCTION(GetSpriteSize), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("void DrawRectangle(const vector2 &in, const vector2 &in, const uint, const uint, const uint, const uint)", asFUNCTION(DrawRectangle), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("void DrawLine(const vector2 &in, const vector2 &in, const uint, const uint, const float)", asFUNCTION(DrawLine), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("string GetSceneFileName()", asFUNCTION(GetSceneFileName), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("void PlayCutscene(const string &in)", asFUNCTION(PlayCutscene), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("vector2 GetScreenSize()", asFUNCTION(GetScreenSize), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("bool Windowed()", asFUNCTION(Windowed), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("uint GetVideoModeCount()", asFUNCTION(GetVideoModeCount), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("videoMode GetVideoMode(const uint)", asFUNCTION(GetVideoMode), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("uint GetBackgroundColor()", asFUNCTION(GetBackgroundColor), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("void SetBackgroundColor(const uint)", asFUNCTION(SetBackgroundColor), asCALL_CDECL); assert(r >= 0);
	//r = pASEngine->RegisterGlobalFunction("bool SetBackgroundImage(const string &in)", asFUNCTION(SetBackgroundImage), asCALL_CDECL); assert(r >= 0);
	//r = pASEngine->RegisterGlobalFunction("void PositionBackgroundImage(const vector2 &in, const vector2 &in)", asFUNCTION(PositionBackgroundImage), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("vector2 GetSystemScreenSize()", asFUNCTION(GetSystemScreenSize), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("bool GetEntityArray(const string &in, ETHEntityArray &)", asFUNCTION(GetEntityArrayByName), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("bool GetEntitiesFromBucket(const vector2 &in, ETHEntityArray &)", asFUNCTION(GetEntityArrayFromBucket), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("void GetVisibleEntities(ETHEntityArray &)", asFUNCTION(GetVisibleEntities), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("void GetIntersectingEntities(const vector2 &in, ETHEntityArray &, const bool)", asFUNCTION(GetIntersectingEntities), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("vector2 GetBucket(const vector2 &in)", asFUNCTION(GetBucket), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("bool IsPixelShaderSupported()", asFUNCTION(IsPixelShaderSupported), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("string GetProgramPath()", asFUNCTION(GetProgramPath), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("void DrawSpriteZ(const string &in, const float, const vector2 &in, const uint)", asFUNCTION(DrawSpriteZ), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("void DrawShapedSpriteZ(const string &in, const float, const vector2 &in, const vector2 &in, const uint)", asFUNCTION(DrawShapedZ), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("void SetPositionRoundUp(const bool)", asFUNCTION(SetPositionRoundUp), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("bool GetPositionRoundUp()", asFUNCTION(GetPositionRoundUp), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("void SetHaloRotation(const bool)", asFUNCTION(SetHaloRotation), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("void EnableQuitKeys(const bool)", asFUNCTION(EnableQuitKeys), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("void EnableRealTimeShadows(const bool)", asFUNCTION(EnableRealTimeShadows), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("void SetBorderBucketsDrawing(const bool)", asFUNCTION(SetBorderBucketsDrawing), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("bool IsDrawingBorderBuckets()", asFUNCTION(IsDrawingBorderBuckets), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("string GetAbsolutePath(const string &in)", asFUNCTION(GetAbsolutePath), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("vector2 GetLastCameraPos()", asFUNCTION(GetLastCameraPos), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("int GetNumRenderedEntities()", asFUNCTION(GetNumRenderedEntities), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("int ParseInt(const string &in)", asFUNCTION(ETHGlobal::ParseIntStd), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("uint ParseUInt(const string &in)", asFUNCTION(ETHGlobal::ParseUIntStd), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("float ParseFloat(const string &in)", asFUNCTION(ETHGlobal::ParseFloatStd), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("int GetArgc()", asFUNCTION(GetArgc), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("string GetArgv(const int)", asFUNCTION(GetArgv), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("vector2 GetWorldSpaceCursorPos2()", asFUNCTION(GetWorldSpaceCursorPos2), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("vector3 GetWorldSpaceCursorPos3()", asFUNCTION(GetWorldSpaceCursorPos3), asCALL_CDECL); assert(r >= 0);
	//r = pASEngine->RegisterGlobalFunction("void SetBackgroundAlphaModulate()", asFUNCTION(SetBackgroundAlphaModulate), asCALL_CDECL); assert(r >= 0);
	//r = pASEngine->RegisterGlobalFunction("void SetBackgroundAlphaAdd()", asFUNCTION(SetBackgroundAlphaAdd), asCALL_CDECL); assert(r >= 0);
	//r = pASEngine->RegisterGlobalFunction("void SetBackgroundAlphaPixel()", asFUNCTION(SetBackgroundAlphaPixel), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("vector2 ComputeCarretPosition(const string &in, const string &in, const uint pos)", asFUNCTION(ComputeCarretPosition), asCALL_CDECL); assert(r >= 0);
	r = pASEngine->RegisterGlobalFunction("vector2 ComputeTextBoxSize(const string &in, const string &in)", asFUNCTION(ComputeTextBoxSize), asCALL_CDECL); assert(r >= 0);

	m_math.RegisterGlobals(pASEngine);
}

void ETHScriptWrapper::Abort()
{
	m_abort = true;
}

bool ETHScriptWrapper::Aborted()
{
	return m_abort;
}

void ETHScriptWrapper::ShowMessage(str_type::string sMsg, const ETH_MESSAGE type, const bool abort)
{
	str_type::stringstream ss;
	Platform::Logger::TYPE logType;
	switch (type)
	{
	case ETH_ERROR:
		ss << GS_L("ERROR - ");
		logType = Platform::Logger::ERROR;
		if (abort)
		{
			Abort();
		}
		break;
	case ETH_WARNING:
		ss << GS_L("Warning - ");
		logType = Platform::Logger::WARNING;
		break;
	default:
		logType = Platform::Logger::INFO;
	};
	ss << sMsg;
	m_provider->Log(ss.str(), logType);
}

ETHEntity *ETHScriptWrapper::SeekEntity(const int id)
{
	if (WarnIfRunsInMainFunction(GS_L("SeekEntity")))
		return 0;

	ETHEntity *pEntity = m_pScene->GetBucketManager().SeekEntity(id);
	if (pEntity)
	{
		// don't let it return temporary handles
		if (pEntity->IsTemporary())
		{
			return 0;
		}
		else
		{
			pEntity->AddRef();
			return pEntity;
		}
	}
	return 0;
}

ETHEntity *ETHScriptWrapper::SeekEntity(const str_type::string &name)
{
	if (WarnIfRunsInMainFunction(GS_L("SeekEntity")))
		return 0;

	ETHEntity *pEntity = m_pScene->GetBucketManager().SeekEntity(name);
	if (pEntity)
	{
		// don't let it return temporary handles
		if (pEntity->IsTemporary())
		{
			return 0;
		}
		else
		{
			pEntity->AddRef();
			return pEntity;
		}
	}
	return 0;
}

int ETHScriptWrapper::AddEntity(const str_type::string &file, const Vector3 &v3Pos, const float angle, ETHEntity **ppOutEntity, const str_type::string &alternativeName)
{
	if (WarnIfRunsInMainFunction(GS_L("AddEntity")))
		return -1;

	str_type::string fileName = m_provider->GetResourcePath();
	fileName += ETHDirectories::GetEntityPath();
	fileName += file;

	ETHRenderEntity* entity = new ETHRenderEntity(fileName, m_provider);
	entity->SetOrphanPosition(v3Pos);
	entity->SetAngle(angle);

	if (entity->IsStatic() && entity->IsApplyLight())
	{
		str_type::stringstream ss;
		ss << GS_L("AddEntity - This is a static entity and its lightmap has not been rendered yet. ") << 
			GS_L("It might be incorrectly lit: ") << fileName;
		ShowMessage(ss.str(), ETH_WARNING);
	}

	if (ppOutEntity)
	{
		if (!entity->IsTemporary())
		{
			entity->AddRef();
			*ppOutEntity = entity;
		}
	}

	return m_pScene->AddEntity(entity, alternativeName);
}

int ETHScriptWrapper::AddEntity(const str_type::string &file, const Vector3 &v3Pos, const str_type::string &alternativeName)
{
	return AddEntity(file, v3Pos, 0.0f, 0, alternativeName);
}


int ETHScriptWrapper::AddEntity(const str_type::string &file, const Vector3 &v3Pos, ETHEntity **ppOutEntity)
{
	return AddEntity(file, v3Pos, 0.0f, ppOutEntity, GS_L(""));
}

int ETHScriptWrapper::AddEntity(const str_type::string &file, const Vector3 &v3Pos, const float angle)
{
	return AddEntity(file, v3Pos, angle, 0, GS_L(""));
}

ETHEntity *ETHScriptWrapper::DeleteEntity(ETHEntity *pEntity)
{
	if (WarnIfRunsInMainFunction(GS_L("DeleteEntity")))
		return 0;

	if (!pEntity)
		return 0;
	if (m_pScene->GetBucketManager().DeleteEntity(pEntity->GetID(), ETHGlobal::GetBucket(pEntity->GetPositionXY(), m_pScene->GetBucketSize())))
	{
		pEntity->Release();
		return 0;
	}
	return pEntity;
}

bool ETHScriptWrapper::GenerateLightmaps()
{
	if (!m_useLightmaps)
		return false;
	return m_pScene->GenerateLightmaps();
}

void ETHScriptWrapper::PrintFloat(const float f)
{
	ETH_STREAM_DECL(ss) << f;
	m_provider->Log(ss.str(), Platform::FileLogger::INFO);
}

void ETHScriptWrapper::PrintInt(const int n)
{
	ETH_STREAM_DECL(ss) << n;
	m_provider->Log(ss.str(), Platform::FileLogger::INFO);
}

void ETHScriptWrapper::PrintUInt(const unsigned int n)
{
	ETH_STREAM_DECL(ss) << n;
	m_provider->Log(ss.str(), Platform::FileLogger::INFO);
}

void ETHScriptWrapper::Print(const str_type::string &str)
{
	ETH_STREAM_DECL(ss) << str;
	m_provider->Log(ss.str(), Platform::FileLogger::INFO);
}

Vector2 ETHScriptWrapper::GetCursorPos()
{
	return m_provider->GetInput()->GetCursorPositionF(m_provider->GetVideo());
}

Vector2 ETHScriptWrapper::GetCursorAbsolutePos()
{
	return m_provider->GetInput()->GetCursorPositionF(VideoPtr());
}

bool ETHScriptWrapper::SetCursorPos(const Vector2 &v2Pos)
{
	return m_provider->GetInput()->SetCursorPositionF(v2Pos);
}

float ETHScriptWrapper::GetTimeF()
{
	return m_provider->GetVideo()->GetElapsedTimeF();
}

unsigned long ETHScriptWrapper::GetTime()
{
	return m_provider->GetVideo()->GetElapsedTime();
}

float ETHScriptWrapper::UnitsPerSecond(const float speed)
{
	return m_timer.UnitsPerSecond(speed);
}

void ETHScriptWrapper::Exit()
{
	m_provider->GetVideo()->Quit();
}

void ETHScriptWrapper::SetAmbientLight(const Vector3 &v3Color)
{
	if (WarnIfRunsInMainFunction(GS_L("SetAmbientLight")))
		return;

	m_pScene->SetAmbientLight(v3Color);
}

Vector3 ETHScriptWrapper::GetAmbientLight()
{
	if (WarnIfRunsInMainFunction(GS_L("GetAmbientLight")))
		return Vector3(0,0,0);

	return m_pScene->GetAmbientLight();
}

unsigned int ETHScriptWrapper::GetScreenWidth()
{
	return m_provider->GetVideo()->GetScreenSize().x;
}

unsigned int ETHScriptWrapper::GetScreenHeight()
{
	return m_provider->GetVideo()->GetScreenSize().y;
}

Vector2 ETHScriptWrapper::GetScreenSize()
{
	return m_provider->GetVideo()->GetScreenSizeF();
}

void ETHScriptWrapper::SetCameraPos(const Vector2 &v2Pos)
{
	if (WarnIfRunsInMainFunction(GS_L("SetCameraPos")))
		return;

	// rounds up camera final position
	m_provider->GetVideo()->RoundUpPosition(m_roundUpPosition);
	m_provider->GetVideo()->SetCameraPos(v2Pos);
	m_provider->GetVideo()->RoundUpPosition(false);
}

void ETHScriptWrapper::AddToCameraPos(const Vector2 &v2Add)
{
	if (WarnIfRunsInMainFunction(GS_L("AddToCameraPos")))
		return;

	// rounds up camera final position
	m_provider->GetVideo()->RoundUpPosition(m_roundUpPosition);
	m_provider->GetVideo()->MoveCamera(v2Add);
	m_provider->GetVideo()->RoundUpPosition(false);
}

void ETHScriptWrapper::EnableLightmaps(const bool enable)
{
	if (WarnIfRunsInMainFunction(GS_L("EnableLightmaps")))
		return;

	m_useLightmaps = enable;
	m_pScene->EnableLightmaps(enable);
}

Vector2 ETHScriptWrapper::GetCameraPos()
{
	if (WarnIfRunsInMainFunction(GS_L("GetCameraPos")))
		return Vector2(0,0);

	return m_provider->GetVideo()->GetCameraPos();
}

float ETHScriptWrapper::GetFPSRate()
{
	return m_provider->GetVideo()->GetFPSRate();
}

Vector2 ETHScriptWrapper::ComputeCarretPosition(const str_type::string &font, const str_type::string &text, const unsigned int pos)
{
	return m_provider->GetVideo()->ComputeCarretPosition(font, text, pos);
}

Vector2 ETHScriptWrapper::ComputeTextBoxSize(const str_type::string &font, const str_type::string &text)
{
	return m_provider->GetVideo()->ComputeTextBoxSize(font, text);
}

void ETHScriptWrapper::DrawText(const Vector2 &v2Pos, const str_type::string &text, const str_type::string &font, const GS_DWORD color)
{
	m_primitiveList.push_back(
		boost::shared_ptr<ETHPrimitiveDrawer>(
				new ETHTextDrawer(
				m_provider, v2Pos, text, font, color, 0x0, 0x0
			)
		)
	);
}

void ETHScriptWrapper::LoadSprite(const str_type::string &name)
{
	if (WarnIfRunsInMainFunction(GS_L("LoadSprite")))
		return;

	str_type::string path = m_provider->GetResourcePath();
	path += name;
	m_provider->GetGraphicResourceManager()->AddFile(m_provider->GetVideo(), path, false);
}

Vector2 ETHScriptWrapper::GetSpriteSize(const str_type::string &name)
{
	SpritePtr pSprite = m_provider->GetGraphicResourceManager()->GetPointer(m_provider->GetVideo(), name, m_provider->GetResourcePath(), GS_L(""), false);
	if (pSprite)
		return pSprite->GetBitmapSizeF();
	return Vector2(0,0);
}

void ETHScriptWrapper::DrawSprite(const str_type::string &name, const Vector2 &v2Pos, const GS_DWORD color)
{
	m_primitiveList.push_back(boost::shared_ptr<ETHPrimitiveDrawer>(
		new ETHSpriteDrawer(m_provider, m_provider->GetGraphicResourceManager(), m_provider->GetResourcePath(), name, v2Pos, Vector2(0,0), color)));
}

void ETHScriptWrapper::DrawShaped(const str_type::string &name, const Vector2 &v2Pos, const Vector2 &v2Size, const GS_DWORD color)
{
	m_primitiveList.push_back(boost::shared_ptr<ETHPrimitiveDrawer>(
		new ETHSpriteDrawer(m_provider, m_provider->GetGraphicResourceManager(), m_provider->GetResourcePath(), name, v2Pos, v2Size, color)));
}

void ETHScriptWrapper::DrawSpriteZ(const str_type::string &name, const float depth, const Vector2 &v2Pos, const GS_DWORD color)
{
	m_primitiveList.push_back(boost::shared_ptr<ETHPrimitiveDrawer>(
		new ETHSpriteDrawer(m_provider, m_provider->GetGraphicResourceManager(), m_provider->GetResourcePath(), name, v2Pos, Vector2(0,0), color, depth)));
}

void ETHScriptWrapper::DrawShapedZ(const str_type::string &name, const float depth, const Vector2 &v2Pos, const Vector2 &v2Size, const GS_DWORD color)
{
	m_primitiveList.push_back(boost::shared_ptr<ETHPrimitiveDrawer>(
		new ETHSpriteDrawer(m_provider, m_provider->GetGraphicResourceManager(), m_provider->GetResourcePath(), name, v2Pos, v2Size, color, depth)));
}

void ETHScriptWrapper::DrawFadingText(const Vector2 &v2Pos, const str_type::string &text, const str_type::string &font, const GS_DWORD color, unsigned long time)
{
	m_primitiveList.push_back(
		boost::shared_ptr<ETHPrimitiveDrawer>(
			new ETHTextDrawer(
				m_provider, v2Pos, text, font, 
				color, time, m_provider->GetVideo()->GetElapsedTime()
			)
		)
	);
}

void ETHScriptWrapper::AddLight(const Vector3 &v3Pos, const Vector3 &v3Color, const float range, const bool castShadows)
{
	if (WarnIfRunsInMainFunction(GS_L("AddLight")))
		return;

	ETHLight light(true);
	light.castShadows = castShadows;
	light.color = v3Color;
	light.pos = v3Pos;
	light.range = range;
	light.staticLight = false;
	m_pScene->AddLight(light);
}

void ETHScriptWrapper::HideCursor(const bool hide)
{
	m_provider->GetVideo()->HideCursor(hide);
}

bool ETHScriptWrapper::LoadMusic(const str_type::string &file)
{
	if (WarnIfRunsInMainFunction(GS_L("LoadMusic")))
		return false;

	str_type::stringstream ss;
	ss << m_provider->GetResourcePath() << file;
	if (!m_provider->GetAudioResourceManager()->AddFile(m_provider->GetAudio(), ss.str(), GSST_MUSIC))
	{
		ShowMessage(GS_L("Could not load the file: ") + file, ETH_ERROR, false);
		return false;
	}
	return true;
}

bool ETHScriptWrapper::LoadSoundEffect(const str_type::string &file)
{
	if (WarnIfRunsInMainFunction(GS_L("LoadSoundEffect")))
		return false;

	str_type::stringstream ss;
	ss << m_provider->GetResourcePath() << file;
	if (!m_provider->GetAudioResourceManager()->AddFile(m_provider->GetAudio(), ss.str().c_str(), GSST_SOUND_EFFECT))
	{
		ShowMessage(GS_L("Could not load the file: ") + file, ETH_ERROR, false);
		return false;
	}
	return true;
}

bool ETHScriptWrapper::PlaySample(const str_type::string &file)
{
	AudioSamplePtr pSample = m_provider->GetAudioResourceManager()->GetPointer(m_provider->GetAudio(), file, m_provider->GetResourcePath(), GS_L(""), GSST_UNKNOWN);
	if (!pSample)
	{
		ShowMessage(GS_L("File not found: ") + file, ETH_ERROR, false);
		return false;
	}
	pSample->Play();
	return true;
}

bool ETHScriptWrapper::LoopSample(const str_type::string &file, const bool loop)
{
	AudioSamplePtr pSample = m_provider->GetAudioResourceManager()->GetPointer(m_provider->GetAudio(), file, m_provider->GetResourcePath(), GS_L(""), GSST_UNKNOWN);
	if (!pSample)
	{
		ShowMessage(GS_L("File not found: ") + file, ETH_ERROR, false);
		return false;
	}
	pSample->SetLoop(loop);
	return true;
}

bool ETHScriptWrapper::StopSample(const str_type::string &file)
{
	AudioSamplePtr pSample = m_provider->GetAudioResourceManager()->GetPointer(m_provider->GetAudio(), file, m_provider->GetResourcePath(), GS_L(""), GSST_UNKNOWN);
	if (!pSample)
	{
		ShowMessage(GS_L("File not found: ") + file, ETH_ERROR, false);
		return false;
	}
	pSample->Stop();
	return true;
}

bool ETHScriptWrapper::PauseSample(const str_type::string &file)
{
	AudioSamplePtr pSample = m_provider->GetAudioResourceManager()->GetPointer(m_provider->GetAudio(), file, m_provider->GetResourcePath(), GS_L(""), GSST_UNKNOWN);
	if (!pSample)
	{
		ShowMessage(GS_L("File not found: ") + file, ETH_ERROR, false);
		return false;
	}
	pSample->Pause();
	return true;
}

bool ETHScriptWrapper::SetSampleVolume(const str_type::string &file, const float volume)
{
	AudioSamplePtr pSample = m_provider->GetAudioResourceManager()->GetPointer(m_provider->GetAudio(), file, m_provider->GetResourcePath(), GS_L(""), GSST_UNKNOWN);
	if (!pSample)
	{
		ShowMessage(GS_L("File not found: ") + file, ETH_ERROR, false);
		return false;
	}
	const float volumeFix = Max(Min(volume, 1.0f), 0.0f);
	pSample->SetVolume(volumeFix);
	return true;
}

bool ETHScriptWrapper::SetSamplePan(const str_type::string &file, const float pan)
{
	AudioSamplePtr pSample = m_provider->GetAudioResourceManager()->GetPointer(m_provider->GetAudio(), file, m_provider->GetResourcePath(), GS_L(""), GSST_UNKNOWN);
	if (!pSample)
	{
		ShowMessage(GS_L("File not found: ") + file, ETH_ERROR, false);
		return false;
	}
	pSample->SetPan(pan);
	return true;
}

bool ETHScriptWrapper::SampleExists(const str_type::string &file)
{
	AudioSamplePtr pSample = m_provider->GetAudioResourceManager()->GetPointer(m_provider->GetAudio(), file, m_provider->GetResourcePath(), GS_L(""), GSST_UNKNOWN);
	if (!pSample)
		return false;
	return true;
}

bool ETHScriptWrapper::IsSamplePlaying(const str_type::string &file)
{
	AudioSamplePtr pSample = m_provider->GetAudioResourceManager()->GetPointer(m_provider->GetAudio(), file, m_provider->GetResourcePath(), GS_L(""), GSST_UNKNOWN);
	if (!pSample)
	{
		ShowMessage(GS_L("File not found: ") + file, ETH_ERROR, false);
		return false;
	}
	return pSample->IsPlaying();
}

unsigned int ETHScriptWrapper::GetNumEntities()
{
	if (WarnIfRunsInMainFunction(GS_L("GetNumEntities")))
		return 0;

	return m_pScene->GetNumEntities();
}

int ETHScriptWrapper::GetLastID()
{
	if (WarnIfRunsInMainFunction(GS_L("GetLastID")))
		return -1;

	return m_pScene->GetLastID();
}

bool ETHScriptWrapper::AddFloatData(const str_type::string &entity, const str_type::string &name, const float value)
{
	if (WarnIfRunsInMainFunction(GS_L("AddFloatData")))
		return false;
	return m_pScene->AddFloatData(entity, name, value);
}

bool ETHScriptWrapper::AddIntData(const str_type::string &entity, const str_type::string &name, const int value)
{
	if (WarnIfRunsInMainFunction(GS_L("AddIntData")))
		return false;
	return m_pScene->AddIntData(entity, name, value);
}

bool ETHScriptWrapper::AddUIntData(const str_type::string &entity, const str_type::string &name, const unsigned int value)
{
	if (WarnIfRunsInMainFunction(GS_L("AddUIntData")))
		return false;
	return m_pScene->AddUIntData(entity, name, value);
}

bool ETHScriptWrapper::AddStringData(const str_type::string &entity, const str_type::string &name, const str_type::string &value)
{
	if (WarnIfRunsInMainFunction(GS_L("AddStringData")))
		return false;
	return m_pScene->AddStringData(entity, name, value);
}

bool ETHScriptWrapper::AddVector2Data(const str_type::string &entity, const str_type::string &name, const Vector2 &value)
{
	if (WarnIfRunsInMainFunction(GS_L("AddVector2Data")))
		return false;
	return m_pScene->AddVector2Data(entity, name, value);
}

bool ETHScriptWrapper::AddVector3Data(const str_type::string &entity, const str_type::string &name, const Vector3 &value)
{
	if (WarnIfRunsInMainFunction(GS_L("AddVector3Data")))
		return false;
	return m_pScene->AddVector3Data(entity, name, value);
}

float ETHScriptWrapper::GetMaxHeight()
{
	if (WarnIfRunsInMainFunction(GS_L("GetMaxHeight")))
		return 0.0f;
	return m_pScene->GetMaxHeight();
}

float ETHScriptWrapper::GetMinHeight()
{
	if (WarnIfRunsInMainFunction(GS_L("GetMinHeight")))
		return 0.0f;
	return m_pScene->GetMinHeight();
}

str_type::string ETHScriptWrapper::GetLastCharInput()
{
	str_type::stringstream ss;
	str_type::char_t lastChar = m_provider->GetInput()->GetLastCharInput();
	if (lastChar)
	{
		ss << lastChar;
		return ss.str();
	}
	return GS_L("");
}

void ETHScriptWrapper::DrawRectangle(const Vector2 &v2Pos, const Vector2 &v2Size,
							  const GS_DWORD color0, const GS_DWORD color1,
							  const GS_DWORD color2, const GS_DWORD color3)
{
	m_primitiveList.push_back(boost::shared_ptr<ETHPrimitiveDrawer>(new ETHRectangleDrawer(m_provider, v2Pos, v2Size, color0, color1, color2, color3)));
}

void ETHScriptWrapper::DrawLine(const Vector2 &v2A, const Vector2 &v2B, const GS_COLOR a, const GS_COLOR b, const float width)
{
	m_primitiveList.push_back(boost::shared_ptr<ETHPrimitiveDrawer>(new ETHLineDrawer(m_provider, v2A, v2B, a, b, width)));
}

/*void ETHScriptWrapper::PlayCutscene(const str_type::string &fileName)
{
	str_type::string path = m_provider->GetResourcePath();
	path += fileName;
	GS_PLAYER_INFO info = ::PlayCutscene(m_provider->GetVideo(), path, m_provider->GetInput(), GSK_ESC);
    if (info == GSPI_CLOSE_WINDOW)
		m_provider->GetVideo()->Quit();
}*/

bool ETHScriptWrapper::Windowed()
{
	return m_provider->GetVideo()->IsWindowed();
}

ETH_VIDEO_MODE ETHScriptWrapper::GetVideoMode(const unsigned int nMode)
{
	const Video::VIDEO_MODE gsMode = m_provider->GetVideo()->GetVideoMode(nMode);
	ETH_VIDEO_MODE mode;
	if (gsMode.height > 0)
	{
		mode.height = gsMode.height;
		mode.width  = gsMode.width;
		mode.pf = gsMode.pf;
	}
	return mode;
}

unsigned int ETHScriptWrapper::GetVideoModeCount()
{
	return m_provider->GetVideo()->GetVideoModeCount();
}

GS_DWORD ETHScriptWrapper::GetBackgroundColor()
{
	return m_provider->GetVideo()->GetBGColor();
}

void ETHScriptWrapper::SetBackgroundColor(const GS_DWORD color)
{
	m_provider->GetVideo()->SetBGColor(color);
}

Vector2 ETHScriptWrapper::GetSystemScreenSize()
{
	const Vector2i v2 = m_provider->GetVideo()->GetClientScreenSize();
	return Vector2((float)v2.x, (float)v2.y);
}

bool ETHScriptWrapper::GetEntityArrayByName(const str_type::string &name, ETHEntityArray &outVector)
{
	if (WarnIfRunsInMainFunction(GS_L("GetEntityArrayByName")))
		return false;

	return m_pScene->GetBucketManager().GetEntityArrayByName(name, outVector);
}

bool ETHScriptWrapper::GetEntityArrayFromBucket(const Vector2 &v2Bucket, ETHEntityArray &outVector)
{
	if (WarnIfRunsInMainFunction(GS_L("GetEntityArrayFromBucket")))
		return false;

	return m_pScene->GetBucketManager().GetEntityArrayFromBucket(v2Bucket, outVector);
}

void ETHScriptWrapper::SetPosition(ETHEntity *pEntity, const Vector3 &v3Pos)
{
	pEntity->SetPosition(v3Pos, m_pScene->GetBucketManager());
}

void ETHScriptWrapper::SetPositionXY(ETHEntity *pEntity, const Vector2 &v2Pos)
{
	pEntity->SetPositionXY(v2Pos, m_pScene->GetBucketManager());
}

void ETHScriptWrapper::AddToPosition(ETHEntity *pEntity, const Vector3 &v3Pos)
{
	pEntity->AddToPosition(v3Pos, m_pScene->GetBucketManager());
}

void ETHScriptWrapper::AddToPositionXY(ETHEntity *pEntity, const Vector2 &v2Pos)
{
	pEntity->AddToPositionXY(v2Pos, m_pScene->GetBucketManager());
}

Vector2 ETHScriptWrapper::GetCurrentBucket(ETHEntity *pEntity)
{
	return pEntity->GetCurrentBucket(m_pScene->GetBucketManager());
}

Vector2 ETHScriptWrapper::GetBucket(const Vector2 &v2)
{
	return ETHGlobal::GetBucket(v2, m_pScene->GetBucketSize());
}

void ETHScriptWrapper::SetBorderBucketsDrawing(const bool enable)
{
	if (WarnIfRunsInMainFunction(GS_L("SetBucketBorderDrawing")))
		return;
	m_pScene->SetBorderBucketsDrawing(enable);
}

bool ETHScriptWrapper::IsDrawingBorderBuckets()
{
	if (WarnIfRunsInMainFunction(GS_L("IsDrawingBorderBuckets")))
		return false;
	return m_pScene->IsDrawingBorderBuckets();
}

int ETHScriptWrapper::GetArgc()
{
	return m_argc;
}

str_type::string ETHScriptWrapper::GetArgv(const int n)
{
	if (n >= m_argc || n < 0)
	{
		return GS_L("");
	}
	return m_argv[n];
}

str_type::string ETHScriptWrapper::GetProgramPath()
{
	return m_provider->GetResourcePath();
}

str_type::string ETHScriptWrapper::GetAbsolutePath(const str_type::string &fileName)
{
	return GetProgramPath() + fileName;
}

void ETHScriptWrapper::SetPositionRoundUp(const bool roundUp)
{
	m_roundUpPosition = roundUp;
}

bool ETHScriptWrapper::GetPositionRoundUp()
{
	return m_roundUpPosition;
}

Vector2 ETHScriptWrapper::GetWorldSpaceCursorPos2()
{
	return Vector2(m_provider->GetInput()->GetCursorPositionF(m_provider->GetVideo())+m_provider->GetVideo()->GetCameraPos());
}

Vector3 ETHScriptWrapper::GetWorldSpaceCursorPos3()
{
	return Vector3(m_provider->GetInput()->GetCursorPositionF(m_provider->GetVideo())+m_provider->GetVideo()->GetCameraPos(), 0);
}

void ETHScriptWrapper::SetHaloRotation(const bool enable)
{
	if (WarnIfRunsInMainFunction(GS_L("SetHaloRotation")))
		return;
	m_pScene->SetHaloRotation(enable);
}

void ETHScriptWrapper::EnableQuitKeys(const bool enable)
{
	m_provider->GetVideo()->EnableQuitShortcuts(enable);
}

void ETHScriptWrapper::EnableRealTimeShadows(const bool enable)
{
	if (WarnIfRunsInMainFunction(GS_L("EnableRealTimeShadows")))
		return;
	m_pScene->EnableRealTimeShadows(enable);
}

void ETHScriptWrapper::GetVisibleEntities(ETHEntityArray &entityArray)
{
	if (WarnIfRunsInMainFunction(GS_L("GetVisibleEntities")))
		return;
	m_pScene->GetBucketManager().GetVisibleEntities(entityArray);
}

void ETHScriptWrapper::GetIntersectingEntities(const Vector2 &v2Here, ETHEntityArray &outVector, const bool screenSpace)
{
	if (WarnIfRunsInMainFunction(GS_L("GetIntersectingEntities")))
		return;
	m_pScene->GetIntersectingEntities(v2Here, outVector, screenSpace);
}

int ETHScriptWrapper::GetNumRenderedEntities()
{
	if (WarnIfRunsInMainFunction(GS_L("GetNumRenderedEntities")))
		return 0;
	return m_pScene->GetNumRenderedEntities();
}

bool ETHScriptWrapper::SaveScene(const str_type::string &escFile)
{
	if (WarnIfRunsInMainFunction(GS_L("SaveScene")))
		return false;

	// loads a new scene from file
	str_type::string fileName = m_provider->GetResourcePath();
	fileName += escFile;

	return m_pScene->SaveToFile(fileName);
}

bool ETHScriptWrapper::LoadScene(const str_type::string &escFile, const Vector2 &v2BucketSize)
{
	m_provider->GetAudioResourceManager()->ReleaseResources();
	m_provider->GetGraphicResourceManager()->ReleaseResources();

	// if the name is set to _ETH_EMPTY_SCENE_STRING, don't load anything
	str_type::string fileName = m_provider->GetResourcePath();
	fileName += escFile;
	if (escFile != _ETH_EMPTY_SCENE_STRING && escFile.size() > 0)
	{
		m_pScene = ETHScenePtr(new ETHScene(fileName, m_provider, ETHSceneProperties(), m_pASModule, m_pScriptContext, v2BucketSize));
		if (!m_pScene->GetNumEntities())
		{
			ShowMessage(GS_L("Couldn't load the scene"), ETH_ERROR);
			return false;
		}
	}
	else
	{
		m_pScene = ETHScenePtr(new ETHScene(m_provider, ETHSceneProperties(), m_pASModule, m_pScriptContext, v2BucketSize));
	}

	m_sceneFileName = escFile;
	m_pScene->EnableLightmaps(m_useLightmaps);
	GenerateLightmaps();
	m_provider->GetVideo()->SetCameraPos(Vector2(0,0));
	LoadSceneScripts();
	m_timer.CalcLastFrame();
	return true;
}

void ETHScriptWrapper::LoadSceneInScript(const str_type::string &escFile)
{
	m_nextScene.SetNextScene((escFile == GS_L("")) ? _ETH_EMPTY_SCENE_STRING : escFile, GS_L(""), GS_L(""), Vector2(_ETH_DEFAULT_BUCKET_SIZE,_ETH_DEFAULT_BUCKET_SIZE));
}

void ETHScriptWrapper::LoadSceneInScript(const str_type::string &escFile, const str_type::string &preLoopFunc, const str_type::string &loopFunc)
{
	m_nextScene.SetNextScene((escFile == GS_L("")) ? _ETH_EMPTY_SCENE_STRING : escFile, preLoopFunc, loopFunc, Vector2(_ETH_DEFAULT_BUCKET_SIZE,_ETH_DEFAULT_BUCKET_SIZE));
}

void ETHScriptWrapper::LoadSceneInScript(const str_type::string &escFile, const str_type::string &preLoopFunc, const str_type::string &loopFunc, const Vector2 &v2BucketSize)
{
	m_nextScene.SetNextScene((escFile == GS_L("")) ? _ETH_EMPTY_SCENE_STRING : escFile, preLoopFunc, loopFunc, v2BucketSize);
}

str_type::string ETHScriptWrapper::GetSceneFileName()
{
	if (WarnIfRunsInMainFunction(GS_L("GetSceneFileName")))
		return false;

	return m_sceneFileName;
}

ETHInput *ETHScriptWrapper::GetInputHandle()
{
	return &m_ethInput;
}

void ETHScriptWrapper::SetWindowProperties(const str_type::string &winTitle, const unsigned int width, const unsigned int height,
									const bool windowed, const bool sync, const GS_PIXEL_FORMAT gsPF)
{
	GS2D_UNUSED_ARGUMENT(sync);
	if (m_provider->GetVideo())
	{
		const bool toggle = (m_provider->GetVideo()->IsWindowed() != windowed) ? true : false;
		m_provider->GetVideo()->ResetVideoMode(width, height, gsPF, toggle);
		m_provider->GetVideo()->SetWindowTitle(winTitle);
		if (windowed)
		{
			Vector2i v2Backbuffer(static_cast<int>(width), static_cast<int>(height));
			Vector2i v2Screen = m_provider->GetVideo()->GetClientScreenSize();
			m_provider->GetVideo()->SetWindowPosition(v2Screen/2-v2Backbuffer/2);
		}
	}
}

void ETHScriptWrapper::UsePixelShaders(const bool enable)
{
	if (!m_provider->GetShaderManager()->IsPixelLightingSupported() && enable)
	{
		ETH_STREAM_DECL(ss) << GS_L("Scripting error: pixel shaders are no supported.");
		m_provider->Log(ss.str(), Platform::FileLogger::WARNING);
		return;
	}
	m_provider->GetShaderManager()->UsePS(enable);
}

bool ETHScriptWrapper::IsPixelShaderSupported()
{
	return m_provider->GetShaderManager()->IsPixelLightingSupported();
}

Vector2 ETHScriptWrapper::GetLastCameraPos()
{
	return m_v2LastCamPos;
}

void ETHScriptWrapper::LoadSceneScripts()
{
	// loads and executes the pre loop script
	if (m_nextScene.GetPreLoopFunc() != GS_L(""))
	{
		const int funcId = CScriptBuilder::GetFunctionIdByName(m_pASModule, m_nextScene.GetPreLoopFunc());
		if (funcId>=0)
		{
			// Executes the scene start script
			ETHGlobal::ExecuteContext(m_pScriptContext, funcId);
		}
		else
		{
			ETHGlobal::CheckFunctionSeekError(funcId, m_nextScene.GetPreLoopFunc());
		}
	}

	// loads the loop script
	if (m_nextScene.GetLoopFunc() != GS_L(""))
	{
		m_loopFunctionId = CScriptBuilder::GetFunctionIdByName(m_pASModule, m_nextScene.GetLoopFunc());
		if (m_loopFunctionId<0)
		{
			ETHGlobal::CheckFunctionSeekError(m_loopFunctionId, m_nextScene.GetLoopFunc());
			m_loopFunctionId =-1;
		}
	}
}

ETHResourceProviderPtr ETHScriptWrapper::GetProvider()
{
	return m_provider;
}