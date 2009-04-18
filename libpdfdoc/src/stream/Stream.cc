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

#include "BufferedFilter.hh"
#include "DeflateFilter.hh"
#include "RawFilter.hh"
#include "StreamFilter.hh"
#include "StreamBufAdaptor.hh"

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
	m_impl->filter.reset( new BufferedFilter(str.begin(), str.end() ) ) ;
	m_impl->sbuf.Set( m_impl->filter.get() ) ;
}

/*!	constructor for streams from file. This constructor will create a stream
	whose content is about to be read from file. Data will be actually read
	on-demand. Therefore, this function will actually read nothing but just
	remember the file and the position in the file to be read from.
	\param	file	the file that the stream data will be read from
	\param	offset	the position of the file that the stream data will
					be read from
	\param	dict	stream dictionary
*/
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

	ApplyFilter( dict["Filter"] ) ;
}
	
Stream::Stream( std::vector<unsigned char>& data, const Object& filter )
	: m_impl( new Impl )
{
	m_impl->filter.reset( new BufferedFilter( data ) ) ;
	ApplyFilter( filter ) ;
}

/*!	create the filters and chain them together. This function will chain up
	the filters. It will handle two types of PDF objects to represent filters:
	
	-	Name objects:	the name of the filter. e.g. "FlateDecode" for the
		deflate algorithm.
	
	-	Array objects:	each of the members in the array is treated to be
		a name of a filter, or another array. the filters will be applied
		in the same sequence as the array.
*/
void Stream::ApplyFilter( const Object& filter )
{
	assert( m_impl->filter.get() != 0 ) ;

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

bool Stream::operator==( const Stream& str ) const
{
	assert( m_impl.get( ) != 0 ) ;
	assert( str.m_impl.get( ) != 0 ) ;
	
	return m_impl.get() == str.m_impl.get() ;
}

const Dictionary& Stream::Dict( ) const
{
	assert( m_impl.get( ) != 0 ) ;
	return m_impl->self ;
}

Dictionary& Stream::Dict( )
{
	assert( m_impl.get( ) != 0 ) ;
	return m_impl->self ;
}

Dictionary Stream::MakeDictWithLength( const Ref& length_link ) const
{
	assert( m_impl.get( ) != 0 ) ;
	Dictionary self( m_impl->self ) ;
	self["Length"] = length_link ;
	return self ;
}

/*! write stream data to a streambuf. This function will read all data
    from the stream and write them to the streambuf \a buf.
    \param  buf     the streambuf that will get all the stream data.
*/
std::size_t Stream::WriteData( std::streambuf *buf ) const
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

std::istream& Stream::InStream( ) const
{
	return m_impl->istr ;
}

void Stream::Reset( ) const
{
	m_impl->istr.clear( ) ;
	m_impl->filter->Reset( ) ;
}

Name Stream::Type( ) const
{
	return m_impl->self["Type"] ;
}

Name Stream::Subtype( ) const
{
	return m_impl->self["Subtype"] ;
}

} // end of namespace

namespace std
{
	void swap( pdf::Stream& s1, pdf::Stream& s2 )
	{
		s1.Swap( s2 ) ;
	}
}
