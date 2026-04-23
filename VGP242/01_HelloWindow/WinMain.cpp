#include <GoatedEngine/Inc/GoatedEngine.h>

using namespace GoatedEngine;
using namespace GoatedEngine::Core;
using namespace GoatedEngine::Input;

class MainState : public AppState
{
public:
	void Initialize() override
	{
		LOG("MainState: Initialize");
	}
	void Update(float deltaTime) override
	{
		if (InputSystem::Get()->IsKeyPressed(KeyCode::G))
		{
			MainApp().ChangeState("GameState");
		}

	}
	void Terminate() override
	{
		LOG("MainState: Terminate");
	}
private:
};

class GameState : public AppState
{
public:
	void Initialize() override
	{
		LOG("GameState: Initialize");
	}
	void Update(float deltaTime) override
	{
		if(InputSystem::Get()->IsKeyPressed(KeyCode::M))
		{
			MainApp().ChangeState("MainState");
		}
	}
	void Terminate() override
	{
		LOG("GameState: Terminate");
	}
private:
};


int WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
	AppConfig config;
	config.appName = L"Hello Window";

	App& mainApp = MainApp();
	mainApp.AddState<MainState>("MainState");
	mainApp.AddState<GameState>("GameState");
	mainApp.Run(config);
	return 0;
}