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
	\file	TrueTypeTable.cc
	\brief	implementation the TrueTypeTable class
	\date	Sat Mar 7 2009
	\author	Nestal Wan
*/

#include "TrueTypeTable.hh"

#include "util/Exception.hh"

#include <iostream>
#include <streambuf>
#include <cassert>

namespace font {

TrueTypeTable::TrueTypeTable( std::streambuf *file )
	: m_file( file ), m_checksum( 0 ), m_length( 0 ), m_offset( 0 ),
	  m_temp_long( 0 )
{
}

TrueTypeTable::TrueTypeTable( std::streambuf *file, std::streamoff offset,
				              std::size_t length )
	: m_file( file ), m_checksum( 0 ), m_length( length ), m_offset( offset ),
	  m_temp_long( 0 )
{
}

template <typename T>
TrueTypeTable& operator>>( TrueTypeTable& bs, T& val )
{
	val = 0 ;
	std::size_t s = sizeof( T ) ;
	while (s > 0)
	{
		int c = bs.m_file->sbumpc() ;
		if ( c == std::streambuf::traits_type::eof() )
			throw pdf::Exception( "unexpected EOF" ) ;
		
		val = (val << 8) + c;
		s--;
	}
	
	return bs ;
}

template <typename T>
TrueTypeTable& operator<<( TrueTypeTable& bs, T val )
{
    char buf[sizeof(T)+1], *p = buf;
    std::size_t s = sizeof( val ) ;
    while ( s-- > 0 )
    {
        *p++ = static_cast<char>(val) & 0xFF;
        val >>= 8;
    }
    p--;
    while ( p >= buf )
        bs.PutByte( *p--) ;
    
    return bs ;
}

unsigned char TrueTypeTable::AddChecksum( unsigned char c )
{
	m_temp_long = (m_temp_long << 8) + c ;
	m_length++;
	if ( m_length % sizeof(m_temp_long) == 0 )
	{
		m_checksum += m_temp_long ;
		m_temp_long = 0;
	}
	return c ;
}

void TrueTypeTable::EndStream( )
{
	// save original length for later
	std::size_t original = m_length ;

	// pad zeros to make it 4-byte aligned
	while ( m_length % 4 != 0 )
		PutByte( 0 ) ;
	assert( m_length % 4 == 0 ) ;
	
	// restore original length because padding is not included in length
	m_length = original ;
}

void TrueTypeTable::PutByte( unsigned char byte )
{
	assert( m_file != 0 ) ;
	m_file->sputc( AddChecksum( byte ) ) ;
}

std::size_t TrueTypeTable::Write( const unsigned char *data, std::size_t size )
{
	assert( data != 0 ) ;
	assert( size > 0 ) ;
	
	for ( const unsigned char *p = data ; p != data + size ; p++ )
		PutByte( *p ) ;
	
	return size ;
}

std::size_t TrueTypeTable::Copy( std::streambuf *src, std::size_t size )
{
	std::size_t i = 0 ;
	for ( ; i < size ; i++ )
	{
		std::streambuf::int_type c = src->sbumpc() ;
		if ( c != std::streambuf::traits_type::eof() )
			PutByte( std::streambuf::traits_type::to_char_type(c) ) ;
		else break ;
	}
	assert( m_length >= i ) ;
	return i ;
}

/*!	Get the number of bytes written to file so far. Since EndStream() will
	write the padding bytes to file to make the table 4-byte algined, the
	value returned by Size() will be different before and after calling
	EndStream. If called before EndStream(), Size() will return the length
	of table without padding bytes.
	\return	Number of bytes written to file.
*/
std::size_t TrueTypeTable::Size( ) const
{
	return m_length ;
}

uint32_t TrueTypeTable::Checksum( ) const
{
	return m_checksum ;
}

template TrueTypeTable& operator>>( TrueTypeTable&, uint8_t& ) ;
template TrueTypeTable& operator>>( TrueTypeTable&, int8_t& ) ;
template TrueTypeTable& operator>>( TrueTypeTable&, uint16_t& ) ;
template TrueTypeTable& operator>>( TrueTypeTable&, int16_t& ) ;
template TrueTypeTable& operator>>( TrueTypeTable&, uint32_t& ) ;
template TrueTypeTable& operator>>( TrueTypeTable&, int32_t& ) ;
template TrueTypeTable& operator>>( TrueTypeTable&, boost::int64_t& ) ;

template TrueTypeTable& operator<<( TrueTypeTable&, uint8_t ) ;
template TrueTypeTable& operator<<( TrueTypeTable&, int8_t ) ;
template TrueTypeTable& operator<<( TrueTypeTable&, uint16_t ) ;
template TrueTypeTable& operator<<( TrueTypeTable&, int16_t ) ;
template TrueTypeTable& operator<<( TrueTypeTable&, uint32_t ) ;
template TrueTypeTable& operator<<( TrueTypeTable&, int32_t ) ;
template TrueTypeTable& operator<<( TrueTypeTable&, boost::int64_t ) ;

} // end of namespace
