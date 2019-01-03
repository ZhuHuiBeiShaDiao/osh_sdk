#include "../sdk/includes.h"
#include "hooks.h"

c_hooks g_hooks;

void c_hooks::init( ) {
	m_directx = std::make_unique< c_vmt >( **reinterpret_cast< uintptr_t ** >( m_d3d9_vmt + 1 ) );

	m_directx->hook_method( 17, hooks::Present );
	m_directx->hook_method( 16, &hooks::Reset );
}

void c_hooks::unload( ) {
	m_directx->unhook_all( );
}

void watermark( ) {
	static auto size = g_renderer->get_instance(  )->GetRenderer( ).GetDisplaySize( );

	time_t t = std::time( nullptr );
	tm tm = *std::localtime( &t );

	std::ostringstream oss;
	oss << std::put_time( &tm, "%I:%M%p" );
	auto time = oss.str( );

	g_renderer->ansi_text( g_renderer->get_font( FONT_VERDANA_7PX ), OSHColor( 0.8f, 1.f, 1.f, 1.f ),
	                      OSHColor( 0.f, 0.f, 0.f, 0.f ), size.Width - 268, size.Height - size.Height + 22, 0,
	                      "oshgui sdk | %s", time.c_str( ) );
}

HRESULT __stdcall hooks::Present( IDirect3DDevice9 *device, const RECT *pSourceRect, const RECT *pDestRect, HWND hDestWindowOverride, const RGNDATA *pDirtyRegion ) {
	static bool once{ false };
	if( !once ) {
		g_renderer->init( device );
		g_input->init( "Valve001", g_renderer->get_instance(  ) );
		once = true;
	}

	g_renderer->start_drawing( device );

	watermark( );

	g_renderer->end_drawing( device );

	return g_hooks.m_directx->get_old_method< fn::Present_t >( 17 )( device, pSourceRect, pDestRect, hDestWindowOverride,
	                         pDirtyRegion );
}

HRESULT __stdcall hooks::Reset( IDirect3DDevice9 *device, D3DPRESENT_PARAMETERS *pPresentationParameters ) {
	g_renderer->get_instance(  )->GetRenderer( ).PreD3DReset( );

	auto ret = g_hooks.m_directx->get_old_method< fn::Reset_t >( 17 )( device, pPresentationParameters );

	g_renderer->get_instance(  )->GetRenderer( ).PostD3DReset( );

	return ret;
}
