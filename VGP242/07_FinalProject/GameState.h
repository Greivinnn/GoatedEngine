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
	void UpdateCelestialBodies(float deltaTime);
	void RenderCelestialBodies();
	void RenderOrbitRings();

	// === Constant buffer layout (matches DoTexture.fx) ===
	struct TransformBuffer
	{
		GoatedEngine::Math::Matrix4 wvp;
	};

	// === Per-object data ===
	struct CelestialBody
	{
		std::string name;

		// Mesh / texture (each object owns these)
		GoatedEngine::Graphics::MeshBuffer meshBuffer;
		GoatedEngine::Graphics::TextureId textureId = 0;

		// Orbit params (the "year")
		float orbitRadius = 0.0f;
		float orbitSpeed = 0.0f;   // radians per second (base)
		float orbitAngle = 0.0f;   // current angle around parent

		// Self rotation params (the "day")
		float rotationSpeed = 0.0f; // radians per second (base)
		float rotationAngle = 0.0f;

		float scale = 1.0f;

		// World position computed each frame (used for orbit ring center & camera lock)
		GoatedEngine::Math::Vector3 worldPosition = GoatedEngine::Math::Vector3::Zero;

		// Index into mPlanets of the parent body (-1 = orbits the sun/origin)
		int parentIndex = -1;
	};

	GoatedEngine::Graphics::Camera mCamera;

	GoatedEngine::Graphics::VertexShader mVertexShader;
	GoatedEngine::Graphics::PixelShader mPixelShader;
	GoatedEngine::Graphics::ConstantBuffer mConstantBuffer;
	GoatedEngine::Graphics::Sampler mSampler;

	// Sky sphere
	GoatedEngine::Graphics::MeshBuffer mSkyMeshBuffer;
	GoatedEngine::Graphics::TextureId mSkyTextureId = 0;

	// Sun
	CelestialBody mSun;

	// 9 planets
	std::vector<CelestialBody> mPlanets;

	// Moon (orbits Earth)
	CelestialBody mMoon;

	// === UI state ===
	bool mShowOrbitRings = true;
	float mSpeedMultiplier = 1.0f;
	int mSelectedPlanetIndex = -1; // -1 = free camera, otherwise camera follows mPlanets[index]

	bool mFreeCamera = true;
};