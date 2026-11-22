#pragma once

#include "ContantBuffer.h"
#include "PixelShader.h"
#include "VertexShader.h"
#include "Camera.h"
#include "RenderTarget.h"

namespace GoatedEngine::Graphics
{
	class RenderObject;
	class RenderGroup;
	struct DirectionalLight;

	// Renders the scene from the light's point of view into a depth map.
	// StandardEffect then samples that depth map to decide what is in shadow.
	class ShadowEffect final
	{
	public:
		void Initialize();
		void Terminate();

		void Begin();
		void End();

		void Render(const RenderObject& renderObject);
		void Render(const RenderGroup& renderGroup);

		void DebugUI();

		void SetDirectionalLight(const DirectionalLight& directionalLight);
		void SetFocus(const Math::Vector3& focusPoint);
		void SetSize(float size);

		const Camera& GetLightCamera() const;
		const Texture& GetDepthMap() const;

	private:
		void UpdateLightCamera();

		struct TransformData
		{
			Math::Matrix4 wvp;
		};
		using TransformBuffer = TypedConstantBuffer<TransformData>;
		TransformBuffer mTransformBuffer;

		VertexShader mVertexShader;
		PixelShader mPixelShader;

		Camera mLightCamera;
		RenderTarget mDepthMapRenderTarget;

		const DirectionalLight* mDirectionalLight = nullptr;
		Math::Vector3 mFocusPoint = Math::Vector3::Zero;
		float mSize = 10.0f;	// width/height of the ortho box the light covers
	};
}
