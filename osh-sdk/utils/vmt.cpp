#include "../includes.h"
#include "vmt.h"

c_vmt::c_vmt( ): m_vmt_base{ 0 }, m_old_vmt{ nullptr }, m_total_methods{ 0 }, m_new_vmt{ nullptr },
                 m_new_vmt_start{ nullptr } { }

c_vmt::c_vmt( void *base ): m_vmt_base{ 0 }, m_old_vmt{ nullptr }, m_total_methods{ 0 }, m_new_vmt{ nullptr },
                            m_new_vmt_start{ nullptr } {
	init( base );
}

c_vmt::c_vmt( uintptr_t base ): m_vmt_base{ 0 }, m_old_vmt{ nullptr }, m_total_methods{ 0 }, m_new_vmt{ nullptr },
                                m_new_vmt_start{ nullptr } {
	init( base );
}

c_vmt::~c_vmt( ) = default;

bool c_vmt::init( void *base ) {
	return init( uintptr_t( base ) );
}

bool c_vmt::init( uintptr_t base ) {

	if( !base )
		return false;

	m_vmt_base = base;

	m_old_vmt = *reinterpret_cast< uintptr_t ** >( base );
	if( !m_old_vmt )
		return false;

	m_total_methods = count_methods( );
	if( !m_total_methods )
		return false;

	// allocate new VMT, making room for RTTI ptr.
	m_new_vmt = std::make_unique< uintptr_t[] >( m_total_methods + 1 );
	if( !m_new_vmt )
		return false;

	// get address of our newly allocated memory.
	const uintptr_t alloc_base = uintptr_t( m_new_vmt.get( ) );

	// copy VMT, starting from RTTI.
	std::memcpy(
		( void * )alloc_base,
		( const void * )( m_old_vmt - 1 ),
		( m_total_methods + 1 ) * sizeof( uintptr_t )
	);

	// get start for new VMT.
	// VMT start is actually 1 function ahead due to RTTI copy.
	m_new_vmt_start = reinterpret_cast< uintptr_t * >( alloc_base + sizeof( uintptr_t ) );

	// set base to our new VMT.
	*reinterpret_cast< uintptr_t * >( base ) = uintptr_t( m_new_vmt_start );

	return true;
}

bool c_vmt::hook_method( uint16_t index, void *func ) const {
	if( !m_old_vmt || !m_new_vmt_start || index > m_total_methods || !func )
		return false;

	m_new_vmt_start[ index ] = uintptr_t( func );

	return true;
}

bool c_vmt::unhook_method( uint16_t index ) const {
	if( !m_old_vmt || !m_new_vmt_start || index > m_total_methods )
		return false;

	m_old_vmt[ index ] = m_new_vmt_start[ index ];

	return true;
}

bool c_vmt::unhook_all( ) const {
	if( !m_old_vmt || !m_vmt_base )
		return false;

	*reinterpret_cast< uintptr_t * >( m_vmt_base ) = uintptr_t( m_old_vmt );

	return true;
}
