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
#include "PredictFilter.hh"

#include "core/Array.hh"
#include "core/Dictionary.hh"

#include "util/Exception.hh"
#include "util/Debug.hh"

#include <boost/bind.hpp>

#include <algorithm>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <iomanip>

#include <zlib.h>

namespace pdf {

struct Stream::Data
{
	std::auto_ptr<StreamFilter>	filter ;
	InStreamBufAdaptor			inbuf ;		///< for reading
	OutStreamBufAdaptor			outbuf ;	///< for writing
	
	/// indicates the stream's content is different from the data on disk.
	/// if true, the stream has to be rewritten to disk.
	bool	dirty ;
	
	Data( ) : dirty( false )
	{
	}
	
	Data( const Data& data )
	: filter( data.filter->Clone() )
	{
		inbuf.Set( filter.get() ) ;
		outbuf.Set( filter.get() ) ;
	}
} ;

Stream::Stream( Filter f )
	: m_data( new Data )
{
	// in memory stream
	m_data->dirty = true ;

	StreamFilter *filter = new BufferedFilter ; 

	if ( f == deflate )
		filter = new DeflateFilter( std::auto_ptr<StreamFilter>(filter) ) ;

	m_data->filter.reset( filter ) ;
	InitFilter( ) ;
}

Stream::Stream( const std::string& str )
	: m_data( new Data )
{
	// in memory stream
	m_data->dirty = true ;

	m_data->filter.reset( new BufferedFilter(str.begin(), str.end() ) ) ;
	InitFilter( ) ;
}

Stream::Stream( const char *str )
: m_data( new Data )
{
	// in memory stream
	m_data->dirty = true ;
	
	m_data->filter.reset( new BufferedFilter( str ) ) ;
	InitFilter( ) ;
}

Stream::Stream( const Object& obj )
	: m_data( obj.As<Stream>().m_data )
{
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
: m_self( dict )
, m_data( new Data )
{
	PDF_ASSERT( file != 0 ) ;
	PDF_ASSERT( m_self.find( "Length" ) != m_self.end() ) ;
	PDF_ASSERT( m_self["Length"].Is<int>() ) ;

file->pubseekpos( offset, std::ios::in ) ;
std::cout << "char after seek = " << file->sgetc() << std::endl ;

	m_data->filter.reset( new RawFilter( file, offset, m_self["Length"] ) ) ;

	ApplyFilter( dict["Filter"], dict["DecodeParms"] ) ;

	PDF_ASSERT( m_data->filter->GetInner()->Length() == m_self["Length"] ) ;
	PDF_ASSERT( m_self["Filter"].Is<Array>() ||
		m_self["Filter"] == m_data->filter->NameChain() ) ;
	InitFilter( ) ;
	
	m_self.erase( "Length" ) ;
	m_self.erase( "Filter" ) ;
}
	
/**	Constructor to initialize with an existing buffer without memory copying.
	\param	data	memory buffer. It will be swap()'ed to an internal buffer.
	\param	filter	filter object. It can be an array of PDF names to specify
					a serious of filters to be applied on the data; or
					a single PDF name of filter; or null object if no filter.
*/
Stream::Stream( std::vector<unsigned char>& data, const Object& filter )
	: m_data( new Data )
{
	// in memory stream
	m_data->dirty = true ;

	m_data->filter.reset( new BufferedFilter( data ) ) ;
	ApplyFilter( filter, Object::NullObj() ) ;
	InitFilter( ) ;
	
	PDF_ASSERT( filter == m_data->filter->NameChain() ) ;
}

/**	empty destructor is required for shared_ptr.
*/
Stream::~Stream( )
{
}

long Stream::UseCount( ) const
{
	return m_data.use_count() ;
}

void Stream::InitFilter( )
{
	m_data->inbuf.Set( m_data->filter.get() ) ;
	m_data->outbuf.Set( m_data->filter.get() ) ;
}

/*!	create the filters and chain them together. This function will chain up
	the filters. It will handle two types of PDF objects to represent filters:
	
	-	Name objects:	the name of the filter. e.g. "FlateDecode" for the
		deflate algorithm.
	
	-	Array objects:	each of the members in the array is treated to be
		a name of a filter, or another array. the filters will be applied
		in the same sequence as the array.

	Object types other than Name and Array will be ignored.
*/
void Stream::ApplyFilter( const Object& filter, const Object& parms )
{
	PDF_ASSERT( m_data->filter.get() != 0 ) ;

	if ( filter.Is<Array>() && (parms.Is<Array>() || parms.Is<void>()) )
	{
		const Array& filters	= filter.As<Array>() ;
		const Array& parms_arr	= parms.Is<Array>()
			? parms.As<Array>()
			: Array( filters.size() ) ;

		if ( filters.size() != parms_arr.size() )
			throw ParseError( "filter and parameters size mismatch" ) ;
		
		for ( std::size_t i = 0 ; i < filters.size() ; i++ )
			CreateFilter( filters[i], parms_arr[i] ) ;
	}
	else if ( filter.Is<Name>() && (parms.Is<Dictionary>() || parms.Is<void>()))
		CreateFilter( filter, parms ) ;
}

/**	\brief	Get the stream dictionary.

	This function returns the stream dictionary. The "Length" and "Filter"
	fields will be updated according to the data contained by the stream.
	In other words, Self() will return a different dictionary when it is called
	after modifying the stream.
	\return	The stream dictionary.
*/
Dictionary Stream::GetRawDict( ) const
{
	PDF_ASSERT( m_data.get() != 0 ) ;
	PDF_ASSERT( m_self.find( "Length" ) == m_self.end() ) ;
	PDF_ASSERT( m_self.find( "Filter" ) == m_self.end() ) ;
	
	Dictionary dict = m_self ;
	dict.insert( "Length", Length( ) ) ;
	Object filter	= m_data->filter->NameChain( ) ;
	if ( !filter.Is<void>() )
		dict.insert( "Filter", filter ) ;
		
	return dict ;
}

const Dictionary& Stream::Self( ) const
{
	PDF_ASSERT( m_self.find( "Length" ) == m_self.end() ) ;
	PDF_ASSERT( m_self.find( "Filter" ) == m_self.end() ) ;
	return m_self ;
}

Dictionary& Stream::Self( )
{
	PDF_ASSERT( m_self.find( "Length" ) == m_self.end() ) ;
	PDF_ASSERT( m_self.find( "Filter" ) == m_self.end() ) ;
	return m_self ;
}

/**	\brief	get raw length of the stream.
	
	This function returns the length after applying all filters. It is equal
	to the number of bytes written to the file. It is not the same as the
	number of bytes you can read from it.
	\return	raw length of the stream, after applying all filters.
*/
std::size_t Stream::Length( ) const
{
	PDF_ASSERT( m_data.get() != 0 ) ;

	return m_data->filter->Length( ) ;
}

/**	\brief	swap two streams	

	Swap the pointer to data for two streams. This function will not throw and
	will never fail.
*/
void Stream::Swap( Stream& str )
{
	std::swap( m_self, str.m_self ) ;
	std::swap( m_data, str.m_data ) ;
}

void Stream::CreateFilter( const Name& filter, const Object& parms )
{
	if ( filter == Name( "FlateDecode" ) )
	{
		m_data->filter.reset( new DeflateFilter( m_data->filter ) ) ;
		
		if ( parms.Is<Dictionary>() )
		{
			const Dictionary& dict = parms.As<Dictionary>() ;
			std::cout << "decode param is " << dict << std::endl ;
			
			if ( dict["Predictor"].Is<int>() &&
				 dict["Predictor"].As<int>() > 10 )
				m_data->filter.reset( new PredictFilter(
					m_data->filter, dict["Columns"].As<int>() ) ) ;
		}
	}
	else
		m_data->filter.reset( new MockStreamFilter( m_data->filter, filter ) ) ;
}

/// copy data only, without the dictionary
Stream Stream::Clone( ) const
{
	PDF_ASSERT( m_data.get( ) != 0 ) ;
	PDF_ASSERT( m_data->filter.get() != 0 ) ;

	std::vector<unsigned char> buf ;
	CopyData( buf ) ;
	return Stream( buf, Object::NullObj() ) ;
}

bool Stream::operator==( const Stream& str ) const
{
	PDF_ASSERT( m_data.get( ) != 0 ) ;
	PDF_ASSERT( str.m_data.get( ) != 0 ) ;
	
	return m_self == str.m_self && m_data.get() == str.m_data.get() ;
}

bool Stream::operator!=( const Stream& str ) const
{
	return !operator==( str ) ;
}

// just kidding
bool Stream::operator<( const Stream& str ) const
{
	return m_self < str.m_self ;
}

/*! write filtered stream data to a streambuf. This function will read all data
    from the stream and write them to the streambuf \a buf.
    \param  buf     the streambuf that will get all the stream data.
    \return	number of bytes copied to output stream. As this function copies
			filtered data, the return value may be different from Length().
*/
std::size_t Stream::CopyData( std::streambuf *buf ) const
{
    PDF_ASSERT( buf != 0 ) ;
	PDF_ASSERT( m_data.get() != 0 ) ;
	PDF_ASSERT( m_data->filter.get() != 0 ) ;

	// first reset to the start of the stream
	m_data->filter->Rewind( ) ;

	return CopyFromFilter( m_data->filter.get(), buf ) ;
}

std::size_t Stream::CopyData( unsigned char *buf, std::size_t size ) const
{
    PDF_ASSERT( buf != 0 ) ;
	PDF_ASSERT( m_data.get() != 0 ) ;
	PDF_ASSERT( m_data->filter.get() != 0 ) ;

	// first reset to the start of the stream
	m_data->filter->Rewind( ) ;

	return m_data->filter->Read( buf, size ) ;
}

void Stream::CopyData( std::vector<unsigned char>& buf ) const
{
	StreamFilter *f = m_data->filter.get() ;
	PDF_ASSERT( f != 0 ) ;
	
	// rewind to the start of the stream
	f->Rewind( ) ;
	
	const std::size_t buf_size = 80 ;
	std::size_t idx = 0 ;
	std::size_t count = 0 ;
	
	do
	{
		buf.resize( idx + buf_size ) ;
		count = f->Read( &buf[idx], buf_size ) ;
		idx += count ;
		buf.resize( idx ) ;
	} while ( count > 0 ) ;
}

/**	copy the unfiltered data to the output streambuf.
	\param	buf		the output streambuf
	\return	the number of bytes copied to the output. It should be equal to
			the return value of Length().
*/
std::size_t Stream::CopyRawData( std::streambuf *buf ) const
{
    PDF_ASSERT( buf != 0 ) ;
	PDF_ASSERT( m_data.get() != 0 ) ;
	PDF_ASSERT( m_data->filter.get() != 0 ) ;

	// first reset to the start of the stream
	m_data->filter->Rewind( ) ;

	return CopyFromFilter( m_data->filter->GetInner(), buf ) ;
}

std::size_t Stream::CopyFromFilter( StreamFilter *f, std::streambuf *buf )
{
	PDF_ASSERT( f != 0 ) ;
	PDF_ASSERT( buf != 0 ) ;
	
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

///	Write the stream to output stream.
/**	The stream will be written according to the PDF specification. Note that
	the "Length" field will never be indirect reference.
*/
std::ostream& operator<<( std::ostream& os, const Stream& s )
{
	PDF_ASSERT( s.m_data.get() != 0 ) ;
	PDF_ASSERT( s.m_data->filter.get() != 0 ) ;
	PDF_ASSERT( s.m_self.find( "Length" ) == s.m_self.end() ) ;
	PDF_ASSERT( s.m_self.find( "Filter" ) == s.m_self.end() ) ;

	// first flush all buffered data inside the filters
	s.m_data->filter->Flush( ) ;
	
	// rewind to start from the beginning
	s.Rewind( ) ;
	
	os 	<< s.GetRawDict() << "\nstream\n" ;
	
	std::size_t length = s.CopyRawData( os.rdbuf() ) ;

	PDF_ASSERT_EQUAL( length, s.GetRawDict()["Length"].To<std::size_t>() ) ;
	PDF_ASSERT_EQUAL( length, s.Length() ) ;
	
	return os << "\nendstream" ;
}

std::streambuf* Stream::InStreamBuf( )
{
	PDF_ASSERT( m_data.get() != 0 ) ;
	return &m_data->inbuf ;
}

std::streambuf* Stream::OutStreamBuf( )
{
	PDF_ASSERT( m_data.get() != 0 ) ;
	PDF_ASSERT( m_data->dirty ) ;
	
	CopyOnWrite( ) ;
	return &m_data->outbuf ;
}

void Stream::Rewind( ) const
{
	PDF_ASSERT( m_data.get() != 0 ) ;
	m_data->filter->Rewind( ) ;
}

Name Stream::Type( ) const
{
	return m_self["Type"] ;
}

Name Stream::Subtype( ) const
{
	return m_self["Subtype"] ;
}

std::size_t Stream::Append( const unsigned char *buf, std::size_t size )
{
	PDF_ASSERT( m_data.get() != 0 ) ;
	PDF_ASSERT( m_data->filter.get() != 0 ) ;
	PDF_ASSERT( m_data->dirty ) ;

	CopyOnWrite( ) ;
	return m_data->filter->Write( buf, size ) ;
}

std::size_t Stream::Append( const char *str )
{
	return Append( reinterpret_cast<const unsigned char*>(str),
	               std::strlen( str ) ) ;
}

bool Stream::IsDirty( ) const
{
	PDF_ASSERT( m_data.get() != 0 ) ;
	return m_data->dirty ;
}

void Stream::Flush( )
{
	PDF_ASSERT( m_data.get() != 0 ) ;
	PDF_ASSERT( m_data->filter.get() != 0 ) ;
	PDF_ASSERT( m_data->dirty ) ;
	
	CopyOnWrite( ) ;
	
	m_data->filter->Flush( ) ;
}

bool Stream::IsContentEqual( const Stream& others ) const
{
	// no use to compare Length(). if the filters are different, Length()
	// will be different but the data may be the same.

	StreamFilter *f		= m_data->filter.get() ;
	StreamFilter *f2	= others.m_data->filter.get() ;
	
	// rewind to the start of the stream
	f->Rewind( ) ;
	f2->Rewind( ) ;
	
	unsigned char data[80], data2[80] ;
	std::size_t count	= 0 ;
	std::size_t count2	= 0 ;

	do
	{
		count	= f->Read( data, sizeof(data) ) ;
		count2	= f2->Read( data2, sizeof(data2) ) ;
		
		if ( count != count2 || std::memcmp( data, data2, count ) != 0 )
			return false ;
			
	} while ( count > 0 ) ;
	
	return true ;
}

void Stream::PrintAsC( std::ostream& os ) const
{
	m_data->filter->Rewind( ) ;
	OutStreamBufAdaptor::int_type c ;
	
	os << "const unsigned char bytes[] =\n{\n" ;
	
	std::size_t count = 0 ;
	while ( (c = m_data->inbuf.sbumpc())
		!= OutStreamBufAdaptor::traits_type::eof( ) )
	{
		if ( count % 8 == 0 )
			os << '\t' ;
	
		os	<< "0x"
			<< std::hex << std::setw( 2 ) << std::setfill('0') << c << ", " ;
		
		++count ;
		if ( count % 8 == 0 )
			os << '\n' ;
	}
	os << "\n}" ;
}

void Stream::CopyOnWrite( )
{
	if ( !m_data.unique() )
		m_data.reset( new Data( *m_data ) ) ;
}

Name Stream::FilterName( ) const
{
	return m_data->filter->RawFormat( ) ;
}

} // end of namespace

namespace std
{
	void swap( pdf::Stream& s1, pdf::Stream& s2 )
	{
		s1.Swap( s2 ) ;
	}
}
