#pragma once

#include "ContantBuffer.h"
#include "PixelShader.h"
#include "VertexShader.h"
#include "DirectionalLight.h"
#include "Material.h"
#include "Sampler.h"


namespace GoatedEngine::Graphics
{
	class Camera;
	class RenderObject;
	class RenderGroup;
	class Texture;


	class StandardEffect final
	{
	public:
		void Initialize(const std::filesystem::path& shaderPath);
		void Terminate();

		void Begin();
		void End();

		void Render(const RenderObject& renderObject);
		void Render(const RenderGroup& renderGroup);

		void DebugUI();

		void SetDirectionalLight(const DirectionalLight& directionalLight);
		void SetCamera(const Camera& camera);
		void SetLightCamera(const Camera& lightCamera);
		void SetShadowMap(const Texture& shadowMap);
	private:
		struct TransformData
		{
			Math::Matrix4 wvp;	// world view projection matrix for ndc space
			Math::Matrix4 world;	// world location
			Math::Matrix4 lwvp;		// light world view projection (shadows)
			Math::Vector3 viewPosition;	// position of camera
			float padding = 0.0f;	// added 
		};

		struct SettingsData
		{
			int useDiffuseMap = 1;
			int useSpecMap = 1; 
			int useNormMap = 1;
			int useBumpMap = 1;
			int useShadowMap = 1;
			float bumpWeight = 0.1f;
			float depthBias = 0.00003f;
			float padding = 0.0f;
		};

		using TransformBuffer = TypedConstantBuffer<TransformData>;
		TransformBuffer mTransformBuffer;

		using LightBuffer = TypedConstantBuffer<DirectionalLight>;
		LightBuffer mLightBuffer;

		using MaterialBuffer = TypedConstantBuffer<Material>;
		MaterialBuffer mMaterialBuffer;

		using SettingsBuffer = TypedConstantBuffer<SettingsData>;
		SettingsBuffer mSettingBuffer;

		VertexShader mVertexShader;
		PixelShader mPixelShader;
		Sampler mSampler;

		const Camera* mCamera = nullptr;
		const DirectionalLight* mDirectionLight = nullptr;
		const Camera* mLightCamera = nullptr;
		const Texture* mShadowMap = nullptr;

		SettingsData mSettingsData;

	};
}

