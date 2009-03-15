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
	\file	Stream.cc
	\brief	implementation the Stream class
	\date	Thu Mar 13 2008
	\author	Nestal Wan
*/

#include "Stream.hh"

#include "core/Array.hh"
#include "core/Dictionary.hh"
#include "util/Exception.hh"

#include <boost/bind.hpp>

#include <algorithm>
#include <cassert>
#include <iostream>
#include <iterator>

#include <zlib.h>

namespace pdf {

struct Stream::Impl
{
	std::vector<unsigned char>	bytes ;
	Dictionary					self ;
} ;

Stream::Stream( )
	: m_impl( new Impl )
{
}

/*!	Non-copying constructor. This contructor will not copy the data specified
	by its argument. It will use std::vector<>::swap() to assign it to its
	members.
	\param	data	data to be swapped.
	\param	dict	dictionary of the stream.
*/
Stream::Stream( std::vector<unsigned char>& data, const Dictionary& dict )
	: m_impl( new Impl )
{
	m_impl->bytes.swap( data ) ;
	m_impl->self = dict ;

	const Object& filter = dict["Filter"] ;
	if ( filter.Type() == Object::array )
	{
		const Array& filters = filter.As<Array>() ;
		std::for_each( filters.begin( ), filters.end( ),
		               boost::bind( &Stream::ApplyFilter, this, _1 ) ) ;
	}
	else if ( filter.Type() == Object::name )
		ApplyFilter( filter ) ;
	
	else if ( filter != Null() )
		throw ParseError( "filter error" ) ;

	// these two fields will be generated again when writing.
	m_impl->self.erase( "Filter" ) ;
	m_impl->self.erase( "Length" ) ;
}

Stream::Stream( const std::string& str )
	: m_impl( new Impl )
{
	m_impl->bytes.assign( str.begin( ), str.end( ) ) ;
}

Stream::~Stream( )
{
}

void Stream::Swap( Stream& str )
{
	std::swap( m_impl, str.m_impl ) ;
}

void Stream::ApplyFilter( const Name& filter )
{
	if ( filter == Name( "FlateDecode" ) )
		Inflate( ) ;
}

void Stream::Inflate( )
{
	::z_stream z = {} ;
	z.next_in	= &m_impl->bytes[0] ;
	z.avail_in	= m_impl->bytes.size( ) ;
	if ( ::inflateInit( &z ) != Z_OK )
		throw ParseError( "inflate init fail" ) ;
	
	std::vector<unsigned char> output ;
	const std::size_t block_size = 100 ;
	
	int result = Z_OK ;
	while ( result == Z_OK && z.avail_out == 0 )
	{
		std::size_t ori_size = output.size() ; 
		output.resize( ori_size + block_size ) ;
		z.next_out	= &output[ori_size] ;
		z.avail_out	= block_size ;
		result = ::inflate( &z, Z_SYNC_FLUSH ) ;
	}
	
	if ( result != Z_STREAM_END )
		throw ParseError( "zlib error: " + std::string(z.msg) ) ;

	output.resize( output.size() - z.avail_out ) ;

	m_impl->bytes.swap( output ) ;
}

std::ostream& operator<<( std::ostream& os, const Stream& b )
{
	assert( b.m_impl.get( ) != 0 ) ;
/*
	std::vector<unsigned char> c( ::compressBound( b.m_impl->bytes.size( ) ) ) ;
	::uLongf dest_len = c.size( ) ;
	::compress2( &c[0], &dest_len, &b.m_impl->bytes[0], b.m_impl->bytes.size(
), 9 ) ;
	c.resize( dest_len ) ;
*/
	const std::vector<unsigned char>& c = b.m_impl->bytes ;

	Dictionary dict( b.m_impl->self ) ;
	dict["Length"]	= c.size( ) ;
// 	dict["Filter"]	= Name( "FlateDecode" ) ;
	os << dict ;

	os << "\nstream\n" ;
	std::copy( c.begin( ), c.end( ),
	           std::ostreambuf_iterator<char>( os ) ) ;
	os << "endstream" ;
	
	return os ;
}

const unsigned char* Stream::Data( ) const
{
	assert( m_impl.get( ) != 0 ) ;
	return m_impl.get() != 0 ? (!m_impl->bytes.empty() ? &m_impl->bytes[0] : 0)
	                         : 0 ;
}

std::size_t Stream::Size( ) const
{
	assert( m_impl.get( ) != 0 ) ;
	return m_impl.get() != 0 ? m_impl->bytes.size() : 0 ;
}

bool Stream::operator==( const Stream& str ) const
{
	assert( m_impl.get( ) != 0 ) ;
	assert( str.m_impl.get( ) != 0 ) ;
	
	return m_impl->bytes == str.m_impl->bytes ;
}

const Dictionary& Stream::GetDictionary( ) const
{
	assert( m_impl.get( ) != 0 ) ;
	return m_impl->self ;
}

Dictionary& Stream::GetDictionary( )
{
	assert( m_impl.get( ) != 0 ) ;
	return m_impl->self ;
}

bool Stream::IsEmpty( ) const
{
	return m_impl->bytes.empty( ) ;
}

} // end of namespace
