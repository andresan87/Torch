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

#include "ETHEngine.h"
#include "ETHScriptObjRegister.h"
#include "ETHBinaryStream.h"
#include "ETHCommon.h"

#ifdef GS2D_STR_TYPE_WCHAR
 #include "../addons/utf16/scriptbuilder.h"
#else
 #include "../addons/ansi/scriptbuilder.h"
#endif

using namespace gs2d;
using namespace gs2d::math;

asIScriptEngine *ETHEngine::m_pASEngine = 0;

ETHEngine::ETHEngine(const bool testing, const bool compileAndRun, const str_type::string& startResourcePath) :
	ETH_DEFAULT_MAIN_SCRIPT_FILE(_ETH_DEFAULT_MAIN_SCRIPT_FILE),
	ETH_DEFAULT_MAIN_BYTECODE_FILE(GS_L("game.bin")),
	ETH_SCRIPT_MODULE(GS_L("EthanonModule")),
	ETH_MAIN_FUNCTION(GS_L("main")),
	m_testing(testing),
	m_compileAndRun(compileAndRun),
	m_startResourcePath(startResourcePath)
{
}

ETHEngine::~ETHEngine()
{
	if (m_pScriptContext)
	{
		m_pScriptContext->Release();
		m_pScriptContext = 0;
	}
	m_pASEngine->Release();
	m_pASEngine = 0;
}

ETHResourceProviderPtr ETHEngine::GetProvider()
{
	return m_provider;
}

void ETHEngine::Start(VideoPtr video, InputPtr input, AudioPtr audio)
{
	m_provider = ETHResourceProviderPtr(new ETHResourceProvider(
		ETHGraphicResourceManagerPtr(new ETHGraphicResourceManager()),
		ETHAudioResourceManagerPtr(new ETHAudioResourceManager()),
		ETHShaderManagerPtr(new ETHShaderManager(video, m_startResourcePath + ETHDirectories::GetShaderPath())),
		m_startResourcePath, video, audio, input));

	if (!m_pASEngine)
	{
		video->SetBGColor(GS_BLACK);

		if (!PrepareScriptingEngine())
		{
			Abort();
			return;
		}

		if (m_compileAndRun)
		{
			if (!RunMainFunction(GetMainFunctionId()))
			{
				Abort();
				return;
			}
			m_ethInput.SetProvider(m_provider);
			video->EnableQuitShortcuts(true);
			m_v2LastCamPos = video->GetCameraPos();
		}
	}
	else
	{
		m_pScene->RecoverResources();
	}
}

Application::APP_STATUS ETHEngine::Update(unsigned long lastFrameDeltaTimeMS)
{
	// run garbage collector
	m_pASEngine->GarbageCollect(asGC_ONE_STEP);

	m_lastFrameDeltaTimeMS = lastFrameDeltaTimeMS;
	//update timer
	m_timer.CalcLastFrame();

	RunLoopFunction();

	if (!LoadNextSceneIfRequested())
		Abort();

	if (Aborted())
		return Application::APP_QUIT;
	else
		return Application::APP_OK;
}

bool ETHEngine::LoadNextSceneIfRequested()
{
	if (m_nextScene.HasNextScene())
	{
		LoadScene(m_nextScene.GetSceneName(), m_nextScene.GetBucketSize());
		m_nextScene.Reset();
	}
	else
	{
		if (!m_pScene)
		{
			ShowMessage(GS_L("ETHEngine::StartEngine: the script has set no scene to load."), ETH_ERROR);
			return false;
		}
	}
	return true;
}

void ETHEngine::RenderFrame()
{
	const VideoPtr& video = m_provider->GetVideo();
	video->BeginSpriteScene();

	// draw scene (if there's any)
	m_pScene->RenderScene(IsRoundingUpPosition());
	m_v2LastCamPos = GetCameraPos();

	// draw sprites, rects, lines and texts
	DrawTopLayer();

	video->EndSpriteScene();

	// run all callbacks from the list after the rendering is finished
	m_pScene->RunCallbacksFromList();
}

bool ETHEngine::RunLoopFunction() const
{
	if (m_loopFunctionId >= 0)
	{
		ETHGlobal::ExecuteContext(m_pScriptContext, m_loopFunctionId);
		return true;
	}
	else
	{
		return false;
	}
}

