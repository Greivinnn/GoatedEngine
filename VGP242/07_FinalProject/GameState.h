#pragma once

#include <GoatedEngine/Inc/GoatedEngine.h>

// =====================================================================
// GameState - Solar System Demo
//
// CALL ORDER (per engine lifecycle):
//   1. Initialize()  - runs once at start. Sets up camera, shared
//                       shader/sampler/constant buffer, and creates
//                       mesh + texture for every object (sky, sun,
//                       9 planets, moon).
//   2. Update()       - runs every frame BEFORE Render().
//                          -> UpdateCelestialBodies() : advances orbit
//                             and spin angles, recomputes world positions
//                          -> UpdateCamera()           : free-fly camera
//                             or locks onto + follows selected planet
//   3. Render()       - runs every frame AFTER Update().
//                          -> draws sky sphere
//                          -> RenderCelestialBodies() : draws sun,
//                             planets, moon 
//                          -> RenderOrbitRings()       : draws orbit
//                             circles with SimpleDraw (if enabled)
//   4. DebugUI()      - runs every frame, draws the ImGui control panel
//                        (toggle rings, speed slider, camera target combo)
//   5. Terminate()    - runs once at shutdown. Releases all meshes,
//                        textures, and shared GPU resources.
// =====================================================================

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

	// === Constant buffer layout ===
	struct TransformBuffer
	{
		GoatedEngine::Math::Matrix4 wvp;
	};

	// === Per-object data ===
	struct CelestialBody
	{
		std::string name;

		// Mesh / texture (each object has these)
		GoatedEngine::Graphics::MeshBuffer meshBuffer;
		GoatedEngine::Graphics::TextureId textureId = 0;

		// Orbit params ("year")
		float orbitRadius = 0.0f;
		float orbitSpeed = 0.0f;   // radians per second (base)
		float orbitAngle = 0.0f;   // current angle around parent

		// Self rotation params ("day")
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