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

#include <cassert>
#include <cstring>
#include <iostream>

namespace pdf {

DeflateFilter::DeflateFilter( StreamFilter *src )
	: m_src( src )
{
	assert( m_src != 0 ) ;
	std::memset( &m_zstr, 0, sizeof(m_zstr) ) ;

	if ( ::inflateInit( &m_zstr ) != Z_OK )
		throw ParseError( "inflate init fail" ) ;
}

DeflateFilter::~DeflateFilter( )
{
}

std::size_t DeflateFilter::Read( unsigned char *data, std::size_t size )
{
	int result = Z_OK ;
	std::size_t offset = 0 ;
	
	do
	{
	    if ( m_zstr.avail_in == 0 )
	    {
		    ReadFromSrc( ) ;
		    // no more input to be read. return number of byte read so far
		    if ( m_buf.empty( ) )
			    break ;
		    
		    m_zstr.next_in	= &m_buf[0] ;
		    m_zstr.avail_in	= m_buf.size( ) ;
		}
		
		m_zstr.next_out		= data + offset ;
		m_zstr.avail_out	= size - offset ;
		result = ::inflate( &m_zstr, Z_SYNC_FLUSH ) ;
		
		// finish filling up the output data
		if ( result == Z_OK || result == Z_STREAM_END )
		{
			// update offset
			offset = size - m_zstr.avail_out ;
		
			// the input is consumed, so erase them
//			m_buf.erase( m_buf.begin( ), m_buf.end( ) - m_zstr.avail_in ) ;
			
			if ( m_zstr.avail_out == 0 )
				break ;
		}
	} while ( result == Z_OK && m_zstr.avail_in == 0 ) ;

	return offset ;
}

std::size_t DeflateFilter::Write( const unsigned char *data, std::size_t size )
{
	return 0 ;
}

void DeflateFilter::ReadFromSrc( )
{
	std::size_t original = m_buf.size( ) ;
	if ( m_buf.size( ) <= m_buf_size )
		m_buf.resize( m_buf_size ) ;

	std::size_t count = m_src->Read( &m_buf[original], m_buf_size - original ) ;
	m_buf.resize( original + count ) ;
}

void DeflateFilter::Reset( )
{
	m_buf.clear( ) ;
	m_src->Reset( ) ;

	if ( ::inflateReset( &m_zstr ) != Z_OK )
		throw ParseError( "inflate init fail" ) ;
}

} // end of namespace
