#pragma once
#include "Color.h"


namespace GoatedEngine::Graphics
{
	class GraphicSystem final
	{
	public:
		// singleton setup code
		static void StaticInitialize(HWND window, bool fullscreen);
		static void StaticTerminate();
		static GraphicSystem* Get();

		//file code
		GraphicSystem() = default;
		~GraphicSystem();
		GraphicSystem(const GraphicSystem&) = delete;
		GraphicSystem(const GraphicSystem&&) = delete;
		GraphicSystem& operator=(const GraphicSystem&) = delete;
		GraphicSystem& operator=(const GraphicSystem&&) = delete;

		void Initialize(HWND window, bool fullscreen);
		void Terminate();

		void BeginRender();
		void EndRender();

		void ToggleFullscreen();
		void Resize(uint32_t width, uint32_t height);

		void ResetRenderTarget();
		void ResetViewport();

		void SetClearColor(const Color& color);
		void SetVSync(bool vSync);

		uint32_t GetBackBufferWidth() const;
		uint32_t GetBackBufferHeight() const;
		float GetBackBufferAspectRatio() const;

		ID3D11Device* GetDevice();
		ID3D11DeviceContext* GetContext();
	private:
		static LRESULT CALLBACK GraphicSystemMessageHandle(HWND window, UINT msg, WPARAM wParam, LPARAM lParam);

		ID3D11Device* mD3DDevice = nullptr;
		ID3D11DeviceContext* mImmediateContext = nullptr;

		IDXGISwapChain* mSwapChain = nullptr;
		ID3D11RenderTargetView* mRenderTargetView = nullptr;

		ID3D11Texture2D* mDepthStencilBuffer = nullptr;
		ID3D11DepthStencilView* mDepthStencilView = nullptr;

		DXGI_SWAP_CHAIN_DESC mSwapChainDesc{};
		D3D11_VIEWPORT mViewport{};

		Color mClearColor = Colors::Black;
		UINT mVsync = 1;
	};
}
