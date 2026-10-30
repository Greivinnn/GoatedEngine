#include "Precompiled.h"
#include "StandardEffect.h"
#include "Camera.h"
#include "VertexTypes.h"
#include "RenderObject.h"

using namespace GoatedEngine;
using namespace GoatedEngine::Graphics;

void StandardEffect::Initialize(const std::filesystem::path& shaderPath)
{
	// initialize buffers
	mTransformBuffer.Initialize();
	mLightBuffer.Initialize();
	mMaterialBuffer.Initialize();
	mSettingBuffer.Initialize();

	// other rendering components 
	mVertexShader.Initialize<Vertex>(shaderPath);
	mPixelShader.Initialize(shaderPath);
	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
}
void StandardEffect::Terminate()
{
	mSampler.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mSettingBuffer.Terminate();
	mMaterialBuffer.Terminate();
	mLightBuffer.Terminate();
	mTransformBuffer.Terminate();
}
void StandardEffect::Begin()
{
	mVertexShader.Bind();
	mPixelShader.Bind();
	mSampler.BindVS(0);
	mSampler.BindPS(0);

	mTransformBuffer.BindVS(0);
	mLightBuffer.BindVS(1);
	mLightBuffer.BindPS(1);
	mMaterialBuffer.BindPS(2);
	mSettingBuffer.BindVS(3);
	mSettingBuffer.BindPS(3);
}
void StandardEffect::End()
{
	// come back for shadows 
}
void StandardEffect::Render(const RenderObject& renderObject)
{
	const Math::Matrix4 matWorld = renderObject.transform.GetMatrix();
	const Math::Matrix4 matView = mCamera->GetViewMatrix();
	const Math::Matrix4 matProj = mCamera->GetProjectionMatrix();
	const Math::Matrix4 matFinal = matWorld * matView * matProj;

	TransformData data;
	data.wvp = Math::Transpose(matFinal);
	data.world = Math::Transpose(matWorld);
	data.viewPosition + mCamera->GetPosition();
	mTransformBuffer.Update(data);
	mLightBuffer.Update(*mDirectionLight);
	mMaterialBuffer.Update(renderObject.material);

	SettingsData settings;
	settings.useDiffuseMap = (mSettingsData.useDiffuseMap > 0 && renderObject.diffuseMapId > 0) ? 1 : 0;
	settings.useSpecMap = (mSettingsData.useSpecMap > 0 && renderObject.specMapId > 0) ? 1 : 0;
	settings.useNormMap = (mSettingsData.useNormMap > 0 && renderObject.normMapId > 0) ? 1 : 0;
	settings.useBumpMap = (mSettingsData.useBumpMap > 0 && renderObject.bumpMapId > 0) ? 1 : 0;
	settings.bumpWeight = mSettingsData.bumpWeight;
	mSettingBuffer.Update(settings);

	TextureManager* tm = TextureManager::Get();
	tm->BindPS(renderObject.diffuseMapId, 0);
	tm->BindPS(renderObject.specMapId, 1);
	tm->BindPS(renderObject.normMapId, 2);
	tm->BindVS(renderObject.bumpMapId, 3);

	renderObject.meshBuffer.Render();
}

void StandardEffect::Render(const RenderGroup& renderGroup)
{
	const Math::Matrix4 matWorld = renderGroup.transform.GetMatrix();
	const Math::Matrix4 matView = mCamera->GetViewMatrix();
	const Math::Matrix4 matProj = mCamera->GetProjectionMatrix();
	const Math::Matrix4 matFinal = matWorld * matView * matProj;

	TransformData data;
	data.wvp = Math::Transpose(matFinal);
	data.world = Math::Transpose(matWorld);
	data.viewPosition + mCamera->GetPosition();
	mTransformBuffer.Update(data);
	mLightBuffer.Update(*mDirectionLight);

	TextureManager* tm = TextureManager::Get();
	SettingsData settings;
	for (const RenderObject& renderObject : renderGroup.renderObjects)
	{
		mMaterialBuffer.Update(renderObject.material);
		settings.useDiffuseMap = (mSettingsData.useDiffuseMap > 0 && renderObject.diffuseMapId > 0) ? 1 : 0;
		settings.useSpecMap = (mSettingsData.useSpecMap > 0 && renderObject.specMapId > 0) ? 1 : 0;
		settings.useNormMap = (mSettingsData.useNormMap > 0 && renderObject.normMapId > 0) ? 1 : 0;
		settings.useBumpMap = (mSettingsData.useBumpMap > 0 && renderObject.bumpMapId > 0) ? 1 : 0;
		settings.bumpWeight = mSettingsData.bumpWeight;
		mSettingBuffer.Update(settings);

		tm->BindPS(renderObject.diffuseMapId, 0);
		tm->BindPS(renderObject.specMapId, 1);
		tm->BindPS(renderObject.normMapId, 2);
		tm->BindVS(renderObject.bumpMapId, 3);

		renderObject.meshBuffer.Render();
	}
}

void StandardEffect::DebugUI()
{
	if (ImGui::CollapsingHeader("StandardEffect", ImGuiTreeNodeFlags_DefaultOpen))
	{
		bool useDiffuse = mSettingsData.useDiffuseMap > 0;
		if (ImGui::Checkbox("UseDiffuse", &useDiffuse))
		{
			mSettingsData.useDiffuseMap = useDiffuse ? 1 : 0;
		}
		bool useSpec = mSettingsData.useSpecMap > 0;
		if (ImGui::Checkbox("UseSpec", &useSpec))
		{
			mSettingsData.useSpecMap = useSpec ? 1 : 0;
		}
		bool useNorm = mSettingsData.useNormMap > 0;
		if (ImGui::Checkbox("UseNorm", &useNorm))
		{
			mSettingsData.useNormMap = useNorm ? 1 : 0;
		}
		bool useBump = mSettingsData.useBumpMap > 0;
		if (ImGui::Checkbox("UseBump", &useBump))
		{
			mSettingsData.useBumpMap = useBump ? 1 : 0;
		}
		ImGui::DragFloat("BumpWeight", &mSettingsData.bumpWeight, 0.01f, 0.0f, 100.0f);
	}
}
void StandardEffect::SetDirectionalLight(const DirectionalLight& directionalLight)
{
	mDirectionLight = &directionalLight;
}
void StandardEffect::SetCamera(const Camera& camera)
{
	mCamera = &camera;
}