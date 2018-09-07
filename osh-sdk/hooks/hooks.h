#pragma once

namespace hooks{
	namespace fn{
		using Present_t = HRESULT (__stdcall *)( IDirect3DDevice9 *, const RECT *, const RECT *, HWND, const RGNDATA * );
		using Reset_t = HRESULT (__stdcall *)( IDirect3DDevice9 *, D3DPRESENT_PARAMETERS * );
	}
	HRESULT __stdcall Present( IDirect3DDevice9 *device, const RECT *pSourceRect, const RECT *pDestRect,
		HWND hDestWindowOverride, const RGNDATA *pDirtyRegion );
	HRESULT __stdcall Reset( IDirect3DDevice9 *device, D3DPRESENT_PARAMETERS *pPresentationParameters );
}

extern hooks::fn::Present_t original_present;
extern hooks::fn::Reset_t original_reset;

class c_hooks {
	uintptr_t present_address;
	uintptr_t reset_address;
public:
	void init( );
	void unload( );
};

extern c_hooks g_hooks;