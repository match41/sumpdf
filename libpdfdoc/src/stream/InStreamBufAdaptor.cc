/***************************************************************************
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
 ***************************************************************************/

/*!
	\file	InStreamBufAdaptor.cc
	\brief	implementation the InStreamBufAdaptor class
	\date	Wed Mar 4 2009
	\author	Nestal Wan
*/

#include "InStreamBufAdaptor.hh"

#include "StreamFilter.hh"
#include "MockStreamFilter.hh"

#include <cassert>
#include <cstring>

namespace
{
	pdf::MockStreamFilter	dummy ;
}

namespace pdf {

InStreamBufAdaptor::InStreamBufAdaptor( StreamFilter *str )
	: m_str( str ? str : &dummy )
{
	setg( m_buf + m_pb_size,
		  m_buf + m_pb_size,
		  m_buf + m_pb_size ) ;

}

InStreamBufAdaptor::InStreamBufAdaptor( const InStreamBufAdaptor& rhs )
	: m_str( rhs.m_str )
{
	std::memcpy( m_buf, rhs.m_buf, m_pb_size ) ;
	
	setg( m_buf + m_pb_size,
		  m_buf + m_pb_size,
		  m_buf + m_pb_size ) ;
}

void InStreamBufAdaptor::Set( StreamFilter *str )
{
	assert( str != 0 ) ;
	assert( m_str == &dummy ) ;
	m_str = str ;
}

int InStreamBufAdaptor::underflow( )
{
	assert( m_str != 0 ) ;

	if ( gptr() < egptr() )
		return traits_type::to_int_type( *gptr() ) ;
	
	return BufferIn( ) ? traits_type::to_int_type( *gptr() )
	                   : traits_type::eof() ;
}

bool InStreamBufAdaptor::BufferIn( )
{
	// cannot directly use m_pb_size in template functions.
	// it should be a bug in gcc
	std::streamsize pb_size	= m_pb_size ;
	
	std::streamsize num_pb	= std::min( gptr() - eback(), pb_size ) ;
	std::memcpy( m_buf + (m_pb_size - num_pb), gptr() - num_pb, num_pb ) ;

	unsigned char *buf = reinterpret_cast<unsigned char*>( m_buf ) ;
	std::size_t count = m_str->Read( buf + m_pb_size,
	                                 m_buf_size - m_pb_size ) ;
	if ( count == 0 )
	{
		setg( 0, 0, 0 ) ;
		return false ;
	}
	else
	{
		setg( m_buf + m_pb_size - num_pb,
		      m_buf + m_pb_size,
		      m_buf + m_pb_size + count ) ;
		return true ;
	}
}

} // end of namespace
