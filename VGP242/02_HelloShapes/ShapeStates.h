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
	virtual void CreateShapes();

	using Vertices = std::vector<GoatedEngine::Graphics::VertexPC>;
	Vertices mVertices;

	GoatedEngine::Graphics::MeshBuffer mMeshBuffer;
	GoatedEngine::Graphics::VertexShader mVertexShader;
	GoatedEngine::Graphics::PixelShader mPixelShader;

};

class QuadState : public ShapeStates
{
public:
	void Update(float deltaTime);
protected:
	void CreateShapes() override;
};