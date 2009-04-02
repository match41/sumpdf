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

#include "Array.hh"
#include "Dictionary.hh"
#include "filter/BufferedFilter.hh"
#include "filter/DeflateFilter.hh"
#include "filter/RawFilter.hh"
#include "filter/StreamFilter.hh"
#include "filter/StreamBufAdaptor.hh"
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
	Dictionary					self ;
	std::auto_ptr<StreamFilter>	filter ;
	StreamBufAdaptor			sbuf ;
	std::istream				istr ;
	
	Impl( ) : istr( &sbuf )
	{
	}
} ;

Stream::Stream( const std::string& str )
	: m_impl( new Impl )
{
	m_impl->filter.reset( new BufferedFilter(str) ) ;
	m_impl->sbuf.Set( m_impl->filter.get() ) ;
}

Stream::Stream( std::streambuf *file, std::streamoff offset,
	            const Dictionary& dict )
	: m_impl( new Impl )
{
	assert( file != 0 ) ;
	assert( dict.find( "Length" ) != dict.end() ) ;
	assert( dict["Length"].IsType<int>() ) ;

	m_impl->self = dict ;
	m_impl->filter.reset( new RawFilter( file, offset,
	                                     dict["Length"].As<int>() ) ) ;

	const Object& filter = dict["Filter"] ;
	if ( filter.Type() == Object::array )
	{
		const Array& filters = filter.As<Array>() ;
		std::for_each( filters.begin( ), filters.end( ),
		               boost::bind( &Stream::CreateFilter, this, _1 ) ) ;
	}
	else if ( filter.Type() == Object::name )
		CreateFilter( filter ) ;

	m_impl->sbuf.Set( m_impl->filter.get() ) ;
}

Stream::~Stream( )
{
}

void Stream::Swap( Stream& str )
{
	std::swap( m_impl, str.m_impl ) ;
}

void Stream::CreateFilter( const Name& filter )
{
	if ( filter == Name( "FlateDecode" ) )
	{
		StreamFilter *sf = m_impl->filter.release( ) ;
		m_impl->filter.reset( new DeflateFilter( sf ) ) ;
	}
}

/*!	\brief	write the stream with an indirect length field

	This function is different from the operator<<() in which it writes the
	"Length" field in the stream dictionary as an indirect object. It is
	because normally for streams with filters the length of the stream is not
	known until after the stream content is written. However, the stream
	dictionary, which contains the length field, is written \e before the
	actual stream content. Therefore, when writing the stream dictionary, as
	the length is still unknown, this function writes an indirect object and
	returns the stream content length. The caller should write the returned
	length to the indirect object it just specified, i.e. \a length_ref .
	\param	os			the output to be written to
	\param	length_ref	the indirect object (or link) of the length field.
						caller should write the returned value (i.e. the
						length) to this link.
	\return	the actual number of bytes written to file for stream content,
			i.e. the length of filtered stream content. stream dictionary is
			not included. 
*/
std::size_t Stream::Write( std::ostream& os, const Ref& length_ref ) const
{
	Dictionary dict( m_impl->self ) ;
	dict["Length"]	= length_ref ;
	os << dict ;

	os << "\nstream\n" ;
	std::size_t count = ReadAll( os.rdbuf() ) ;
	os << "endstream" ;
	
	return count ;
}
bool Stream::operator==( const Stream& str ) const
{
	assert( m_impl.get( ) != 0 ) ;
	assert( str.m_impl.get( ) != 0 ) ;
	
	return m_impl.get() == str.m_impl.get() ;
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

std::size_t Stream::ReadAll( std::streambuf *buf ) const
{
    assert( buf != 0 ) ;
	assert( m_impl.get() != 0 ) ;
	assert( m_impl->filter.get() != 0 ) ;

	unsigned char data[80] ;
	std::size_t count = m_impl->filter->Read( data, sizeof(data) ) ;
	std::size_t total = 0 ;
	
	while ( count > 0 )
	{
		total += buf->sputn( reinterpret_cast<const char*>( data ), count ) ;
		count = m_impl->filter->Read( data, sizeof(data) ) ;
	}

	return total ;
}

std::istream& Stream::InStream( )
{
	return m_impl->istr ;
}

} // end of namespace

namespace std
{
	void swap( pdf::Stream& s1, pdf::Stream& s2 )
	{
		s1.Swap( s2 ) ;
	}
}
