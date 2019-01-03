#pragma once

class c_vmt {
private:
	uintptr_t m_vmt_base; // VMT base ( on heap / in rdata ).
	uintptr_t *m_old_vmt; // actual VMT.
	uint16_t m_total_methods; // total number of methods in VMT.
	std::unique_ptr< uintptr_t[] > m_new_vmt; // our newly allocated VMT.
	uintptr_t *m_new_vmt_start; // the start ptr to our new VMT ( since RTTI was copied ).

	__forceinline uint16_t count_methods( ) const {
		uint16_t i = 0;

		while( util::valid_code_ptr( m_old_vmt[ i ] ) )
			++i;

		return i;
	}

public:
	c_vmt( );

	c_vmt( void *base );

	c_vmt( uintptr_t base );

	~c_vmt( );

	bool init( void *base );

	bool init( uintptr_t base );

	bool hook_method( uint16_t index, void *func ) const;

	bool unhook_method( uint16_t index ) const;

	bool unhook_all( ) const;

	template< typename t = uintptr_t >
	__forceinline t get_old_method( uint16_t index ) {
		return reinterpret_cast< t >( m_old_vmt[ index ] );
	}
};
