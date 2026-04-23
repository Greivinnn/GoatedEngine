#include "Precompiled.h"
#include "App.h"
#include "AppState.h"

using namespace GoatedEngine;
using namespace GoatedEngine::Core;
using namespace GoatedEngine::Input;	

void App::Run(const AppConfig& config)
{
	// start application
	LOG("App Started");

	//for all systems we build, initialize all singletons
	Window myWindow;
	myWindow.Initialize(GetModuleHandle(nullptr), config.appName, config.winWidth, config.winHeight);

	auto handle = myWindow.GetWindowHandle();
	InputSystem::StaticInitialize(handle);

	// after initializing singletons, initialize current state
	ASSERT(mCurrentState != nullptr, "App: No state added to the app");
	mCurrentState->Initialize();	
	InputSystem* input = InputSystem::Get();
	// run the application
	mRunning = true;
	while(mRunning)
	{
		input->Update();
		myWindow.ProcessMessage();
		if (!myWindow.IsActive() || input->IsKeyPressed(KeyCode::ESCAPE))
		{
			Quit();
			continue;
		}
		if(mNextState != nullptr)
		{
			mCurrentState->Terminate();
			mCurrentState = std::exchange(mNextState, nullptr);
			mCurrentState->Initialize();
		}

		float deltaTime = TimeUtil::GetDeltaTime();
#if defined(_DEBUG)
		if(deltaTime > 0.5f)
		{
			LOG("App: Large delta time %f seconds", deltaTime);
			continue;
		}
#endif
		mCurrentState->Update(deltaTime);

		// render flow 
	}
	// terminate activee state first
	mCurrentState->Terminate();
	InputSystem::StaticTerminate();
	myWindow.Terminate();
	// close the application
	LOG("App Ended");
}
void App::Quit()
{
	mRunning = false;
}

void GoatedEngine::App::ChangeState(const std::string& stateName)
{
	auto iter = mAppStates.find(stateName);
	if (iter != mAppStates.end())
	{
		mNextState = iter->second.get();
	}
	else
	{
		LOG("App: State %s not found", stateName.c_str());
	}
}
