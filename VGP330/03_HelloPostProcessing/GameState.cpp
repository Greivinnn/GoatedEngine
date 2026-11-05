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
	mDirectionalLight.specular = { 0.9f, 0.9f, 0.9f, 1.0f };

	// set up render object 
	mCharacter.Initialize(L"CharacterOne/XBot.model");
	
	Mesh groundMesh = MeshBuilder::CreatePlane(10, 10, 1.0f);
	mGround.meshBuffer.Initialize(groundMesh);
	mGround.diffuseMapId = TextureManager::Get()->LoadTexture("misc/concrete.jpg");

	// set up the effect 
	std::filesystem::path shaderPath = L"../../Assets/Shaders/Standard.fx";
	mStandardEffect.Initialize(shaderPath);
	mStandardEffect.SetCamera(mCamera);
	mStandardEffect.SetDirectionalLight(mDirectionalLight);

	// set up the post processing effect items
	shaderPath = L"../../Assets/Shaders/PostProcessing.fx";
	mPostProcessingEffect.Initialize(shaderPath);
	mPostProcessingEffect.SetTexture(&mRenderTarget, 0);
	mPostProcessingEffect.SetTexture(&mCombineTexture, 1);

	MeshPX screenQuad = MeshBuilder::CreateScreenQuadPX();
	mScreenQuad.meshBuffer.Initialize(screenQuad);

	GraphicSystem* gs = GraphicSystem::Get();
	const uint32_t screenWidth = gs->GetBackBufferWidth();
	const uint32_t screenHeight = gs->GetBackBufferHeight();
	mRenderTarget.Initialize(screenWidth, screenHeight, RenderTarget::Format::RGBA_U8);

	//mCombineTexture.Initialize(L"../../Assets/Textures/");

	// assignment additional characters
	mCharacter2.Initialize(L"CharacterTwo/FredoModel.model");
	mCharacter3.Initialize(L"CharacterThree/JPModel.model");
	mCharacter2.transform.position = { -1.5f, 0.0f, 0.0f };
	mCharacter3.transform.position = { 1.5f, 0.0f, 0.0f };


}

void GameState::Terminate()
{
	mCombineTexture.Terminate();
	mScreenQuad.Terminate();
	mRenderTarget.Terminate();
	mPostProcessingEffect.Terminate();
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
	SimpleDraw::AddGroundPlane(20.0f, Colors::LightGray);
	SimpleDraw::Render(mCamera);

	mRenderTarget.BeginRender();
		mStandardEffect.Begin();
			mStandardEffect.Render(mCharacter);
			mStandardEffect.Render(mCharacter2);
			mStandardEffect.Render(mCharacter3);
			mStandardEffect.Render(mGround);
		mStandardEffect.End();
	mRenderTarget.EndRender();

	mPostProcessingEffect.Begin();
	mPostProcessingEffect.Render(mScreenQuad);
	mPostProcessingEffect.End();
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

	ImGui::Image(
		mRenderTarget.GetRawData(),
		{ 200, 200 },
		{ 0, 0 },
		{ 1, 1 },
		{ 1, 1, 1, 1 },
		{ 1, 1, 1, 1 });

	mStandardEffect.DebugUI();
	mPostProcessingEffect.DebugUI();
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
