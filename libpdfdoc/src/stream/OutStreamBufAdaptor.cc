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

/**
	\file	OutStreamBufAdaptor.cc
	\brief	definition the OutStreamBufAdaptor class
	\date	Dec 21, 2009
	\author	Nestal Wan
*/

#include "OutStreamBufAdaptor.hh"

#include "StreamFilter.hh"
#include "MockStreamFilter.hh"

#include <cstring>
#include <cassert>

namespace
{
	pdf::MockStreamFilter	dummy ;
}

namespace pdf {

OutStreamBufAdaptor::OutStreamBufAdaptor( StreamFilter *dest )
	: m_dest( dest ? dest : &dummy )
{
	setp( m_buf, m_buf + m_buf_size ) ;
}

OutStreamBufAdaptor::~OutStreamBufAdaptor( )
{
	sync( ) ;
}

void OutStreamBufAdaptor::Set( StreamFilter *dest )
{
	assert( dest != 0 ) ;
	assert( m_dest == &dummy ) ;
	m_dest = dest ;
}

int OutStreamBufAdaptor::sync( )
{
	return BufferOut( ) ;
}

int OutStreamBufAdaptor::BufferOut( )
{
	int cnt = pptr() - pbase() ;
	int retval = m_dest->Write(reinterpret_cast<unsigned char*>( m_buf ), cnt) ;

	pbump( -cnt ) ;
	return retval ;
}

OutStreamBufAdaptor::int_type OutStreamBufAdaptor::overflow( int_type c )
{
	if ( BufferOut() < 0 )
		return traits_type::eof( ) ;
	else
	{
		if ( !traits_type::eq_int_type( c, traits_type::eof() ) )
			return sputc( c ) ;
		else
			return traits_type::not_eof( c ) ;
	}
}

std::streamsize OutStreamBufAdaptor::xsputn(
	const char_type *s, std::streamsize n )
{
	if ( n < epptr() - pptr() )
	{
		std::memcpy( pptr(), s, n * sizeof(char_type) ) ;
		pbump( n ) ;
	}
	else
	{
		for ( std::streamsize i = 0 ; i < n ; i++ )
		{
			if ( traits_type::eq_int_type( sputc(s[i]), traits_type::eof() ) )
				return i ;
		}
	}
	return n ;
}

} // end of namespace
