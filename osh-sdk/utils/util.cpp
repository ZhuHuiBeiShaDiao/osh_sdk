#include "../sdk/includes.h"
#include "util.h"

bool util::valid_code_ptr( const uintptr_t addr ) {
	if( !addr )
		return false;

	MEMORY_BASIC_INFORMATION mbi;
	if( !VirtualQuery( reinterpret_cast< const void * >( addr ), &mbi, sizeof mbi ) )
		return false;

	if( !( mbi.Protect & PAGE_EXECUTE_READWRITE || mbi.Protect & PAGE_EXECUTE_READ ) )
		return false;

	return true;
}
