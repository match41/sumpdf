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

/*!
	\file	DeflateStreamBuf.cc
	\brief	implementation the DeflateStreamBuf class
	\date	Sun Mar 22 2009
	\author	Nestal Wan
*/

#include "DeflateStreamBuf.hh"

#include <cassert>

namespace pdf {

DeflateStreamBuf::DeflateStreamBuf( std::streambuf *str )
	: m_str( str )
{
	assert( m_str != 0 ) ;
	setg( m_buf + 4, m_buf + 4, m_buf + 4 ) ;
}

DeflateStreamBuf::int_type DeflateStreamBuf::underflow( )
{
	if ( gptr() < egptr() )
		return traits_type::to_int_type( *gptr() ) ;
	
	if ( BufferIn() < 0 )
		return traits_type::eof() ;
	else
		return traits_type::to_int_type( *gptr() ) ;
}

DeflateStreamBuf::int_type DeflateStreamBuf::BufferIn( )
{
}

} // end of namespace
