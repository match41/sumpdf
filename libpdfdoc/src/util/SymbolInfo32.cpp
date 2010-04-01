/***************************************************************************\
 *   Copyright (C) 2009 by Nestal Wan                                      *
 *   me@nestal.net                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
\***************************************************************************/

#include "SymbolInfo.hh"
#include "util/Util.hh"

#include <ostream>
#include <string>

#include <windows.h>

#ifdef HAVE_DBGHELP
#include <Dbghelp.h>
#ifdef _M_IX86
#pragma optimize( "g", off )
#pragma warning( push )
#pragma warning( disable : 4748 )
#endif

#endif

#include <iostream>

namespace pdf {

struct SymbolInfo::Impl {} ;

SymbolInfo::SymbolInfo( )
{
#ifdef HAVE_DBGHELP
	::SymInitialize( ::GetCurrentProcess(), 0, TRUE ) ;
#endif
}

SymbolInfo::~SymbolInfo( )
{
}

SymbolInfo* SymbolInfo::Instance( )
{
	static SymbolInfo sthis ;
	return &sthis ;
}

std::size_t SymbolInfo::Backtrace( addr_t *stack, std::size_t count )
{
#ifdef HAVE_DBGHELP
    // Get the frame pointer (aka base pointer)
	CONTEXT	context;
    memset( &context, 0, sizeof( CONTEXT ) );

	::RtlCaptureContext( &context );
    
	// Initialize the STACKFRAME64 structure.
    STACKFRAME64 frame;
    memset( &frame, 0, sizeof(frame) ) ;
#ifdef _M_IX86
    frame.AddrPC.Offset    = context.Eip;
    frame.AddrFrame.Offset = context.Ebp;
    frame.AddrStack.Offset = context.Esp;
#else
    frame.AddrPC.Offset    = context.Rip;
    frame.AddrFrame.Offset = context.Rbp;
    frame.AddrStack.Offset = context.Rsp;
#endif

	frame.AddrPC.Mode      = AddrModeFlat;
	frame.AddrFrame.Mode   = AddrModeFlat;
	frame.AddrStack.Mode   = AddrModeFlat;

	DWORD arch =
#ifdef _M_IX86
    IMAGE_FILE_MACHINE_I386;
#else
    IMAGE_FILE_MACHINE_AMD64;
#endif

    // .. use StackWalk to walk the stack ..
    std::size_t idx = 0 ;
    while ( idx < count && StackWalk64(
		arch,
		GetCurrentProcess( ),
		GetCurrentThread( ),
		&frame,
		&context,
		0, SymFunctionTableAccess64, 
		SymGetModuleBase64, 0 ) )
	{
		if ( frame.AddrPC.Offset == frame.AddrReturn.Offset )
			break;

		stack[idx++] = frame.AddrPC.Offset ;
	}
	
    return idx ;
#else
	return 0 ;
#endif
}

void SymbolInfo::PrintTrace( addr_t addr, std::ostream& os, std::size_t idx )
{
#ifdef HAVE_DBGHELP
	static const DWORD name_length = 2048 ;
	IMAGEHLP_SYMBOL64 *sym =
		(IMAGEHLP_SYMBOL64 *)malloc( sizeof(IMAGEHLP_SYMBOL64) + name_length );
	sym->SizeOfStruct   = sizeof(IMAGEHLP_SYMBOL64) ;
	sym->MaxNameLength  = name_length ;

	DWORD64 offset ;
	if ( SymGetSymFromAddr64( GetCurrentProcess(), addr, &offset, sym ) )
	{
		os << "#" << idx << " " << std::hex << addr << " " ;

		IMAGEHLP_LINE64 line = { sizeof(IMAGEHLP_LINE64) } ;
		if ( SymGetLineFromAddr64( GetCurrentProcess(), addr, 0, &line ) )
			os	<< (line.FileName != 0 ? std::string(line.FileName)
									   : std::string() ) << ":"
				<< line.LineNumber ;

		
		
		os << " " << sym->Name << std::endl ;
	}

	free( sym ) ;

#endif
}

} // end of namespace
