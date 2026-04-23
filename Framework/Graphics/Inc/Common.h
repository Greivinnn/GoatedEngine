#pragma once
#include <Core/Inc/Core.h>

// directx11 
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <DirectXTK/Inc/CommonStates.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

template<class T>
inline void SafeRealease(T*& ptr)
{
	if (ptr)
	{
		ptr->Release();
		ptr = nullptr;
	}
}