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
	\file	Backtrace.cc
	\brief	implementation the Backtrace class
	\date	Mon Mar 30 2009
	\author	Nestal Wan
*/

#include "Backtrace.hh"

#include "SymbolInfo.hh"
#include "util/CArray.hh"

#include <algorithm>
#include <cstring>
#include <sstream>

namespace pdf {

Backtrace::Backtrace( std::size_t skip ) :
	m_count( SymbolInfo::Instance()->Backtrace(m_stack, Count(m_stack) )),
	m_skip( std::min( skip, m_count ) )
{
}

Backtrace::Backtrace( const Backtrace& bt ) :
	m_count( bt.m_count ),
	m_skip( bt.m_skip )
{
	std::memcpy( m_stack, bt.m_stack, m_count * sizeof(m_stack[0]) ) ;
}

/*!	\brief	operator<< for printing backtraces
    \internal

	This function will call SymbolInfo::PrintTrace() to print out a backtrace
	to the stream. It will use the SymbolInfo::Instance() singleton to get
	the symbol information.
	\param	os	The output stream.
	\param	b	The backtrace information.
	\sa SymbolInfo::Backtrace(), SymbolInfo::Instance()
*/
std::ostream& operator<<( std::ostream& os, const pdf::Backtrace& b )
{
	// the 1st function in the stack is SymbolInfo::Backtrace()
	// the 2nd one is the Backtrace() constructor
	// both are not interesting to user
	for ( std::size_t i = b.m_skip ; i < b.m_count ; i++ )
		SymbolInfo::Instance()->PrintTrace( b.m_stack[i], os, i - b.m_skip ) ;

	return os ;
}

std::string Backtrace::ToString( ) const
{
	std::ostringstream oss ;
	oss << *this ;
	return oss.str( ) ;
}

} // end of namespace
