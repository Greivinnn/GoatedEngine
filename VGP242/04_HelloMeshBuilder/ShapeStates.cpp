#include "ShapeStates.h"

using namespace GoatedEngine;
using namespace GoatedEngine::Graphics;
using namespace GoatedEngine::Input;
void ShapeStates::Initialize()
{
	mCamera.SetPosition({ 0.0f, 1.0f, -3.0f });
	mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

	// create shape
	CreateShape();
	mMeshBuffer.Initialize(mMesh);

	std::filesystem::path shaderPath = L"../../Assets/Shaders/DoTransform.fx";
	mVertexShader.Initialize<VertexPC>(shaderPath);
	mPixelShader.Initialize(shaderPath);
	mConstantBuffer.Initialize(sizeof(Math::Matrix4));
}
void ShapeStates::Terminate()
{
	mConstantBuffer.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mMeshBuffer.Terminate();

}
void ShapeStates::Update(float deltaTime)
{
	InputSystem* input = InputSystem::Get();
	const float moveSpeed = input->IsKeyDown(KeyCode::LSHIFT) ? 10.0f : 1.0f;
	const float turnSpeed = 0.1f;

	if (input->IsKeyDown(KeyCode::W))
	{
		mCamera.Walk(moveSpeed * deltaTime);
	}
	if (input->IsKeyDown(KeyCode::S))
	{
		mCamera.Walk(-moveSpeed * deltaTime);
	}
	if (input->IsKeyDown(KeyCode::D))
	{
		mCamera.Strafe(moveSpeed * deltaTime);
	}
	if (input->IsKeyDown(KeyCode::A))
	{
		mCamera.Strafe(-moveSpeed * deltaTime);
	}
	if (input->IsKeyDown(KeyCode::E))
	{
		mCamera.Rise(moveSpeed * deltaTime);
	}
	if (input->IsKeyDown(KeyCode::Q))
	{
		mCamera.Rise(-moveSpeed * deltaTime);
	}

	if (input->IsMouseDown(MouseButton::RBUTTON))
	{
		mCamera.Yaw(input->GetMouseMoveX() * turnSpeed * deltaTime);
		mCamera.Pitch(input->GetMouseMoveY() * turnSpeed * deltaTime);
	}
}
void ShapeStates::Render()
{
	// prepare the GPU
	mVertexShader.Bind();
	mPixelShader.Bind();

	// sync buffer information
	mConstantBuffer.BindVS(0);

	// update buffer data
	Math::Matrix4 matWorld = Math::Matrix4::Identity;
	Math::Matrix4 matView = mCamera.GetViewMatrix();
	Math::Matrix4 matProj = mCamera.GetProjectionMatrix();
	Math::Matrix4 wvp = matWorld * matView * matProj;
	wvp = Math::Transpose(wvp);
	mConstantBuffer.Update(&wvp);

	// render object
	mMeshBuffer.Render();
}
void ShapeStates::CreateShape()
{
	//mMesh = MeshBuilder::CreateVertexCubePC(1.0f, Colors::Green);
	//mMesh = MeshBuilder::CreateCubePC(1.0f);
	//mMesh = MeshBuilder::CreateBoxPC(3.0f, 2.0f, 1.0f);
	//mMesh = MeshBuilder::CreatePyramidPC(1.0f);
	//mMesh = MeshBuilder::CreatePlanePC(10, 10, 1.0f, false);
	mMesh = MeshBuilder::CreateCylinderPC(20, 20);
	//mMesh = MeshBuilder::CreateSpherePC(20, 20, 1.0f);
}