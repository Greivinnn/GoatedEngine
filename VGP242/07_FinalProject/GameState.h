#pragma once

#include <GoatedEngine/Inc/GoatedEngine.h>	

class GameState : public GoatedEngine::AppState
{
public:
	void Initialize() override;
	void Terminate() override;
	void Update(float deltaTime) override;
	void Render() override;
	void DebugUI() override;

private:
	void UpdateCamera(float deltaTime);
	GoatedEngine::Graphics::Camera mCamera;
};