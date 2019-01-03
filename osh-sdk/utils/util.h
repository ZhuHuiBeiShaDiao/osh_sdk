#pragma once

namespace util {
	bool valid_code_ptr( uintptr_t addr );

	// get method from VMT.
	template< typename t >
	t vfunc( void *this_ptr, uint16_t index ) {
		return ( *static_cast< t ** >( this_ptr ) )[ index ];
	}

	// follow relative32 offset.
	// input argument is the address of the relative offset.
	template< typename t = uintptr_t >
	t follow_rel32( const uintptr_t addr ) {
		if( !addr )
			return 0;

		// get rel32 offset.
		const auto disp = *reinterpret_cast< uint32_t * >( addr );
		if( !disp )
			return 0;

		// displacement is relative to address of the next instruction.
		return ( addr + sizeof( uint32_t ) ) + disp;
	}
}
