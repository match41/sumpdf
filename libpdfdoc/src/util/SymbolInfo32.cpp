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
#include "Util.hh"

#include <ostream>
#include <string>

#include <windows.h>
#include <Dbghelp.h>

// _ReturnAddress should be prototyped before use
extern "C" void * _ReturnAddress(void);

#pragma intrinsic(_ReturnAddress)

namespace pdf {

struct SymbolInfo::Impl {} ;

SymbolInfo::SymbolInfo( )
{
    ::SymInitialize( ::GetCurrentProcess(), 0, TRUE ) ;
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
    // Get the required values for initialization of the STACKFRAME64
    // structure to be passed to StackWalk64(). Required fields are
    // AddrPC and AddrFrame.
    DWORD_PTR programcounter = (DWORD_PTR)_ReturnAddress( ) ;
    
    // Get the frame pointer (aka base pointer)
    DWORD_PTR    framepointer;
    __asm mov [framepointer], ebp

    // Initialize the STACKFRAME64 structure.
    STACKFRAME64 frame;
    memset( &frame, 0, sizeof(frame) ) ;
    frame.AddrPC.Offset    = programcounter;
    frame.AddrPC.Mode      = AddrModeFlat;
    frame.AddrFrame.Offset = framepointer;
    frame.AddrFrame.Mode   = AddrModeFlat;

    // .. use StackWalk to walk the stack ..
    std::size_t idx = 0 ;
	CONTEXT	context;
    for ( ; StackWalk64( IMAGE_FILE_MACHINE_I386,
					     GetCurrentProcess( ),
						 GetCurrentThread( ),
						 &frame, &context, 0, 
						 SymFunctionTableAccess64, 
						 SymGetModuleBase64, 0 ) ; idx++ )
        stack[idx] = frame.AddrPC.Offset ;

    return idx ;
}

void SymbolInfo::PrintTrace( addr_t addr, std::ostream& os, std::size_t idx )
{
    static const DWORD name_length = 1024 ;
    IMAGEHLP_SYMBOL64 *sym =
    	(IMAGEHLP_SYMBOL64 *)malloc( sizeof(IMAGEHLP_SYMBOL64) + name_length );
    sym->SizeOfStruct   = sizeof(IMAGEHLP_SYMBOL64) ;
    sym->MaxNameLength  = name_length ;

    DWORD64 offset ;
    if ( SymGetSymFromAddr64( GetCurrentProcess(), addr, &offset, sym ) )
    {
        IMAGEHLP_LINE64 line = { sizeof(IMAGEHLP_LINE64) } ;
        SymGetLineFromAddr64( GetCurrentProcess(), addr, 0, &line ) ;
        
	    os << "#" << idx << " " << std::hex << addr
	       << " "
	       << (line.FileName != 0 ? std::string(line.FileName)
                                  : std::string() ) << ":"
           << line.LineNumber 
	       << " " << sym->Name
	       << std::endl ;
    }
    
    free( sym ) ;
}

} // end of namespace
