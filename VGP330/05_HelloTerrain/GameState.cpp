#include "GameState.h"

using namespace GoatedEngine;
using namespace GoatedEngine::Graphics;
using namespace GoatedEngine::Input;

void GameState::Initialize()
{
	// Setup camera
	mCamera.SetPosition({ 0.0f, 1.0f, -3.0f });
	mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

	// Setup directional light
	mDirectionalLight.direction = Math::Normalize({ 1.0f, -1.0f, 1.0f });
	mDirectionalLight.ambient = { 0.5f, 0.5f, 0.5f, 1.0f };
	mDirectionalLight.diffuse = { 0.8f,0.8f, 0.8f, 1.0f };
	mDirectionalLight.specular = { 0.9f, 0.9f, 0.9f, 1.0f };

	// Setup render object
	mCharacter.Initialize(L"CharacterOne/XBot.model");
	mCharacter.transform.position = { 0.0f, 0.0f, 0.0f };

	mCharacter2.Initialize(L"CharacterTwo/FredoModel.model");
	mCharacter2.transform.position = { -2.0f, 0.0f, 0.0f };

	mCharacter3.Initialize(L"CharacterThree/JPModel.model");
	mCharacter3.transform.position = { 2.0f, 0.0f, 0.0f };

	// Setup terrain
	mTerrain.Initialize(L"../../Assets/Textures/terrain/heightmap_512x512.raw", 30.0f);
	mGround.meshBuffer.Initialize(mTerrain.mesh);
	mGround.diffuseMapId = TextureManager::Get()->LoadTexture(L"terrain/dirt_seamless.jpg");	// low texture
	mGround.specMapId = TextureManager::Get()->LoadTexture(L"mountain/mountain_texture.jpg");	// high texture

	// Setup shadow effect first, standard + terrain effects read its depth map + light camera
	mShadowEffect.Initialize();
	mShadowEffect.SetDirectionalLight(mDirectionalLight);

	// Setup standard effect
	std::filesystem::path shaderPath = L"../../Assets/Shaders/Standard.fx";
	mStandardEffect.Initialize(shaderPath);
	mStandardEffect.SetCamera(mCamera);
	mStandardEffect.SetDirectionalLight(mDirectionalLight);
	mStandardEffect.SetLightCamera(mShadowEffect.GetLightCamera());
	mStandardEffect.SetShadowMap(mShadowEffect.GetDepthMap());

	// Setup terrain effect
	mTerrainEffect.Initialize();
	mTerrainEffect.SetCamera(mCamera);
	mTerrainEffect.SetLightCamera(mShadowEffect.GetLightCamera());
	mTerrainEffect.SetDirectionalLight(mDirectionalLight);
	mTerrainEffect.SetShadowMap(mShadowEffect.GetDepthMap());

	// move everything to the center of the map, on top of the terrain
	Math::Vector3 centerOfMap = Math::Vector3::Zero;
	centerOfMap.x = static_cast<float>(mTerrain.columns) * 0.5f;
	centerOfMap.z = static_cast<float>(mTerrain.rows) * 0.5f;
	centerOfMap.y = mTerrain.GetHeight(centerOfMap);

	Math::Vector3 cameraPosition = centerOfMap + mCamera.GetPosition();
	cameraPosition.y = mTerrain.GetHeight(cameraPosition) + 2.0f;
	mCamera.SetPosition(cameraPosition);

	mCharacter.transform.position += centerOfMap;
	mCharacter.transform.position.y = mTerrain.GetHeight(mCharacter.transform.position);
	mCharacter2.transform.position += centerOfMap;
	mCharacter2.transform.position.y = mTerrain.GetHeight(mCharacter2.transform.position);
	mCharacter3.transform.position += centerOfMap;
	mCharacter3.transform.position.y = mTerrain.GetHeight(mCharacter3.transform.position);

	mShadowEffect.SetFocus(centerOfMap);
}

void GameState::Terminate()
{
	mTerrainEffect.Terminate();
	mShadowEffect.Terminate();
	mGround.Terminate();
	mCharacter3.Terminate();
	mCharacter2.Terminate();
	mCharacter.Terminate();
	mStandardEffect.Terminate();
}

void GameState::Update(float deltaTime)
{
	UpdateCamera(deltaTime);
}

void GameState::Render()
{
	// render the depth map from the light
	mShadowEffect.Begin();
		mShadowEffect.Render(mCharacter);
		mShadowEffect.Render(mCharacter2);
		mShadowEffect.Render(mCharacter3);
		mShadowEffect.Render(mGround);
	mShadowEffect.End();

	// render the characters
	mStandardEffect.Begin();
		mStandardEffect.Render(mCharacter);
		mStandardEffect.Render(mCharacter2);
		mStandardEffect.Render(mCharacter3);
	mStandardEffect.End();

	// render the terrain
	mTerrainEffect.Begin();
		mTerrainEffect.Render(mGround);
	mTerrainEffect.End();
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

	mStandardEffect.DebugUI();
	mShadowEffect.DebugUI();
	mTerrainEffect.DebugUI();
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

	// keep the camera walking on the terrain
	float height = mTerrain.GetHeight(mCamera.GetPosition());
	if (height >= 0.0f)
	{
		Math::Vector3 pos = mCamera.GetPosition();
		pos.y = height + 2.0f;
		mCamera.SetPosition(pos);
		mShadowEffect.SetFocus(pos);
	}
}
