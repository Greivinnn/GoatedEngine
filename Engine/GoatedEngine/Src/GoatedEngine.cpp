#include "Precompiled.h"
#include "GoatedEngine.h"

GoatedEngine::App& GoatedEngine::MainApp()
{
	static App sApp;
	return sApp;
}