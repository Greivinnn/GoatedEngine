#include "GameState.h"

using namespace GoatedEngine;
using namespace GoatedEngine::Graphics;
using namespace GoatedEngine::Input;
using namespace GoatedEngine::Math;

namespace
{
	constexpr const char* kShaderPath = "../../Assets/Shaders/DoTexture.fx";


	struct PlanetData
	{
		const char* name;
		const char* texture;
		float orbitRadius;
		float orbitSpeed;
		float rotationSpeed;
		float scale;
	};

	// array that holds the properties of each planet
	// this makes adding or tweaking planets easier, as we can just modify this array instead of changing code in multiple places
	const PlanetData kPlanetData[] =
	{
		{ "Mercury", "planets/mercury.jpg",  4.0f, 0.80f, 0.50f, 0.30f },
		{ "Venus",   "planets/venus.jpg",    6.0f, 0.60f, 0.30f, 0.45f },
		{ "Earth",   "planets/earth.jpg",    8.0f, 0.50f, 1.00f, 0.50f },
		{ "Mars",    "planets/mars.jpg",    10.0f, 0.40f, 0.90f, 0.40f },
		{ "Jupiter", "planets/jupiter.jpg", 14.0f, 0.22f, 2.00f, 1.40f },
		{ "Saturn",  "planets/saturn.jpg",  18.0f, 0.18f, 1.80f, 1.20f },
		{ "Uranus",  "planets/uranus.jpg",  22.0f, 0.14f, 1.20f, 0.90f },
		{ "Neptune", "planets/neptune.jpg", 26.0f, 0.10f, 1.10f, 0.85f },
		{ "Pluto",   "planets/pluto.jpg",   30.0f, 0.08f, 0.40f, 0.20f }
	};

	// index of Earth in the kPlanetData array, used for the moon's orbit
	constexpr int kEarthIndex = 2;
}

void GameState::Initialize()
{
	mCamera.SetPosition({ 0.0f, 5.0f, -20.0f });
	mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

	mVertexShader.Initialize<VertexPX>(kShaderPath);
	mPixelShader.Initialize(kShaderPath);
	mConstantBuffer.Initialize(sizeof(TransformBuffer));
	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);

	// create a sky sphere mesh using MeshBuilder 
	MeshPX skyMesh = MeshBuilder::CreateSkySpherePX(32, 16, 500.0f);
	mSkyMeshBuffer.Initialize(skyMesh);
	mSkyTextureId = TextureManager::Get()->LoadTexture("skysphere/blackSpace.jpg");
	{
		MeshPX sunMesh = MeshBuilder::CreateSpherePX(32, 16, 1.0f);
		mSun.meshBuffer.Initialize(sunMesh);
		mSun.textureId = TextureManager::Get()->LoadTexture("sun.jpg");
		mSun.name = "Sun";
		mSun.scale = 2.0f;
		mSun.rotationSpeed = 0.1f;
		// orbitRadius and orbitSpeed are set to 0 for the Sun, as it keeps it fixed at the origin
		mSun.orbitRadius = 0.0f;
		mSun.orbitSpeed = 0.0f;
	}

	// resize the vector to hold the planets in our array, so we can index eahc planet
	mPlanets.resize(std::size(kPlanetData));
	for (size_t i = 0; i < std::size(kPlanetData); ++i)
	{
		const PlanetData& data = kPlanetData[i];
		CelestialBody& body = mPlanets[i];

		// all planets share the same sphere mesh from MeshBuilder, the scale is applied per planet via world matrix
		MeshPX mesh = MeshBuilder::CreateSpherePX(32, 16, 1.0f);
		body.meshBuffer.Initialize(mesh);
		body.name = data.name;
		body.textureId = TextureManager::Get()->LoadTexture(data.texture);
		body.orbitRadius = data.orbitRadius;
		body.orbitSpeed = data.orbitSpeed;
		body.rotationSpeed = data.rotationSpeed;
		body.scale = data.scale;
		body.orbitAngle = 0.0f;
		body.rotationAngle = 0.0f;
		body.parentIndex = -1;	// -1 means no parent, only the moon has a parent (Earth), all other planets orbit the Sun (origin)
	}

	{
		// the moon gets a different mesh than the planets, as it is smaller and has a different texture
		MeshPX moonMesh = MeshBuilder::CreateSpherePX(16, 8, 1.0f);
		mMoon.meshBuffer.Initialize(moonMesh);
		mMoon.textureId = TextureManager::Get()->LoadTexture("planets/pluto.jpg"); 
		mMoon.name = "Moon";
		mMoon.orbitRadius = 1.2f;    
		mMoon.orbitSpeed = 2.5f;     // orbit around earth
		mMoon.rotationSpeed = 0.5f;
		mMoon.scale = 0.15f;
		mMoon.parentIndex = kEarthIndex;	// parent index is Earth
	}
}

// terminate everything 
void GameState::Terminate()
{
	mMoon.meshBuffer.Terminate();
	TextureManager::Get()->ReleaseTexture(mMoon.textureId);

	for (auto& planet : mPlanets)
	{
		planet.meshBuffer.Terminate();
		TextureManager::Get()->ReleaseTexture(planet.textureId);
	}

	mSun.meshBuffer.Terminate();
	TextureManager::Get()->ReleaseTexture(mSun.textureId);

	mSkyMeshBuffer.Terminate();
	TextureManager::Get()->ReleaseTexture(mSkyTextureId);

	mSampler.Terminate();
	mConstantBuffer.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
}

