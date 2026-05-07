#include "ShapeStates.h"

using namespace GoatedEngine;
using namespace GoatedEngine::Graphics;
using namespace GoatedEngine::Input;

void ShapeStates::Initialize()
{
	CreateShapes();
	
	mMeshBuffer.Initialize(mVertices.data(), sizeof(VertexPC), mVertices.size());

	std::filesystem::path shaderPath = L"../../Assets/Shaders/DoColor.fx";
	mVertexShader.Initialize<VertexPC>(shaderPath);
	mPixelShader.Initialize(shaderPath);
}

void ShapeStates::Terminate()
{
	mVertices.clear();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mMeshBuffer.Terminate();
}

void ShapeStates::Update(float deltaTime)
{
	if (InputSystem::Get()->IsKeyPressed(KeyCode::UP))
	{
		MainApp().ChangeState("QuadState");
	}
}

void ShapeStates::Render()
{
	mVertexShader.Bind();
	mPixelShader.Bind();
	mMeshBuffer.Render();
}

void ShapeStates::CreateShapes()
{
	mVertices.push_back({ {-0.5, -0.5, 0.0,}, {GoatedEngine::Graphics::Colors::Red} });
	mVertices.push_back({ {0.5,  0.5, 0.0,}, {GoatedEngine::Graphics::Colors::Red} });
	mVertices.push_back({ {0.5, -0.5, 0.0,}, {GoatedEngine::Graphics::Colors::Red} });
}

void QuadState::Update(float deltaTime)
{
	if (InputSystem::Get()->IsKeyPressed(KeyCode::DOWN))
	{
		MainApp().ChangeState("ShapeState");
	}
}
void QuadState::CreateShapes()
{
	mVertices.push_back({ {-0.5, -0.5, 0.0,}, {GoatedEngine::Graphics::Colors::Red} });
	mVertices.push_back({ {-0.5, 0.5, 0.0,}, {GoatedEngine::Graphics::Colors::Red} });
	mVertices.push_back({ {0.5, 0.5, 0.0,}, {GoatedEngine::Graphics::Colors::Red} });

	mVertices.push_back({ {-0.5, -0.5, 0.0,}, {GoatedEngine::Graphics::Colors::Red} });
	mVertices.push_back({ {0.5, 0.5, 0.0,}, {GoatedEngine::Graphics::Colors::Red} });
	mVertices.push_back({ {0.5, -0.5, 0.0,}, {GoatedEngine::Graphics::Colors::Red} });
}