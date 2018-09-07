/* This file is part of oshgui_sdk by alpine971, licensed under the MIT license:
*
* MIT License
*
* Copyright (c) alpine971 2018
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

#include "pattern.h"

uintptr_t pattern::find( uintptr_t address, uintptr_t length, const char *pattern, int offset ) {
	//	sanity checks
	if ( !address || !length || !pattern ) {
		return 0;
	}

	//	some stuff we need before we begin searching
	auto	*string_pattern	= pattern;
	uintptr_t match_data	= 0;
	uintptr_t return_data	= 0;

	//	scan for the pattern
	for ( auto data_patch { address }; data_patch < length; ++data_patch ) {
		if ( !*string_pattern ) {
			return_data = match_data;
		}

		//	byte wildcard handling
		if ( *pubyte_t( string_pattern ) == '\?' ||
			*reinterpret_cast< ubyte_t * >( data_patch ) == ( ( string_pattern [ 0 ] >= '0'
				&& string_pattern [ 0 ] <= '9' ? string_pattern [ 0 ] - '0' : ( string_pattern [ 0 ] & ~32 ) - 'A' + 10 ) << 4 | ( string_pattern [ 1 ] >= '0' && string_pattern [ 1 ] <= '9' ? string_pattern [ 1 ] - '0' : ( string_pattern [ 1 ] & ~32 ) - 'A' + 10 ) ) ) {
			if ( !match_data )
				match_data = data_patch;

			if ( !string_pattern [ 2 ] )
				return_data = match_data;

			string_pattern += *puword_t( string_pattern ) == 16191 || *pubyte_t( string_pattern ) != '\?' ? 3 : 2;
		}

		//	doesn't match, search elsewhere
		else {
			string_pattern = pattern;
			match_data = 0;
		}
	}

	return ( return_data + offset );
}