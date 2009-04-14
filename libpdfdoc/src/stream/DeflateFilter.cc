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
	\file	DeflateFilter.cc
	\brief	implementation the DeflateFilter class
	\date	Wed Mar 4 2009
	\author	Nestal Wan
*/

#include "DeflateFilter.hh"

#include "util/Exception.hh"

#include <boost/format.hpp>

#include <cassert>
#include <cstring>
#include <iostream>

namespace pdf {

DeflateFilter::Error::Error( const char *func, const char *msg )
	: Exception( boost::format( "%1% error: %2%" ) % msg )
{
}

DeflateFilter::DeflateFilter( StreamFilter *src )
	: m_src( src )
{
	assert( m_src.get() != 0 ) ;
	std::memset( &m_decomp.str, 0, sizeof(m_decomp.str) ) ;
	std::memset( &m_comp.str, 0, sizeof(m_comp.str) ) ;

	if ( ::inflateInit( &m_decomp.str ) != Z_OK )
		throw Error( "inflateInit(): ", m_decomp.str.msg ) ;
	if ( ::deflateInit( &m_comp.str, 9 ) != Z_OK )
		throw Error( "deflateInit(): ", m_decomp.str.msg ) ;

    m_decomp.buf.reserve( m_buf_size ) ;
    m_comp.buf.reserve( m_buf_size ) ;
}

std::size_t DeflateFilter::Read( unsigned char *data, std::size_t size )
{
	int result = Z_OK ;
	std::size_t offset = 0 ;
	
	do
	{
	    if ( m_decomp.str.avail_in == 0 )
	    {
			m_decomp.buf.resize( m_buf_size ) ;
			m_decomp.buf.resize( m_src->Read( &m_decomp.buf[0], m_buf_size ) ) ;
		    
		    // no more input to be read. return number of byte read so far
		    if ( m_decomp.buf.empty( ) )
			    break ;
		    
		    m_decomp.str.next_in	= &m_decomp.buf[0] ;
		    m_decomp.str.avail_in	= m_decomp.buf.size( ) ;
		}
		
		m_decomp.str.next_out	= data + offset ;
		m_decomp.str.avail_out	= size - offset ;
		result = ::inflate( &m_decomp.str, Z_SYNC_FLUSH ) ;
		
		// finish filling up the output data
		if ( result == Z_OK || result == Z_STREAM_END )
		{
			// update offset
			offset = size - m_decomp.str.avail_out ;
		
			if ( m_decomp.str.avail_out == 0 )
				break ;
		}
	} while ( result == Z_OK && m_decomp.str.avail_in == 0 ) ;

	return offset ;
}

std::size_t DeflateFilter::Write( const unsigned char *data, std::size_t size )
{
	int result = Z_OK ;
	std::size_t offset = 0 ;
	
	assert( m_comp.str.avail_in == 0 ) ;
	m_comp.str.next_in	= const_cast<unsigned char*>( data ) ;
	m_comp.str.avail_in	= size ;
	
	while ( result == Z_OK && m_decomp.str.avail_in > 0 )
	{
		break ;
	}

	return offset ;
}

void DeflateFilter::Reset( )
{
	m_decomp.buf.clear( ) ;
	m_src->Reset( ) ;

	if ( ::inflateReset( &m_decomp.str ) != Z_OK )
		throw ParseError( "inflate init fail" ) ;
}

} // end of namespace
