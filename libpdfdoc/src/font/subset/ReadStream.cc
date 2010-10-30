/***************************************************************************\
 *   Copyright (C) 2006 by Nestal Wan                                      *
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

/**	\file	ReadStream.cc
	\brief	implementation of the ReadStream class
	\date	Apr 10, 2010
	\author	Nestal Wan
*/

#include "ReadStream.hh"

#include "Types.hh"
#include "util/Endian.hh"

// boost headers
#include <boost/detail/endian.hpp>

#include <cstring>

namespace pdf {

/**	constructor
	
*/
ReadStream::ReadStream( const unsigned char *p, std::size_t size )
	: m_ptr( p )
	, m_size( size )
	, m_failed( false )
{
}

template <typename T>
ReadStream& ReadStream::operator>>( T& v )
{
	if ( !m_failed && m_size >= sizeof(v) )
	{
		std::memcpy( &v, m_ptr, sizeof(v) ) ;

#ifdef BOOST_LITTLE_ENDIAN
		v = SwapByte( v ) ;
#endif
		
		m_ptr	+= sizeof(T) ;
		m_size	-= sizeof(T) ;
	}
	else
		m_failed = true ;
	
	return *this ;
}

template ReadStream& ReadStream::operator>>( u32& v ) ;
template ReadStream& ReadStream::operator>>( u16& v ) ;

ReadStream::operator const void*() const
{
	return m_failed ? 0 : this ;
}

std::size_t ReadStream::Size( ) const
{
	return m_size ;
}

const unsigned char* ReadStream::Data( ) const
{
	return m_ptr ;
}

} // end of namespace
