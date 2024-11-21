#pragma once
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx11.h"
#include "d3d9.h"
#include "d3d11.h"
#include <dxgi.h>
#include <stdint.h>
#include "../Utils/EzHook/EzHook.h"

typedef HRESULT(__stdcall* PresentDX11) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
typedef HRESULT(__stdcall* ResizeBuffer)(IDXGISwapChain* pSwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags);


typedef HRESULT(WINAPI* PresentDX9) (LPDIRECT3DDEVICE9 Device, CONST RECT* pSrcRect, CONST RECT* pDestRect, HWND hDestWindow, CONST RGNDATA* pDirtyRegion);
typedef HRESULT(__stdcall* PresentDX11) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
typedef HRESULT(__stdcall* ResizeBuffer)(IDXGISwapChain* pSwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags);

namespace DirectXHook {
	inline bool Inited = false;

	inline bool Draw = true;

	bool HookDX11();
	bool unHook();

	HRESULT __stdcall Hooked_PresentDX11(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
	HRESULT  __stdcall Hooked_ResizeBufferDX11(IDXGISwapChain* pSwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags);
}
