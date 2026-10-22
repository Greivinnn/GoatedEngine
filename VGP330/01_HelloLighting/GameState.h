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
	GoatedEngine::Graphics::DirectionalLight mDirectionalLight;
	GoatedEngine::Graphics::StandardEffect mStandardEffect;
	GoatedEngine::Graphics::StandardEffect mStandardEffectCel;
	GoatedEngine::Graphics::RenderTarget mRenderTarget;

	GoatedEngine::Graphics::RenderObject mRenderObject;

	// added in assignment 1 
	GoatedEngine::Graphics::RenderObject mRenderBall;
	GoatedEngine::Graphics::RenderObject mRenderPlane;


	bool mUseCellShading = false;
};