void GameState::Update(float deltaTime)
{
	UpdateCelestialBodies(deltaTime);
	UpdateCamera(deltaTime);
}

void GameState::UpdateCelestialBodies(float deltaTime)
{
	// mSpeedMultiplier allows the user to chande the time forward/backward via ImGui
	const float dt = deltaTime * mSpeedMultiplier;

	mSun.rotationAngle += mSun.rotationSpeed * dt;
	mSun.worldPosition = Vector3::Zero;

	for (auto& planet : mPlanets)
	{
		planet.orbitAngle += planet.orbitSpeed * dt;
		planet.rotationAngle += planet.rotationSpeed * dt;

		// cirvular orbit: convert the accumalated angle to XZ world pos
		planet.worldPosition =
		{
			std::cosf(planet.orbitAngle) * planet.orbitRadius,
			0.0f,
			std::sinf(planet.orbitAngle) * planet.orbitRadius
		};
	}

	mMoon.orbitAngle += mMoon.orbitSpeed * dt;
	mMoon.rotationAngle += mMoon.rotationSpeed * dt;

	// moon is close to earth so we add the moon local orbit on top of earth world postion 
	const Vector3& earthPos = mPlanets[mMoon.parentIndex].worldPosition;
	mMoon.worldPosition =
	{
		earthPos.x + std::cosf(mMoon.orbitAngle) * mMoon.orbitRadius,
		0.0f,
		earthPos.z + std::sinf(mMoon.orbitAngle) * mMoon.orbitRadius
	};
}

void GameState::Render()
{
	{
		mVertexShader.Bind();
		mPixelShader.Bind();
		mSampler.BindPS(0);

		Matrix4 world = Matrix4::Translation(mCamera.GetPosition());
		Matrix4 wvp = Transpose(world * mCamera.GetViewMatrix() * mCamera.GetProjectionMatrix());

		TransformBuffer cb;
		cb.wvp = wvp;
		mConstantBuffer.Update(&cb);
		mConstantBuffer.BindVS(0);

		TextureManager::Get()->BindPS(mSkyTextureId, 0);
		mSkyMeshBuffer.Render();
	}

	RenderCelestialBodies();
	RenderOrbitRings();
}

void GameState::RenderCelestialBodies()
{
	mVertexShader.Bind();
	mPixelShader.Bind();
	mSampler.BindPS(0);

	// Computes view-projection once and reuse it for every planet
	const Matrix4 viewProj = mCamera.GetViewMatrix() * mCamera.GetProjectionMatrix();

	// lambda function that capture the share state (shader, buffer, viewProj)
	// every planet only needs to supply its own scale, spin, and pos
	auto DrawBody = [&](const CelestialBody& body)
		{
			Matrix4 scale = Matrix4::Scaling(body.scale);
			Matrix4 spin = Matrix4::RotationY(body.rotationAngle);
			Matrix4 translation = Matrix4::Translation(body.worldPosition);

			Matrix4 world = scale * spin * translation;
			Matrix4 wvp = Transpose(world * viewProj);

			TransformBuffer cb;
			cb.wvp = wvp;
			mConstantBuffer.Update(&cb);
			mConstantBuffer.BindVS(0);

			TextureManager::Get()->BindPS(body.textureId, 0);
			body.meshBuffer.Render();
		};

	DrawBody(mSun);

	for (const auto& planet : mPlanets)
	{
		DrawBody(planet);
	}

	DrawBody(mMoon);
}

void GameState::RenderOrbitRings()
{
	if (!mShowOrbitRings)
	{
		return;
	}

	for (const auto& planet : mPlanets)
	{
		SimpleDraw::AddGroundCircle(64, planet.orbitRadius, Vector3::Zero, Colors::White);
	}

	SimpleDraw::AddGroundCircle(32, mMoon.orbitRadius, mPlanets[mMoon.parentIndex].worldPosition, Colors::LightGray);

	SimpleDraw::Render(mCamera);
}

void GameState::DebugUI()
{
	ImGui::Begin("Solar System", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::Checkbox("Show Orbit Rings", &mShowOrbitRings);
	ImGui::DragFloat("Speed", &mSpeedMultiplier, 0.05f, 0.0f, 20.0f);

	ImGui::Separator();

	std::vector<std::string> names;
	names.push_back("Free Camera");
	for (const auto& planet : mPlanets)
	{
		names.push_back(planet.name);
	}

	std::vector<const char*> namePtrs;
	for (const auto& n : names)
	{
		namePtrs.push_back(n.c_str());
	}

	int comboIndex = mSelectedPlanetIndex + 1;
	if (ImGui::Combo("Camera Target", &comboIndex, namePtrs.data(), (int)namePtrs.size()))
	{
		mSelectedPlanetIndex = comboIndex - 1;
		mFreeCamera = (mSelectedPlanetIndex == -1);
	}

	ImGui::End();
}

void GameState::UpdateCamera(float deltaTime)
{
	if (!mFreeCamera && mSelectedPlanetIndex >= 0 && mSelectedPlanetIndex < (int)mPlanets.size())
	{
		const CelestialBody& target = mPlanets[mSelectedPlanetIndex];

		// Offset the camera behind and above the planet, scaled by the planet's size
		// so smaller planets don't get a huge camera and larger ones don't clip
		const float distance = target.scale * 4.0f + 1.0f;
		Vector3 offset = { 0.0f, target.scale * 1.5f + 0.5f, -distance };

		mCamera.SetPosition(target.worldPosition + offset);
		mCamera.SetLookAt(target.worldPosition);
		return;
	}

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