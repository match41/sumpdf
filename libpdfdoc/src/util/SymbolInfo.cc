/***************************************************************************
 *   Copyright (C) 2006 by Nestal Wan                                      *
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
 ***************************************************************************/

/*!
	\file	SymbolInfo.cc
	\brief	implementation the SymbolInfo class
	\date	Wed Mar 4 2009
	\author	Nestal Wan
*/

#include "SymbolInfo.hh"
#include "Util.hh"

#include <vector>

#include <bfd.h>
#include <execinfo.h>
#include <dlfcn.h>

#include <cassert>
#include <cstring>
#include <cstdlib>
#include <ostream>
#include <iomanip>
#include <algorithm>

namespace pdf {

struct SymbolInfo::Impl
{
	bfd 	*m_bfd ;
	asymbol	**m_symbols ;
	long	m_symbol_count ;
} ;

SymbolInfo::SymbolInfo( )
	: m_impl( new Impl )
{
	bfd_init( ) ;
	
	// opening itself
	bfd *b = bfd_openr( "/proc/self/exe", 0 ) ;
	if ( bfd_check_format(b, bfd_archive) )
	{
		bfd_close( b ) ;
		throw -1 ;
	}
	char **matching ;
	if ( !bfd_check_format_matches (b, bfd_object, &matching))
	{
		fprintf (stderr, "%s: %s\n", bfd_get_filename (b),
				bfd_errmsg (bfd_get_error ()));
		if (bfd_get_error () == bfd_error_file_ambiguously_recognized)
		{
			char **p = matching;
			fprintf (stderr, "%s: Matching formats:", bfd_get_filename (b));
			while (*p)
			{
				fprintf (stderr, " %s", *p++);
			}
			fprintf (stderr, "\n");
			std::free (matching);
		}
		bfd_close (b);
		b = NULL;
		throw -1;
	}
	m_impl->m_bfd = b ;

	long storage_needed = bfd_get_symtab_upper_bound( m_impl->m_bfd ) ;

	m_impl->m_symbols		= (asymbol**)std::malloc( storage_needed ) ;
	m_impl->m_symbol_count	= bfd_canonicalize_symtab( b, m_impl->m_symbols ) ;
}

SymbolInfo::~SymbolInfo( )
{
	if ( m_impl->m_symbols != 0 )
		std::free( m_impl->m_symbols ) ;
}

struct SymbolInfo::BacktraceInfo
{
	const SymbolInfo	*m_pthis ;
	void		*m_addr ;
	const char	*m_filename ;
	const char	*m_func_name ;
	unsigned int	m_lineno ;
	unsigned int	m_is_found ;
	
	static void Callback(bfd *abfd, asection *section, void* _address ) ;
} ;

void SymbolInfo::BacktraceInfo::Callback( bfd *abfd, asection *section,
                                          void* data )
{
	BacktraceInfo *info = (BacktraceInfo *)data ;
	if ((section->flags & SEC_ALLOC) == 0)
	{
		return;
	}
	
	bfd_vma vma = bfd_get_section_vma(abfd, section);
	
	unsigned long address = (unsigned long)(info->m_addr);
	if (address < vma)
		return;
	
	bfd_size_type size = bfd_section_size(abfd, section);
	if ( address > (vma + size))
	{
		return;
	}
	
	const SymbolInfo *pthis	= info->m_pthis ;
	info->m_is_found	=  bfd_find_nearest_line( abfd, section,
	                                              pthis->m_impl->m_symbols,
	                                              address - vma,
	                                              &info->m_filename,
	                                              &info->m_func_name,
	                                              &info->m_lineno);
}

void SymbolInfo::Backtrace( std::ostream& os, std::size_t limit ) const
{
	void *stack[100] ;
	int count = backtrace( stack, Count(stack) ) ;
	std::size_t loop = std::min( (std::size_t)count, 1+limit ) ;
	for ( std::size_t i = 1 ; i < loop ; i++ )
	{
		BacktraceInfo btinfo =
		{
			this, stack[i], 0, 0, 0, false
		} ;
		
		Dl_info sym ;
		bfd_map_over_sections( m_impl->m_bfd,
		                       &SymbolInfo::BacktraceInfo::Callback,
		                       &btinfo ) ;
		if ( btinfo.m_is_found )
		{
			std::string filename = ( btinfo.m_filename == 0 ? std::string()
			                         : btinfo.m_filename ) ;
#ifdef SRC_DIR
			std::size_t pos = filename.find( SRC_DIR ) ;
			if ( pos != std::string::npos )
				filename.erase( pos, std::strlen( SRC_DIR ) ) ;
#endif
			os << "#" << i << " " << stack[i] << " "
			   << filename << ":" << btinfo.m_lineno 
			   << " " << Demangle(btinfo.m_func_name)
			   << std::endl ;
		}
		else if ( dladdr( stack[i], &sym ) )
			os << "#" << i << " " << stack[i] << " "
				<< sym.dli_fname
				<< " " << Demangle( sym.dli_sname )
				<< std::endl ;
	}
}

SymbolInfo* SymbolInfo::Instance( )
{
	static SymbolInfo sthis ;
	return &sthis ;
}

} // end of namespace
