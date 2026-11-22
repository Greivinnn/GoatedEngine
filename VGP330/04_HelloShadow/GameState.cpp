#include "GameState.h"

using namespace GoatedEngine;
using namespace GoatedEngine::Graphics;
using namespace GoatedEngine::Input;

void GameState::Initialize()
{
	// set up camera
	mCamera.SetPosition({ 0.0f, 2.0f, -3.0f });
	mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

	// set up lighting
	mDirectionalLight.direction = Math::Normalize({ 1.0f, -1.0f, 1.0f });
	mDirectionalLight.ambient = { 0.4f, 0.4f, 0.4f, 1.0f };
	mDirectionalLight.diffuse = { 0.7f, 0.7f, 0.7f, 1.0f };
	mDirectionalLight.specular = { 0.5f, 0.5f, 0.5f, 1.0f };

	// set up render objects
	mCharacter.Initialize(L"CharacterOne/XBot.model");
	mCharacter2.Initialize(L"CharacterTwo/FredoModel.model");
	mCharacter3.Initialize(L"CharacterThree/JPModel.model");
	mCharacter2.transform.position = { -2.0f, 0.0f, 0.0f };
	mCharacter3.transform.position = { 2.0f, 0.0f, 0.0f };

	Mesh groundMesh = MeshBuilder::CreatePlane(10, 10, 1.0f);
	mGround.meshBuffer.Initialize(groundMesh);
	mGround.diffuseMapId = TextureManager::Get()->LoadTexture("misc/concrete.jpg");

	// spheres: one floating so it shadows the middle character, one sitting on the ground
	TextureManager* tm = TextureManager::Get();
	Mesh sphereMesh = MeshBuilder::CreateSphere(60, 60, 0.5f);
	mSphere.meshBuffer.Initialize(sphereMesh);
	mSphere.diffuseMapId = tm->LoadTexture("planets/earth/earth.jpg");
	mSphere.normMapId = tm->LoadTexture("planets/earth/earth_normal.jpg");
	mSphere.specMapId = tm->LoadTexture("planets/earth/earth_spec.jpg");
	mSphere.transform.position = { -1.0f, 2.5f, -1.0f };

	mSphere2.meshBuffer.Initialize(sphereMesh);
	mSphere2.diffuseMapId = tm->LoadTexture("misc/basketball.jpg");
	mSphere2.transform.position = { 1.0f, 0.5f, -1.5f };

	// shadow effect first, standard effect reads its depth map + light camera
	mShadowEffect.Initialize();
	mShadowEffect.SetDirectionalLight(mDirectionalLight);

	std::filesystem::path shaderPath = L"../../Assets/Shaders/Standard.fx";
	mStandardEffect.Initialize(shaderPath);
	mStandardEffect.SetCamera(mCamera);
	mStandardEffect.SetDirectionalLight(mDirectionalLight);
	mStandardEffect.SetLightCamera(mShadowEffect.GetLightCamera());
	mStandardEffect.SetShadowMap(mShadowEffect.GetDepthMap());
}

void GameState::Terminate()
{
	mSphere2.Terminate();
	mSphere.Terminate();
	mGround.Terminate();
	mCharacter3.Terminate();
	mCharacter2.Terminate();
	mCharacter.Terminate();
	mStandardEffect.Terminate();
	mShadowEffect.Terminate();
}

void GameState::Update(float deltaTime)
{
	UpdateCamera(deltaTime);
}

void GameState::Render()
{
	// pass 1: depth from the light's POV (only things that CAST shadows)
	mShadowEffect.Begin();
		mShadowEffect.Render(mCharacter);
		mShadowEffect.Render(mCharacter2);
		mShadowEffect.Render(mCharacter3);
		mShadowEffect.Render(mSphere);
		mShadowEffect.Render(mSphere2);
	mShadowEffect.End();

	// pass 2: normal render, samples the depth map to darken shadowed pixels
	mStandardEffect.Begin();
		mStandardEffect.Render(mGround);
		mStandardEffect.Render(mCharacter);
		mStandardEffect.Render(mCharacter2);
		mStandardEffect.Render(mCharacter3);
		mStandardEffect.Render(mSphere);
		mStandardEffect.Render(mSphere2);
	mStandardEffect.End();
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

	if (ImGui::CollapsingHeader("Objects", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat3("Earth##Sphere", &mSphere.transform.position.x, 0.05f);
		ImGui::DragFloat3("Ball##Sphere2", &mSphere2.transform.position.x, 0.05f);
		ImGui::DragFloat3("XBot##Char", &mCharacter.transform.position.x, 0.05f);
		ImGui::DragFloat3("Fredo##Char", &mCharacter2.transform.position.x, 0.05f);
		ImGui::DragFloat3("JP##Char", &mCharacter3.transform.position.x, 0.05f);
	}

	mStandardEffect.DebugUI();
	mShadowEffect.DebugUI();
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
