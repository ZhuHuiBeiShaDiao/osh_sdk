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