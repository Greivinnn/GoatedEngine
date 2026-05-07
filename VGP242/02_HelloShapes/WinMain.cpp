#include <GoatedEngine/Inc/GoatedEngine.h>
#include "ShapeStates.h"

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
	GoatedEngine::AppConfig config;
	config.appName = L"Hello Shapes";

	GoatedEngine::App& myApp = GoatedEngine::MainApp();
	myApp.AddState<ShapeStates>("ShapeState");
	myApp.AddState<QuadState>("QuadState");
	myApp.Run(config);
	

	return 0;
}