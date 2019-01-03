#pragma once

namespace hooks {
	namespace fn {
		using Present_t = HRESULT (__stdcall *)( IDirect3DDevice9 *, const RECT *, const RECT *, HWND, const RGNDATA * );
		using Reset_t = HRESULT (__stdcall *)( IDirect3DDevice9 *, D3DPRESENT_PARAMETERS * );
	}

	HRESULT __stdcall Present( IDirect3DDevice9 *device, const RECT *pSourceRect, const RECT *pDestRect,
	                           HWND hDestWindowOverride, const RGNDATA *pDirtyRegion );
	HRESULT __stdcall Reset( IDirect3DDevice9 *device, D3DPRESENT_PARAMETERS *pPresentationParameters );
}

class c_hooks {
	uintptr_t m_d3d9_vmt = pattern::find< uintptr_t >( GetModuleHandleA( "shaderapidx9.dll" ), "A1 ? ? ? ? 50 8B 08 FF 51 0C" );

public:
	std::unique_ptr< c_vmt > m_directx;

	void init( );
	void unload( );
};

extern c_hooks g_hooks;