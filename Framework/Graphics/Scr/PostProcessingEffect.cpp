#include "Precompiled.h"
#include "PostProcessingEffect.h"

#include "RenderObject.h"
#include "Texture.h"
#include "VertexTypes.h"

using namespace GoatedEngine;
using namespace GoatedEngine::Graphics;

namespace
{
	// names shown in the DebugUI combo box (order must match the Mode enum)
	const char* gModeNames[] =
	{
		"None",
		"Monochrome",
		"Invert",
		"Mirror",
		"Blur",
		"Combine2",
		"MotionBlur",
		"ChromaticAberration",
		"Wave"
	};
}

void PostProcessingEffect::Initialize(const std::filesystem::path& filePath)
{
	mVertexShader.Initialize<VertexPX>(filePath);
	mPixelShader.Initialize(filePath);
	mSampler.Initialize(Sampler::Filter::Point, Sampler::AddressMode::Wrap);
	mPostProcessingBuffer.Initialize();
}

void PostProcessingEffect::Terminate()
{
	mPostProcessingBuffer.Terminate();
	mSampler.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
}

void PostProcessingEffect::Begin()
{
	mVertexShader.Bind();
	mPixelShader.Bind();
	mSampler.BindPS(0);

	for (uint32_t i = 0; i < mTextures.size(); ++i)
	{
		if (mTextures[i] != nullptr)
		{
			mTextures[i]->BindPS(i);
		}
	}

	PostProcessingData data;
	data.mode = static_cast<int>(mMode);

	mPostProcessingBuffer.Update(data);
	mPostProcessingBuffer.BindPS(0);
}

void PostProcessingEffect::End()
{
	// unbind so the render target can be written to again next frame
	for (uint32_t i = 0; i < mTextures.size(); ++i)
	{
		Texture::UnbindPS(i);
	}
}

void PostProcessingEffect::Render(const RenderObject& renderObject)
{
	renderObject.meshBuffer.Render();
}

void PostProcessingEffect::SetTexture(const Texture* texture, uint32_t slot)
{
	ASSERT(slot < mTextures.size(), "PostProcessingEffect: invalid slot index");
	mTextures[slot] = texture;
}

void PostProcessingEffect::SetMode(Mode mode)
{
	mMode = mode;
}

void PostProcessingEffect::DebugUI()
{
	if (ImGui::CollapsingHeader("PostProcessingEffect", ImGuiTreeNodeFlags_DefaultOpen))
	{
		int currentMode = static_cast<int>(mMode);
		if (ImGui::Combo("Mode", &currentMode, gModeNames, static_cast<int>(std::size(gModeNames))))
		{
			mMode = static_cast<Mode>(currentMode);
		}

		if (mMode == Mode::Mirror)
		{
			ImGui::DragFloat("MirrorScaleX", &mMirrorScaleX, 0.1f, -1.0f, 1.0f);
			ImGui::DragFloat("MirrorScaleY", &mMirrorScaleY, 0.1f, -1.0f, 1.0f);
		}
		else if (mMode == Mode::Blur || mMode == Mode::MotionBlur)
		{
			ImGui::DragFloat("BlurStrength", &mBlurStrength, 1.0f, 0.0f, 100.0f);
		}
		else if (mMode == Mode::Combine2)
		{
			ImGui::DragFloat("Combine2Alpha", &mCombine2Alpha, 0.01f, 0.0f, 1.0f);
		}
		else if (mMode == Mode::ChromaticAberration)
		{
			ImGui::DragFloat("AberrationValue", &mAberrationValue, 0.001f, 0.0f, 1.0f);
		}
		else if (mMode == Mode::Wave)
		{
			ImGui::DragFloat("WaveLength", &mWaveLength, 0.001f, 0.0f, 1.0f);
			ImGui::DragFloat("NumWaves", &mNumWaves, 1.0f, 0.0f, 1000.0f);
		}
	}
}
