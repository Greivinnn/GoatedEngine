#pragma once

namespace GoatedEngine::Graphics::DebugUI
{
	enum class Theme
	{
		Classic,
		Dark,
		Light
	};

	void StaticInitialize(HWND window, bool docking = false, bool multiViewpoint = false);
	void StaticTerminate();
	void SetTheme(Theme theme);

	void BeginDraw();
	void EndDraw();
}