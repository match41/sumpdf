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
	\file	Stream.cc
	\brief	implementation the Stream class
	\date	Thu Mar 13 2008
	\author	Nestal Wan
*/

#include "Stream.hh"

#include "BufferedFilter.hh"
#include "DeflateFilter.hh"
#include "MockStreamFilter.hh"
#include "RawFilter.hh"
#include "InStreamBufAdaptor.hh"
#include "OutStreamBufAdaptor.hh"

#include "core/Array.hh"
#include "core/Dictionary.hh"

#include "util/Exception.hh"

#include <boost/bind.hpp>

#include <algorithm>
#include <cassert>
#include <cstring>
#include <iostream>
#include <iterator>

#include <zlib.h>

namespace pdf {

struct Stream::Impl
{
	Dictionary					self ;
	std::auto_ptr<StreamFilter>	filter ;
	InStreamBufAdaptor			inbuf ;		///< for reading
	OutStreamBufAdaptor			outbuf ;	///< for writing
	
	/// indicates the stream's content is different from the data on disk.
	/// if true, the stream has to be rewritten to disk.
	bool	dirty ;
	
	Impl( ) : dirty( false )
	{
	}
} ;

Stream::Stream( Filter f )
	: m_impl( new Impl )
{
	// in memory stream
	m_impl->dirty = true ;

	StreamFilter *filter = new BufferedFilter ; 

	if ( f == deflate )
		filter = new DeflateFilter( std::auto_ptr<StreamFilter>(filter) ) ;

	m_impl->filter.reset( filter ) ;
	InitFilter( ) ;
}

Stream::Stream( const std::string& str )
	: m_impl( new Impl )
{
	// in memory stream
	m_impl->dirty = true ;

	m_impl->filter.reset( new BufferedFilter(str.begin(), str.end() ) ) ;
	InitFilter( ) ;
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
	assert( dict["Length"].Is<int>() ) ;

	m_impl->self = dict ;
	m_impl->filter.reset( new RawFilter( file, offset, dict["Length"] ) ) ;

	ApplyFilter( dict["Filter"] ) ;
	assert( m_impl->filter->GetInner()->Length() == dict["Length"] ) ;
	assert( dict["Filter"] == m_impl->filter->GetFilterName() ) ;
	InitFilter( ) ;
	
	m_impl->self.erase( "Length" ) ;
	m_impl->self.erase( "Filter" ) ;
}
	
/**	Constructor to initialize with an existing buffer without memory copying.
	\param	data	memory buffer. It will be swap()'ed to an internal buffer.
	\param	filter	filter object. It can be an array of PDF names to specify
					a serious of filters to be applied on the data; or
					a single PDF name of filter; or null object if no filter.
*/
Stream::Stream( std::vector<unsigned char>& data, const Object& filter )
	: m_impl( new Impl )
{
	// in memory stream
	m_impl->dirty = true ;

	m_impl->filter.reset( new BufferedFilter( data ) ) ;
	ApplyFilter( filter ) ;
	InitFilter( ) ;
	
	assert( filter == m_impl->filter->GetFilterName() ) ;
}

/**	empty destructor is required for shared_ptr.
*/
Stream::~Stream( )
{
}

void Stream::InitFilter( )
{
	m_impl->inbuf.Set( m_impl->filter.get() ) ;
	m_impl->outbuf.Set( m_impl->filter.get() ) ;
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
}

Dictionary Stream::Self( ) const
{
	assert( m_impl.get() != 0 ) ;
	
	Dictionary dict = m_impl->self ;
	dict["Length"]	= Length( ) ;
	Object filter	= m_impl->filter->GetFilterName( ) ;
	if ( !filter.IsNull() )
		dict["Filter"]	= filter ;
		
	return dict ;
}

/**	This function returns the length after applying all filters. It is equal
	to the number of bytes written to the file. It is not the same as the
	number of bytes you can read from it.
*/
std::size_t Stream::Length( ) const
{
	assert( m_impl.get() != 0 ) ;

	return m_impl->filter->Length( ) ;
}

void Stream::Swap( Stream& str )
{
	std::swap( m_impl, str.m_impl ) ;
}

void Stream::CreateFilter( const Name& filter )
{
	if ( filter == Name( "FlateDecode" ) )
		m_impl->filter.reset( new DeflateFilter( m_impl->filter ) ) ;
	else
		m_impl->filter.reset( new MockStreamFilter( m_impl->filter, filter ) ) ;
}

Stream Stream::Clone( ) const
{
	assert( m_impl.get( ) != 0 ) ;
	assert( m_impl->filter.get() != 0 ) ;

	std::vector<unsigned char> buf( Length() ) ;
	CopyData( &buf[0], buf.size() ) ;
	return Stream( buf, m_impl->filter->GetFilterName( ) ) ;
}

bool Stream::operator==( const Stream& str ) const
{
	assert( m_impl.get( ) != 0 ) ;
	assert( str.m_impl.get( ) != 0 ) ;
	
	return m_impl.get() == str.m_impl.get() ;
}

bool Stream::operator!=( const Stream& str ) const
{
	return !operator==( str ) ;
}

/*! write filtered stream data to a streambuf. This function will read all data
    from the stream and write them to the streambuf \a buf.
    \param  buf     the streambuf that will get all the stream data.
    \return	number of bytes copied to output stream. As this function copies
			filtered data, the return value may be different from Length().
*/
std::size_t Stream::CopyData( std::streambuf *buf ) const
{
    assert( buf != 0 ) ;
	assert( m_impl.get() != 0 ) ;
	assert( m_impl->filter.get() != 0 ) ;

	// first reset to the start of the stream
	m_impl->filter->Rewind( ) ;

	return CopyFromFilter( m_impl->filter.get(), buf ) ;
}

std::size_t Stream::CopyData( unsigned char *buf, std::size_t size ) const
{
    assert( buf != 0 ) ;
	assert( m_impl.get() != 0 ) ;
	assert( m_impl->filter.get() != 0 ) ;

	// first reset to the start of the stream
	m_impl->filter->Rewind( ) ;

	return m_impl->filter->Read( buf, size ) ;
}

/**	copy the unfiltered data to the output streambuf.
	\param	buf		the output streambuf
	\return	the number of bytes copied to the output. It should be equal to
			the return value of Length().
*/
std::size_t Stream::CopyRawData( std::streambuf *buf ) const
{
    assert( buf != 0 ) ;
	assert( m_impl.get() != 0 ) ;
	assert( m_impl->filter.get() != 0 ) ;

	// first reset to the start of the stream
	m_impl->filter->Rewind( ) ;

	return CopyFromFilter( m_impl->filter->GetInner(), buf ) ;
}

std::size_t Stream::CopyFromFilter( StreamFilter *f, std::streambuf *buf )
{
	assert( f != 0 ) ;
	assert( buf != 0 ) ;
	
	// rewind to the start of the stream
	f->Rewind( ) ;
	
	unsigned char data[80] ;
	std::size_t count = f->Read( data, sizeof(data) ) ;
	std::size_t total = 0 ;

	while ( count > 0 )
	{
		total += buf->sputn( reinterpret_cast<const char*>( data ), count ) ;
		count = f->Read( data, sizeof(data) ) ;
	}

	return total ;
}

std::ostream& operator<<( std::ostream& os, const Stream& s )
{
	assert( s.m_impl.get() != 0 ) ;
	assert( s.m_impl->filter.get() != 0 ) ;

	// first flush all buffered data inside the filters
	s.m_impl->filter->Flush( ) ;
	
	os 	<< s.Self( ) << "\nstream\n" ;

	std::size_t length = s.CopyRawData( os.rdbuf() ) ;
	assert( length == static_cast<std::size_t>(s.Self( )["Length"].As<int>() ));
	
	return os << "\nendstream\n" ;
}

std::streambuf* Stream::InStreamBuf( )
{
	assert( m_impl.get() != 0 ) ;
	return &m_impl->inbuf ;
}

std::streambuf* Stream::OutStreamBuf( )
{
	assert( m_impl.get() != 0 ) ;
	assert( m_impl->dirty ) ;
	return &m_impl->outbuf ;
}

void Stream::Rewind( ) const
{
	assert( m_impl.get() != 0 ) ;
	m_impl->filter->Rewind( ) ;
}

Name Stream::Type( ) const
{
	assert( m_impl.get() != 0 ) ;
	return m_impl->self["Type"] ;
}

Name Stream::Subtype( ) const
{
	assert( m_impl.get() != 0 ) ;
	return m_impl->self["Subtype"] ;
}

std::size_t Stream::Append( const unsigned char *buf, std::size_t size )
{
	assert( m_impl.get() != 0 ) ;
	assert( m_impl->filter.get() != 0 ) ;
	assert( m_impl->dirty ) ;
	
	return m_impl->filter->Write( buf, size ) ;
}

std::size_t Stream::Append( const char *str )
{
	return Append( reinterpret_cast<const unsigned char*>(str),
	               std::strlen( str ) ) ;
}

bool Stream::IsDirty( ) const
{
	assert( m_impl.get() != 0 ) ;
	return m_impl->dirty ;
}

void Stream::Flush( )
{
	assert( m_impl.get() != 0 ) ;
	assert( m_impl->filter.get() != 0 ) ;
	assert( m_impl->dirty ) ;
	
	m_impl->filter->Flush( ) ;
}

} // end of namespace

namespace std
{
	void swap( pdf::Stream& s1, pdf::Stream& s2 )
	{
		s1.Swap( s2 ) ;
	}
}
