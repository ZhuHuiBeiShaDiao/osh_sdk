#include "../sdk/includes.h"
#include "hooks.h"

c_hooks g_hooks;

hooks::fn::Present_t original_present;
hooks::fn::Reset_t original_reset;

void c_hooks::init( ){
	present_address = pattern::find< uintptr_t >( GetModuleHandleA( "gameoverlayrenderer.dll" ), "FF 15 ? ? ? ? 8B F8 85 DB" ) + 0x2;
	reset_address = pattern::find< uintptr_t >( GetModuleHandleA( "gameoverlayrenderer.dll" ), "FF 15 ? ? ? ? 8B F8 85 FF 78 18" ) + 0x2;
	original_present = **reinterpret_cast< hooks::fn::Present_t** >( present_address );
	original_reset = **reinterpret_cast< hooks::fn::Reset_t** >( reset_address );

	**reinterpret_cast< void*** >( present_address ) = reinterpret_cast< void* >( &hooks::Present );
	**reinterpret_cast< void*** >( reset_address ) = reinterpret_cast< void* >( &hooks::Reset );
}

void c_hooks::unload( ) {
	**reinterpret_cast< void*** >( present_address ) = reinterpret_cast< void* >( original_present );
	**reinterpret_cast< void*** >( reset_address ) = reinterpret_cast< void* >( original_reset );
}

void watermark( ) {
	static auto size = g_renderer.m_instance->GetRenderer( ).GetDisplaySize( );

	time_t t = std::time( nullptr );
	tm tm = *std::localtime( &t );

	std::ostringstream oss;
	oss << std::put_time( &tm, "%I:%M%p" );
	auto time = oss.str( );

	g_renderer.ansi_text( g_renderer.get_font( FONT_VERDANA_7PX ), OSHColor( 0.8f, 1.f, 1.f, 1.f ),
		OSHColor( 0.f, 0.f, 0.f, 0.f ), size.Width - 268, size.Height - size.Height + 22, 0,
		"oshgui sdk | %s", time.c_str( ) );
}

HRESULT __stdcall hooks::Present(IDirect3DDevice9 * device, const RECT * pSourceRect, const RECT * pDestRect, HWND hDestWindowOverride, const RGNDATA * pDirtyRegion)
{
	static bool once{ false };
	if( !once ) {
		g_renderer.init( device );
		once = true;
	}

	g_renderer.start_drawing( device );

	watermark( );

	g_renderer.end_drawing( device );

	return original_present( device, pSourceRect, pDestRect, hDestWindowOverride,
		pDirtyRegion );
}

HRESULT __stdcall hooks::Reset(IDirect3DDevice9 * device, D3DPRESENT_PARAMETERS * pPresentationParameters)
{
	g_renderer.m_instance->GetRenderer().PreD3DReset();

	auto ret = original_reset( device, pPresentationParameters );

	g_renderer.m_instance->GetRenderer().PostD3DReset();

	return ret;
}