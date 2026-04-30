#include "Precompiled.h"
#include "GraphicSystem.h"

using namespace GoatedEngine;
using namespace GoatedEngine::Graphics;

namespace
{
	std::unique_ptr<GraphicSystem> sGraphicSystem;
	Core::WindowMessageHandler sWindowMessageHandler;
}

LRESULT CALLBACK GraphicSystem::GraphicSystemMessageHandle(HWND window, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (sGraphicSystem != nullptr)
	{
		switch (msg)
		{
			case WM_SIZE:
			{
				const uint32_t width = static_cast<uint32_t>(LOWORD(lParam));
				const uint32_t height = static_cast<uint32_t>(HIWORD(lParam));
				sGraphicSystem->Resize(width, height);
				break;
			}
		}
	}
	return sWindowMessageHandler.ForwardMessage(window, msg, wParam, lParam);
}

void GraphicSystem::StaticInitialize(HWND window, bool fullscreen)
{
	ASSERT(sGraphicSystem == nullptr, "[GraphicSystem] System already initialized!");
	sGraphicSystem = std::make_unique<GraphicSystem>();
	sGraphicSystem->Initialize(window, fullscreen);
}
void  GraphicSystem::StaticTerminate()
{
	if(sGraphicSystem != nullptr)
	{
		sGraphicSystem->Terminate();
		sGraphicSystem.reset();
	}
}
GraphicSystem* GraphicSystem::Get()
{
	ASSERT(sGraphicSystem != nullptr, "[GraphicSystem] No system registered.");
	return sGraphicSystem.get();
}

//file code
GraphicSystem::~GraphicSystem()
{
	ASSERT(mD3DDevice == nullptr && mImmediateContext == nullptr, "[GraphicSystem] Terminate must be called to clean up the system.");
}

void GraphicSystem::Initialize(HWND window, bool fullscreen)
{
	RECT clientRect = {};
	GetClientRect(window, &clientRect);
	UINT width = (UINT)(clientRect.right - clientRect.left);
	UINT height = (UINT)(clientRect.bottom - clientRect.top);

	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	swapChainDesc.BufferCount = 2;
	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Height = height;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = window;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = !fullscreen;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	const D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_1;
	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		&featureLevel,
		1,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&mSwapChain,
		&mD3DDevice,
		nullptr,
		&mImmediateContext);
		
	ASSERT(SUCCEEDED(hr), "[GraphicSystem] Failed to create device and swap chain!");
	mSwapChain->GetDesc(&mSwapChainDesc);

	Resize(GetBackBufferHeight(), GetBackBufferHeight());

	sWindowMessageHandler.Hook(window, GraphicSystemMessageHandle);	
}
void GraphicSystem::Terminate()
{
	sWindowMessageHandler.Unhook();

	SafeRealease(mDepthStencilView);
	SafeRealease(mDepthStencilBuffer);
	SafeRealease(mRenderTargetView);
	SafeRealease(mSwapChain);
	SafeRealease(mImmediateContext);
	SafeRealease(mD3DDevice);

}

void GraphicSystem::BeginRender()
{
	mImmediateContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);
	mImmediateContext->ClearRenderTargetView(mRenderTargetView, (FLOAT*)(&mClearColor));
	mImmediateContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}
void GraphicSystem::EndRender()
{
	mSwapChain->Present(mVsync, 0);
}

void GraphicSystem::ToggleFullscreen()
{
	BOOL fullscreen = FALSE;
	mSwapChain->GetFullscreenState(&fullscreen, nullptr);
	mSwapChain->SetFullscreenState(!fullscreen, nullptr);

}
void GraphicSystem::Resize(uint32_t width, uint32_t height)
{
	mImmediateContext->OMSetRenderTargets(0, nullptr, nullptr);

	SafeRealease(mRenderTargetView);
	SafeRealease(mDepthStencilView);
	SafeRealease(mDepthStencilBuffer);

	HRESULT hr;
	if (width != GetBackBufferWidth() || height != GetBackBufferHeight())
	{
		hr = mSwapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
		ASSERT(SUCCEEDED(hr), "GraphicSystem: failed to access swap chain view");
		mSwapChain->GetDesc(&mSwapChainDesc);
	}

	ID3D11Texture2D* backBuffer = nullptr;
	hr = mSwapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
	ASSERT(SUCCEEDED(hr), "GraphicSystem: failed to create render target");
	
	D3D11_TEXTURE2D_DESC depthStencilDesc = {};
	depthStencilDesc.Width = GetBackBufferWidth();
	depthStencilDesc.Height = GetBackBufferHeight();
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;	
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;
	hr = mD3DDevice->CreateTexture2D(&depthStencilDesc, nullptr, &mDepthStencilBuffer);
	ASSERT(SUCCEEDED(hr), "GraphicSystem: failed to create depth stencil buffer");

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = depthStencilDesc.Format;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;
	hr = mD3DDevice->CreateDepthStencilView(mDepthStencilBuffer, &dsvDesc, &mDepthStencilView);
	ASSERT(SUCCEEDED(hr), "GraphicSystem: failed to create depth stencil view");

	ResetRenderTarget();

	mViewport.Width = static_cast<FLOAT>(GetBackBufferWidth());
	mViewport.Height = static_cast<FLOAT>(GetBackBufferHeight());
	mViewport.MinDepth = 0.0f;
	mViewport.MaxDepth = 1.0f;
	mViewport.TopLeftX = 0.0f;
	mViewport.TopLeftY = 0.0f;
	ResetViewport();
}

void GraphicSystem::ResetRenderTarget()
{
	mImmediateContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);
}
void GraphicSystem::ResetViewport()
{
	mImmediateContext->RSSetViewports(1, &mViewport);
}

void GraphicSystem::SetClearColor(const Color& color)
{
	mClearColor = color;
}
void GraphicSystem::SetVSync(bool vSync)
{
	mVsync = mVsync ? 1 : 0;
}

uint32_t GraphicSystem::GetBackBufferWidth() const
{
	return mSwapChainDesc.BufferDesc.Width;
}
uint32_t GraphicSystem::GetBackBufferHeight() const
{
	return mSwapChainDesc.BufferDesc.Height;
}
float GraphicSystem::GetBackBufferAspectRatio() const
{
	return static_cast<float>(GetBackBufferWidth()) / static_cast<float>(GetBackBufferHeight());
}

ID3D11Device* GraphicSystem::GetDevice()
{
	return mD3DDevice;
}
ID3D11DeviceContext* GraphicSystem::GetContext()
{
	return mImmediateContext;
}
		