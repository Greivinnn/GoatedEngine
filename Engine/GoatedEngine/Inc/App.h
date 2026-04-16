#pragma once

namespace GoatedEngine
{
	struct AppConfig
	{
		std::wstring appName = L"AppName";
		uint32_t winWidth = 1200;
		uint32_t winHeight = 720;
		bool fullscreen = false;
	};

	class App final
	{
	public:
		void Run(const AppConfig& config);
		void Quit();

	private:
		bool mRunning = false;
	};
}