/* This file is part of oshgui_sdk by alpine971, licensed under the MIT license:
*
* MIT License
*
* Copyright (c) namazso 2018
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/

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