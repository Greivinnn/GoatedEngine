#pragma once

#include <GoatedEngine/Inc/GoatedEngine.h>

class ShapeStates : public GoatedEngine::AppState
{
public:
	void Initialize() override;
	void Terminate() override;
	void Update(float deltaTime) override;
	void Render() override;

protected:
	virtual void CreateShape();

	GoatedEngine::Graphics::MeshPC mMesh;

	GoatedEngine::Graphics::MeshBuffer mMeshBuffer;
	GoatedEngine::Graphics::VertexShader mVertexShader;
	GoatedEngine::Graphics::PixelShader mPixelShader;

	GoatedEngine::Graphics::Camera mCamera;
	GoatedEngine::Graphics::ConstantBuffer mConstantBuffer;
};