/***************************************************************************\
 *   Copyright (C) 2009 by Nestal Wan                                      *
 *   me@nestal.net                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2.                              *
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

/*!
	\file	SymbolInfo.hh
	\brief	definition the SymbolInfo class
	\date	Wed Mar 4 2009
	\author	Nestal Wan
*/

#ifndef __PDF_SYMBOL_INFO_HEADER_INCLUDED__
#define __PDF_SYMBOL_INFO_HEADER_INCLUDED__

#include "Addr.hh"

#include <memory>
#include <iosfwd>

namespace pdf {

///	ource code symbolic information
/**	\internal
	
	This class represents symbolic information about the source code,
	e.g. function names and line numbers. It provides an interface to
	lookup these informations by address.
*/
class SymbolInfo
{
public :
	SymbolInfo( ) ;
	~SymbolInfo( ) ;

    /*! \brief  singleton function
        \internal
        
        Returns the SymbolInfo singleton. Normally only one object
        of SymbolInfo is enough for one application, so a singleton
        is enough. This function will create the SymbolInfo object
        in the first call.
    */
	static SymbolInfo* Instance( ) ;

	std::size_t Backtrace( addr_t *stack, std::size_t count ) ;
	void PrintTrace( addr_t addr, std::ostream& os, std::size_t idx = 0 ) ;
	
private :
	struct Impl ;
	const std::auto_ptr<Impl> m_impl ;
	
	struct BacktraceInfo ;
	friend struct BacktraceInfo ;
} ;

} // end of namespace

#endif
