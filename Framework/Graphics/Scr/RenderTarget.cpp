#include "Precompiled.h"
#include "RenderTarget.h"
#include "GraphicSystem.h"

using namespace GoatedEngine;
using namespace GoatedEngine::Graphics;

namespace
{
	DXGI_FORMAT GetDXGIFormat(RenderTarget::Format format)
	{
		switch (format)
		{
		case RenderTarget::Format::RGBA_U8: return DXGI_FORMAT_R8G8B8A8_UNORM;
		case RenderTarget::Format::RGBA_U32: return DXGI_FORMAT_R32G32B32A32_UINT;
		default:
			ASSERT(false, "RenderTarget: unsupported format");
			break;
		}
		return DXGI_FORMAT_R8G8B8A8_UNORM;
	}
}

RenderTarget::~RenderTarget()
{
	ASSERT(mRenderTargetView == nullptr && mDepthStencilView == nullptr, "RenderTarget: must call terminate");
}

void RenderTarget::Initialize(const std::filesystem::path& fileName)
{
	ASSERT(false, "RenderTarget: do not support this initialize");
}
void RenderTarget::Initialize(uint32_t width, uint32_t height, Format format)
{
	D3D11_TEXTURE2D_DESC desc{};
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = GetDXGIFormat(format);
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	auto device = GraphicSystem::Get()->GetDevice();
	ID3D11Texture2D* texture = nullptr;
	HRESULT hr = device->CreateTexture2D(&desc, nullptr, &texture);
	ASSERT(SUCCEEDED(hr), "RenderTarget: failed to create texture");

	hr = device->CreateShaderResourceView(texture, nullptr, &mShaderResourceView);
	ASSERT((hr), "RenderTarget: failed to create shader resource view");

	hr = device->CreateRenderTargetView(texture, nullptr, &mRenderTargetView);
	ASSERT((hr), "TargetView: failed to create render target view");

	SafeRealease(texture);

	desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	hr = device->CreateTexture2D(&desc, nullptr, &texture);
	ASSERT((hr), "RenderTarget: failed to create depth stencil texture");

	hr = device->CreateDepthStencilView(texture, nullptr, &mDepthStencilView);
	ASSERT((hr), "RenderTarget: failed tp create depth stencil view");

	SafeRealease(texture);

	mViewPort.TopLeftX = 0.0f;
	mViewPort.TopLeftY = 0.0f;
	mViewPort.Width = static_cast<float>(width);
	mViewPort.Height = static_cast<float>(height);
	mViewPort.MinDepth = 0.0f;
	mViewPort.MaxDepth = 1.0f;

}
void RenderTarget::Terminate()
{
	Texture::Terminate();

	SafeRealease(mDepthStencilView);
	SafeRealease(mRenderTargetView);
}
	 
void RenderTarget::BeginRender(Color clearColor)
{
	auto context = GraphicSystem::Get()->GetContext();

	// store current settings
	UINT numViewports = 1;
	context->OMGetRenderTargets(1, &mOldRenderTargetView, &mOldDepthStencilView);
	context->RSGetViewports(&numViewports, &mOldViewport);

	// apply render target setting
	context->ClearRenderTargetView(mRenderTargetView, &clearColor.r);
	context->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0.0f);
	context->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);
	context->RSSetViewports(1, &mViewPort);
}
void RenderTarget::EndRender()
{
	auto context = GraphicSystem::Get()->GetContext();
	context->OMSetRenderTargets(1, &mOldRenderTargetView, mDepthStencilView);
	context->RSSetViewports(1, &mOldViewport);

	SafeRealease(mOldDepthStencilView);
	SafeRealease(mOldRenderTargetView);
}