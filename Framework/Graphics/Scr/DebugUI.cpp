#include "Precompiled.h"
#include "DebugUI.h"

#include "GraphicSystem.h"

#include <ImGui/Inc/imgui_impl_dx11.h>
#include <ImGui/Inc/imgui_impl_win32.h>

using namespace GoatedEngine;
using namespace GoatedEngine::Graphics;
using namespace GoatedEngine::Core;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace 
{
	WindowMessageHandler sWindowMessageHandler;

	bool IsMouseInput(UINT msg)
	{
		switch (msg)
		{
		case WM_LBUTTONUP:
		case WM_LBUTTONDOWN:
		case WM_LBUTTONDBLCLK:
		case WM_RBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONDBLCLK:
		case WM_MBUTTONUP:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONDBLCLK:
		case WM_MOUSEWHEEL:
		case WM_MOUSEHWHEEL:
			return true;
		default:
			break;
		}
		return false;
	}
	bool IsKeyboardInput(UINT msg)
	{
		switch (msg)
		{
		case WM_CHAR:
		case WM_KEYUP:
		case WM_KEYDOWN:
		case WM_SYSKEYUP:
		case WM_SYSKEYDOWN:
			return true;
		default:
			break;
		}
		return false;
	}

	LRESULT CALLBACK DebugUIMessageHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		ImGuiIO& io = ImGui::GetIO();
		// does imgui want to consume the most input
		if (io.WantCaptureMouse && IsMouseInput(msg))
		{
			return ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam);
		}
		// does imgui want to consume any key input
		if (io.WantCaptureKeyboard && IsKeyboardInput(msg))
		{
			return ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam);
		}

		LRESULT result = ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam);
		if(result != 0)
		{
			return result;
		}
		return sWindowMessageHandler.ForwardMessage(hwnd, msg, wParam, lParam);
	}

	
}

void DebugUI::StaticInitialize(HWND window, bool docking, bool multiViewpoint)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	
	if(docking)
	{
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	}
	if(multiViewpoint)
	{
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	}

	auto device = GraphicSystem::Get()->GetDevice();
	auto context = GraphicSystem::Get()->GetContext();

	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX11_Init(device, context);

	sWindowMessageHandler.Hook(window, DebugUIMessageHandler);
}
void DebugUI::StaticTerminate()
{
	sWindowMessageHandler.Unhook();
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

}
void DebugUI::SetTheme(Theme theme)
{
	switch (theme)
	{
		case Theme::Classic:		ImGui::StyleColorsClassic(); break;
		case Theme::Dark:			ImGui::StyleColorsDark(); break;
		case Theme::Light:			ImGui::StyleColorsLight(); break;
		default:
			break;
	}
}
void DebugUI::BeginDraw()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}
void DebugUI::EndDraw()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}

}

