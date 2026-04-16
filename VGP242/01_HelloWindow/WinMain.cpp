#include <GoatedEngine/Inc/GoatedEngine.h>

int WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
	AppConfig config;
	config.appName = L"Hello Window";

	App& mainApp = MainApp();
	mainApp.Run(config);
	return 0;
}