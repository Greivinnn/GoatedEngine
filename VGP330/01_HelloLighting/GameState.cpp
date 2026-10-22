#include "GameState.h"

using namespace GoatedEngine;
using namespace GoatedEngine::Graphics;
using namespace GoatedEngine::Input;

void GameState::Initialize()
{
	// set up camera
	mCamera.SetPosition({ 0.0f, 1.0f, -3.0f });
	mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

	// set up lighting
	mDirectionalLight.direction = Math::Normalize({ 1.0f, -1.0f, 1.0f });
	mDirectionalLight.ambient = { 0.4f, 0.4f, 0.4f, 1.0f };
	mDirectionalLight.diffuse = { 0.7f, 0.7f, 0.7f, 1.0f };
	mDirectionalLight.specular = { 0.9f, 0.9f, 0.9f, 1.0f };

	// set up render object 
	Mesh mesh = MeshBuilder::CreateSphere(30, 30, 1.0f);
	mRenderObject.meshBuffer.Initialize(mesh);
	TextureManager* tm = TextureManager::Get();
	mRenderObject.diffuseMapId = tm->LoadTexture("earth.jpg");
	mRenderObject.specMapId = tm->LoadTexture("earth_spec.jpg");
	mRenderObject.normMapId = tm->LoadTexture("earth_normal.jpg");
	mRenderObject.bumpMapId = tm->LoadTexture("earth_bump.jpg");
	
	// set up the effect 
	std::filesystem::path shaderPath = L"../../Assets/Shaders/Standard.fx";
	mStandardEffect.Initialize(shaderPath);
	mStandardEffect.SetCamera(mCamera);
	mStandardEffect.SetDirectionalLight(mDirectionalLight);

	// set up cel effect 
	shaderPath = L"../../Assets/Shaders/CelShader.fx";
	mStandardEffectCel.Initialize(shaderPath);
	mStandardEffectCel.SetCamera(mCamera);
	mStandardEffectCel.SetDirectionalLight(mDirectionalLight);

	// initialize render target
	const uint32_t size = 512;
	mRenderTarget.Initialize(size, size, RenderTarget::Format::RGBA_U32);

	// Created in assignment 1
	// set up render ball
	Mesh meshBall = MeshBuilder::CreateSphere(30, 30, 0.5f);
	mRenderBall.meshBuffer.Initialize(meshBall);
	TextureManager* tmBall = TextureManager::Get();
	mRenderBall.diffuseMapId = tmBall->LoadTexture("wood_diffuse.jpg");
	mRenderBall.specMapId = tmBall->LoadTexture("wood_specular.jpg");
	mRenderBall.normMapId = tmBall->LoadTexture("wood_normal.jpg");
	mRenderBall.bumpMapId = tmBall->LoadTexture("wood_bump.jpg");
	mRenderBall.transform.position = { 2.0f, 0.0f, 0.0f };
	
	// set up render plane
	Mesh meshPlane = MeshBuilder::CreatePlane(3, 3, 1.0f);
	mRenderPlane.meshBuffer.Initialize(meshPlane);
	TextureManager* tmPlane = TextureManager::Get();
	mRenderPlane.diffuseMapId = tmPlane->LoadTexture("brick_diffuse.jpg");
	mRenderPlane.specMapId = tmPlane->LoadTexture("brick_specular.jpg");
	mRenderPlane.normMapId = tmPlane->LoadTexture("brick_normal.jpg");
	mRenderPlane.bumpMapId = tmPlane->LoadTexture("brick_specular.jpg");
	mRenderPlane.transform.position = { -3.0f, 0.0f, 0.0f };	
}

void GameState::Terminate()
{
	mRenderTarget.Terminate();
	mStandardEffectCel.Terminate();
	mStandardEffect.Terminate();
	mRenderObject.Terminate();
	//Created in assignment 1
	mRenderBall.Terminate();
	mRenderPlane.Terminate();
}

void GameState::Update(float deltaTime)
{
	UpdateCamera(deltaTime);
}

void GameState::Render()
{
	SimpleDraw::AddGroundPlane(20.0f, Colors::LightGray);
	SimpleDraw::Render(mCamera);

	if (mUseCellShading)
	{
		mStandardEffectCel.Begin();
			mStandardEffectCel.Render(mRenderObject);
			// Created in assingment 1
			mStandardEffectCel.Render(mRenderBall);
			mStandardEffectCel.Render(mRenderPlane);
		mStandardEffectCel.End();
	}
	else
	{
		mStandardEffect.Begin();
			mStandardEffect.Render(mRenderObject);
			// Created in assignment 1
			mStandardEffect.Render(mRenderBall);
			mStandardEffect.Render(mRenderPlane);
		mStandardEffect.End();
	}
	// render target render
	mRenderTarget.BeginRender();
		mStandardEffect.Begin();
			mStandardEffect.Render(mRenderObject);
		mStandardEffect.End();
	mRenderTarget.EndRender();
}

void GameState::DebugUI()
{
	ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::DragFloat3("Direction", &mDirectionalLight.direction.x, 0.01f))
		{
			mDirectionalLight.direction = Math::Normalize(mDirectionalLight.direction);
		}
		ImGui::ColorEdit4("Ambient##Light", &mDirectionalLight.ambient.r);
		ImGui::ColorEdit4("Diffuse##Light", &mDirectionalLight.diffuse.r);
		ImGui::ColorEdit4("Specular##Light", &mDirectionalLight.specular.r);
	}
	if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::ColorEdit4("Emissive##Material", &mRenderObject.material.emissive.r);
		ImGui::ColorEdit4("Ambient##Material", &mRenderObject.material.ambient.r);
		ImGui::ColorEdit4("Specular##Material", &mRenderObject.material.diffuse.r);
		ImGui::ColorEdit4("Shininess##Material", &mRenderObject.material.specular.r);
		ImGui::DragFloat("Shininess##Material", &mRenderObject.material.shininess, 0.01f, 0.0f, 1000.0f);
	}
	ImGui::Image(
		mRenderTarget.GetRawData(),
		{ 128, 128 },
		{ 0, 0 },
		{ 1, 1 },
		{ 1, 1, 1, 1 },
		{ 1, 1, 1, 1 });
	ImGui::Checkbox("UseCelShader", &mUseCellShading);
	if (mUseCellShading)
	{
		mStandardEffectCel.DebugUI();
	}
	else
	{
		mStandardEffect.DebugUI();
	}
	ImGui::End();
}

void GameState::UpdateCamera(float deltaTime)
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