void ETHEngine::Destroy()
{
	m_provider->GetGraphicResourceManager()->ReleaseResources();
	m_provider->GetAudioResourceManager()->ReleaseResources();
}

bool ETHEngine::PrepareScriptingEngine()
{
	m_pASEngine = asCreateScriptEngine(ANGELSCRIPT_VERSION);
	if (!m_pASEngine)
	{
		ShowMessage(GS_L("Failed to create AngelScript engine."), ETH_ERROR);
		return false;
	}

	// Set UTF-8 encoding
	int r = m_pASEngine->SetEngineProperty(asEP_SCRIPT_SCANNER, 1);
	if (!CheckAngelScriptError(r, GS_L("Failed setting up script scanner.")))
		return false;

	#ifdef GS2D_STR_TYPE_WCHAR
	r = m_pASEngine->SetEngineProperty(asEP_STRING_ENCODING, 1);
	// #else
	// r = m_pASEngine->SetEngineProperty(asEP_STRING_ENCODING, 0);
	#endif

	if (!CheckAngelScriptError(r, GS_L("Failed while setting up string encoding")))
		return false;

	// Message callback
	r = m_pASEngine->SetMessageCallback(asFUNCTION(MessageCallback), 0, asCALL_CDECL);
	if (!CheckAngelScriptError(r, GS_L("Failed while setting message callback.")))
		return false;

	ETHGlobal::RegisterEnumTypes(m_pASEngine);
	ETHGlobal::RegisterGlobalProperties(m_pASEngine);
	ETHGlobal::RegisterAllObjects(m_pASEngine);
	RegisterGlobalFunctions(m_pASEngine);
	m_pScriptContext = m_pASEngine->CreateContext();

	if (!BuildModule())
		return false;

	return true;
}

bool ETHEngine::BuildModule()
{
	const str_type::string resourcePath = m_provider->GetResourcePath();
	const str_type::string mainScript = resourcePath + ETH_DEFAULT_MAIN_SCRIPT_FILE;
	const str_type::string byteCodeFile = resourcePath + ETH_DEFAULT_MAIN_BYTECODE_FILE;

	// if there's a main script file, load the source from text code and compile it
	if (ETHGlobal::FileExists(mainScript, m_provider->GetVideo()->GetFileManager()))
	{
		ETH_STREAM_DECL(ssi) << GS_L("Loading game script from source-code: ") << ETH_DEFAULT_MAIN_SCRIPT_FILE;
		m_provider->Log(ssi.str(), Platform::Logger::INFO);

		// Load the main script
		CScriptBuilder builder(m_provider);

		#ifdef _DEBUG
			builder.DefineWord("DEBUG");
		#endif
		#ifdef ANDROID
			builder.DefineWord("ANDROID");
		#endif

		if (m_testing)
		{
			builder.DefineWord("TESTING");
		}

		int r;
		r = builder.StartNewModule(m_pASEngine, ETH_SCRIPT_MODULE.c_str());
		if (!CheckAngelScriptError(r, GS_L("Failed while starting the new module.")))
			return false;

		r = builder.AddSectionFromFile(mainScript.c_str());
		str_type::stringstream ss;
		ss << GS_L("Failed while loading the main script. Verify the ") << mainScript << GS_L(" file");
		if (!CheckAngelScriptError(r, ss.str()))
			return false;

		// builds the module
		r = builder.BuildModule();
		if (!CheckAngelScriptError(r, GS_L("Failed while building module.")))
			return false;

		// Gets the recently built module
		m_pASModule = CScriptBuilder::GetModule(m_pASEngine, ETH_SCRIPT_MODULE);

		// Writes the compiled byte code to file
		ETHBinaryStream stream;
		if (stream.OpenW(byteCodeFile))
		{
			m_pASModule->SaveByteCode(&stream);
			stream.CloseW();

			SaveByteCodeReadme(resourcePath);
		}
		else
		{
			ETH_STREAM_DECL(ss) << GS_L("Failed while writing the byte code file ") << byteCodeFile;
			m_provider->Log(ss.str(), Platform::Logger::ERROR);
			return false;
		}
	}
	else // otherwiser, try to load the bytecode
	{
		ETH_STREAM_DECL(ss) << GS_L("Loading game script from pre-compiled byte code: ") << ETH_DEFAULT_MAIN_BYTECODE_FILE;
		m_provider->Log(ss.str(), Platform::Logger::INFO);
	
		m_pASModule = CScriptBuilder::GetModule(m_pASEngine, ETH_SCRIPT_MODULE, asGM_ALWAYS_CREATE);
		ETHBinaryStream stream;
		if (stream.OpenR(byteCodeFile))
		{
			if (m_pASModule->LoadByteCode(&stream) < 0)
			{
				ETH_STREAM_DECL(ss) << GS_L("Couldn't load game script from pre-compiled byte code: ") << ETH_DEFAULT_MAIN_BYTECODE_FILE;
				m_provider->Log(ss.str(), Platform::Logger::ERROR);
				stream.CloseR();
				return false;
			}
			stream.CloseR();
		}
		else
		{
			ETH_STREAM_DECL(ss) << GS_L("Failed while reading the byte code file ") << byteCodeFile;
			m_provider->Log(ss.str(), Platform::Logger::ERROR);
			Abort();
			return false;
		}
	}

	return true;
}

