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
	\file	FilterIOStream.cc
	\brief	implementation the FilterIOStream class
	\date	Wed Mar 4 2009
	\author	Nestal Wan
*/

#include "FilterIOStream.hh"
#include "StreamFilter.hh"

#include <cassert>
#include <cstring>

namespace pdf {

FilterIOStream::FilterIOStream( StreamFilter *str )
	: m_str( str )
{
	assert( m_str != 0 ) ;
}

FilterIOStream::~FilterIOStream( )
{
}

int FilterIOStream::underflow( )
{
	assert( m_str != 0 ) ;

	if ( gptr() < egptr() )
		return traits_type::to_int_type( *gptr() ) ;
	
	if ( BufferIn() < 0 )
		return traits_type::eof() ;
	else
		return traits_type::to_int_type( *gptr() ) ;
}

int FilterIOStream::BufferIn( )
{
/*	std::streamsize num_pb = std::min( gptr() - eback(), m_pb_size ) ;
	std::memcpy( m_buf + (m_pb_size - num_pb), gptr() - num_pb, num_pb ) ;
*/
	std::size_t count = m_str->Read( (unsigned char*)m_buf, sizeof( m_buf ) ) ;
	if ( count == 0 )
		return -1 ;
	
	setg( m_buf, m_buf, m_buf + count ) ;
	return static_cast<int>( count ) ;
}

} // end of namespace
