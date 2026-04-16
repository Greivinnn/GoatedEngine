#include "Precompiled.h"
#include "App.h"

using namespace GoatedEngine;

void App::Run(const AppConfig& config)
{
	// start application
	LOG("App Started");

	//for all systems we build, initialize all singletons
	Window myWindow;
	myWindow.Initialize(GetModuleHandle(nullptr), config.appName, config.winWidth, config.winHeight);

	// run the application
	mRunning = true;
	while(mRunning)
	{
		myWindow.ProcessMessage();
		if (!myWindow.IsActive())
		{
			Quit();
			continue;
		}
	}

	myWindow.Terminate();
	// close the application
	LOG("App Ended");
}
void App::Quit()
{
	mRunning = false;
}