int ETHEngine::GetMainFunctionId() const
{
	// finds the main function
	const int mainFuncId = CScriptBuilder::GetFunctionIdByName(m_pASModule, ETH_MAIN_FUNCTION);
	ETH_STREAM_DECL(ss) << GS_L("Function not found: ") << ETH_MAIN_FUNCTION;
	CheckAngelScriptError(mainFuncId, ss.str());
	return mainFuncId;
}

bool ETHEngine::CheckAngelScriptError(const int r, const str_type::string &description)
{
	if (r < 0)
	{
		ShowMessage(description, ETH_ERROR);
		return false;
	}
	else
	{
		return true;
	}
}

void ETHEngine::DrawTopLayer()
{
	const VideoPtr& video = m_provider->GetVideo();
	// draw sprites, rectangles lines and texts
	const bool zBuffer = video->GetZBuffer();
	const bool zWrite  = video->GetZWrite();
	const Vector2 v2OldCam = video->GetCameraPos();
	video->SetCameraPos(Vector2(0,0));
	video->SetZBuffer(true);
	video->SetZWrite(true);
	video->SetSpriteDepth(0.0f);
	for (std::list<boost::shared_ptr<ETHPrimitiveDrawer>>::iterator iter = m_primitiveList.begin();
		iter != m_primitiveList.end();)
	{
		(*iter)->Draw(video->GetElapsedTime());
		if ((*iter)->IsAlive(video->GetElapsedTime()))
		{
			iter++;
		}
		else
		{
			iter = m_primitiveList.erase(iter);
		}
	}
	video->SetCameraPos(v2OldCam);
	//m_primitiveList.clear();
	video->SetZBuffer(zBuffer);
	video->SetZWrite(zWrite);
}

void ETHEngine::MessageCallback(const asSMessageInfo *msg)
{
	str_type::string typeStr = GS_L("");
	Platform::Logger::TYPE type;
	if (msg->type == asMSGTYPE_WARNING)
	{
		type = Platform::Logger::WARNING;
		typeStr = GS_L("WARNING");
	}
	else if (msg->type == asMSGTYPE_INFORMATION) 
	{
		type = Platform::Logger::INFO;
		typeStr = GS_L("INFO");
	}
	else
	{
		type = Platform::Logger::ERROR;
		typeStr = GS_L("ERROR");
	}

	ETH_STREAM_DECL(ss) << std::endl << GS_L("AngelScript ") << typeStr << GS_L(" (line ") << msg->row << GS_L("): ") << std::endl << msg->message;
	m_provider->Log(ss.str(), type);
}

void ETHEngine::SaveByteCodeReadme(str_type::string path)
{
	path += ETH_DEFAULT_MAIN_BYTECODE_FILE;
	path += GS_L(".readme.txt");
	str_type::ofstream ofs;
	ofs.open(path.c_str());
	if (ofs.is_open())
	{
		str_type::stringstream ss;
		ss << ETH_DEFAULT_MAIN_BYTECODE_FILE << GS_L(" is the current pre-compiled byte code of your project. \n")
			<< GS_L("Remove ") << ETH_DEFAULT_MAIN_SCRIPT_FILE << GS_L(" from your project's path in order to load the pre-compiled byte code instead ")
			<< GS_L("of compiling ") << ETH_DEFAULT_MAIN_SCRIPT_FILE << GS_L(".");
		ofs.write(ss.str().c_str(), ss.str().length());
		ofs.close();
	}
}