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