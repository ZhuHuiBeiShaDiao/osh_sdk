#pragma once

#include <windows.h>

// wee woo wee woo, it's the MSVC macro police :^)
typedef unsigned char ubyte_t;
typedef ubyte_t *pubyte_t;
typedef unsigned short uword_t;
typedef uword_t *puword_t;

namespace pattern {
	uintptr_t find( uintptr_t address, uintptr_t length, const char *pattern, int offset = 0 );

	template < typename t >
	static t find( void *module, const char *pattern, int offset = 0 ) {
		auto base			= reinterpret_cast< uintptr_t >( module );
		auto data_header	= reinterpret_cast< PIMAGE_NT_HEADERS >( base + reinterpret_cast< PIMAGE_DOS_HEADER >( base )->e_lfanew )->OptionalHeader;

		return ( t ) ( find( base + data_header.BaseOfCode, base + data_header.SizeOfCode, pattern, offset ) );
	}
}