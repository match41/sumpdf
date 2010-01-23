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
	\file	DeflateFilter.cc
	\brief	implementation the DeflateFilter class
	\date	Wed Mar 4 2009
	\author	Nestal Wan
*/

#include "DeflateFilter.hh"

#include "StreamError.hh"

#include "core/Array.hh"
#include "core/Name.hh"

#include <boost/format.hpp>

#include <cassert>
#include <cstring>
#include <iostream>

namespace pdf {

DeflateFilter::Error::Error( const char *func, const char *msg )
	: Exception( boost::format( "%1% error: %2%" ) % msg )
{
}

DeflateFilter::DeflateFilter( std::auto_ptr<StreamFilter> src )
	: m_src( src )
{
	assert( m_src.get() != 0 ) ;
	std::memset( &m_decomp.z,	0, sizeof(m_decomp.z) ) ;
	std::memset( &m_comp.z, 	0, sizeof(m_comp.z) ) ;

	if ( ::inflateInit( &m_decomp.z ) != Z_OK )
		throw Error( "inflateInit(): ", m_decomp.z.msg ) ;
	if ( ::deflateInit( &m_comp.z, 9 ) != Z_OK )
		throw Error( "deflateInit(): ", m_decomp.z.msg ) ;

    m_decomp.buf.reserve( m_buf_size ) ;
	m_comp.buf.resize( m_buf_size ) ;
}

DeflateFilter::~DeflateFilter( )
{
	deflateEnd( &m_comp.z ) ;
	inflateEnd( &m_decomp.z ) ;
}

std::size_t DeflateFilter::Read( unsigned char *data, std::size_t size )
{
	assert( data != 0 ) ;
	assert( size > 0 ) ;
	assert( m_decomp.z.avail_in <= m_decomp.buf.size() ) ;
	
	int result = Z_OK ;
	std::size_t offset = 0 ;
	
	do
	{
	    if ( m_decomp.z.avail_in == 0 )
	    {
			m_decomp.buf.resize( m_buf_size ) ;
			m_decomp.buf.resize( m_src->Read( &m_decomp.buf[0], m_buf_size ) ) ;

		    // no more input to be read. return number of byte read so far
		    if ( m_decomp.buf.empty( ) )
			    break ;
		    
		    m_decomp.z.next_in	= &m_decomp.buf[0] ;
		    m_decomp.z.avail_in	= m_decomp.buf.size( ) ;
		}

		assert( m_decomp.z.avail_in <= m_decomp.buf.size() ) ;
		assert( m_decomp.z.next_in  != 0 ) ;
		
		m_decomp.z.next_out		= data + offset ;
		m_decomp.z.avail_out	= size - offset ;
		result = ::inflate( &m_decomp.z, Z_SYNC_FLUSH ) ;
		
		// finish filling up the output data
		if ( result == Z_OK || result == Z_STREAM_END )
		{
			// update offset
			offset = size - m_decomp.z.avail_out ;
		
			if ( m_decomp.z.avail_out == 0 || result == Z_STREAM_END )
				break ;
		}
		else
		{
			throw StreamError(
				"inflate() error: " +
				( m_comp.z.msg != 0  ? std::string( m_comp.z.msg ) : "" ) ); 
		}
	} while ( result == Z_OK && m_decomp.z.avail_in == 0 ) ;

	return offset ;
}

std::size_t DeflateFilter::Write( const unsigned char *data, std::size_t size )
{
	int 		result = Z_OK ;
	std::size_t	offset = 0 ;

	m_is_need_flush	= true ;

	assert( m_comp.z.avail_in == 0 ) ;
	m_comp.z.next_in	= const_cast<unsigned char*>( data ) ;
	m_comp.z.avail_in	= size ;

	// TODO: this loop is not finished yet
	while ( result == Z_OK && m_comp.z.avail_in > 0 )
	{
		m_comp.z.next_out	= &m_comp.buf[0] ;
		m_comp.z.avail_out	= m_buf_size ;
		
		result = ::deflate( &m_comp.z, Z_NO_FLUSH ) ;
		if ( result == Z_OK )
		{
			// write out the data compressed by deflate()
			m_src->Write( &m_comp.buf[0], m_comp.z.next_out - &m_comp.buf[0] ) ;
			
			// deflate() has used up all its output space. we must consume its
			// output and call it again
			if ( m_comp.z.avail_out == 0 )
			{
				m_comp.z.next_out	= &m_comp.buf[0] ;
				m_comp.z.avail_out	= m_buf_size ;
			}
		}
		else
			throw StreamError(
				boost::format( "deflate() error: %1%" ) % m_comp.z.msg ); 
	}
	
	return offset ;
}

void DeflateFilter::Flush( )
{
	assert( m_comp.z.avail_in == 0 ) ;

	if ( m_is_need_flush )
	{
		m_comp.z.next_out	= &m_comp.buf[0] ;
		m_comp.z.avail_out	= m_buf_size ;
	
		int r ;
		while ( (r = ::deflate( &m_comp.z, Z_FINISH )) != Z_STREAM_END )
		{
			// write out the data compressed by deflate()
			m_src->Write( &m_comp.buf[0], m_comp.z.next_out - &m_comp.buf[0] ) ;
			
			m_comp.z.next_out	= &m_comp.buf[0] ;
			m_comp.z.avail_out	= m_buf_size ;
		}
		
		if ( r == Z_STREAM_END )
			m_src->Write( &m_comp.buf[0], m_comp.z.next_out - &m_comp.buf[0] ) ;
	
		m_is_need_flush = false ;
	}
	::deflateReset( &m_comp.z ) ;
}

void DeflateFilter::Rewind( )
{
	m_decomp.buf.clear( ) ;
	m_src->Rewind( ) ;

	// inflateReset() will clear this for us
	m_decomp.z.avail_in	= 0 ;
	m_decomp.z.next_in	= 0 ;

	using boost::format ;
	if ( ::inflateReset( &m_decomp.z ) != Z_OK )
		throw ParseError( format("inflateReset() error: %1%") % m_decomp.z.msg); 
}

std::size_t DeflateFilter::Length( ) const
{
	// TODO: verify if there is any cached data in DeflateFilter
	return m_src->Length( ) ;
}

DeflateFilter* DeflateFilter::Clone( ) const
{
	return new DeflateFilter( std::auto_ptr<StreamFilter>(m_src->Clone()) ) ;
}

Object DeflateFilter::GetFilterName( ) const
{
	Object name = m_src->GetFilterName( ) ;
	if ( name.Is<Array>( ) )
	{
		Array arr ;
		arr.push_back( Name( "FlateDecode" ) ) ;
		
		const Array& old = name.As<Array>( ) ;
		arr.insert( arr.end(), old.begin(), old.end() ) ;
		return Object( arr ) ;
	}
	else if ( name.Is<void>() )
		return Name( "FlateDecode" ) ;
	else
		throw Exception( "invalid filter" ) ;
}

StreamFilter* DeflateFilter::GetInner( )
{
	return m_src.get() ;
}

} // end of namespace
