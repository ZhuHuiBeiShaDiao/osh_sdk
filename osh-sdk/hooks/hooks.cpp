#include "../includes.h"
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
	auto size = g_renderer->get_instance(  )->GetRenderer( ).GetDisplaySize( );

	time_t t = std::time( nullptr );
	tm tm = *std::localtime( &t );

	std::ostringstream oss;
	oss << std::put_time( &tm, "%I:%M%p" );
	auto time = oss.str( );

	g_renderer->ansi_text( g_renderer->get_font( FONT_VERDANA_7PX ), OSHColor::White( ),
	                      OSHColor::Black( ), size.Width - 268, size.Height - size.Height + 22, OUTLINED,
	                      "oshgui sdk | %s", time.c_str( ) );
}

HRESULT __stdcall hooks::Present( IDirect3DDevice9 *device, const RECT *pSourceRect, const RECT *pDestRect, HWND hDestWindowOverride, const RGNDATA *pDirtyRegion ) {
	static bool once{ false };
	if( !once ) {
		g_renderer->init( device );
		g_input->init( "Valve001", g_renderer->get_instance(  ) );
		g_menu.init( );
		once = true;
	}

	g_renderer->start_drawing( device );

	g_renderer->ansi_text( g_renderer->get_font( FONT_VERDANA_7PX ), OSHColor::Red( ),
		OSHColor::Black( ), 10, 200, OUTLINED,
		std::to_string( g_menu.test ) );

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
