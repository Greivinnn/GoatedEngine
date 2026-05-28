#include "Precompiled.h"
#include "Texture.h"
#include "GraphicSystem.h"
#include <DirectXTK/Inc/WICTextureLoader.h>

using namespace GoatedEngine;
using namespace GoatedEngine::Graphics;

void Texture::UnbindPS(uint32_t slot)
{
	static ID3D11ShaderResourceView* dummy = nullptr;
	GraphicSystem::Get()->GetContext()->PSSetShaderResources(slot, 1, &dummy);
}

Texture::~Texture()
{
	ASSERT(mShaderResourceView == nullptr, "Texture terminate must be called");
}

Texture::Texture(Texture&& rhs) noexcept
	:mShaderResourceView(rhs.mShaderResourceView)
{
	rhs.mShaderResourceView = nullptr;
}
Texture& Texture::operator=(Texture&& rhs) noexcept
{
	mShaderResourceView = rhs.mShaderResourceView;
	rhs.mShaderResourceView = nullptr;
	return *this;
}

void Texture::Initialize(const std::filesystem::path& fileName)
{
	auto device = GraphicSystem::Get()->GetDevice();
	auto context = GraphicSystem::Get()->GetContext();
	HRESULT hr = DirectX::CreateWICTextureFromFile(device, context, fileName.c_str(), nullptr, &mShaderResourceView);
	ASSERT(SUCCEEDED(hr), "Texture: failed to create texture %s", fileName.u8string().c_str());
}

void Texture::Terminate()
{
	SafeRealease(mShaderResourceView);
}

void Texture::BindVS(uint32_t slot) const
{
	auto context = GraphicSystem::Get()->GetContext();
	context->VSSetShaderResources(slot, 1, &mShaderResourceView);
}
void Texture::BindPS(uint32_t slot) const
{
	auto context = GraphicSystem::Get()->GetContext();
	context->PSSetShaderResources(slot, 1, &mShaderResourceView);
}