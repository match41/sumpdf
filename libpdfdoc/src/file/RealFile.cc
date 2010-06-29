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
	\file	RealFile.cc
	\brief	implementation the File class
	\date	Fri Mar 21 2008
	\author	Nestal Wan
*/

#include "RealFile.hh"

#include "core/Array.hh"
#include "core/Object.hh"
#include "core/String.hh"
#include "core/Token.hh"

#include "util/Debug.hh"
#include "util/Exception.hh"
#include "util/SymbolInfo.hh"
#include "util/Util.hh"

#include "stream/Stream.hh"

#include <boost/bind.hpp>

#include <cassert>
#include <iomanip>
#include <sstream>
#include <iostream>

namespace pdf {

/*!	Constructor for reading PDF file. After construction, client can use
	the Root() function to get the reference to the PDF catalog of the file,
	as well as ReadObject() to read objects from the file.
	\param	is	istream for the PDF file. It must be opened with the
				std::ios::binary.
*/
RealFile::RealFile( std::istream *is )
	: m_objs( 1, 0 )
	, m_in( is )
	, m_out( 0 )
{
	ReadXRef( ReadXRefOffset( ), m_trailer ) ;

	std::size_t obj_count = m_trailer["Size"].To<int>() ;
	if ( m_objs.size() != obj_count )
	{
		std::ostringstream oss ;
		oss << "object count mismatch: trailer: " << obj_count
		    << " xref: " << m_objs.size() ;
		throw ParseError( oss.str() ) ;
	}
}

/*!	Constructor for writing PDF file. Client can then use WriteObject() to
	write objects to the file. After writing all objects, client must call
	WriteTrailer() to write the trailer to the file.
	\param	os	ostream for the PDF file. It must be opened with the
				std::ios::binary.
*/
RealFile::RealFile( std::ostream *os )
	: m_objs( 1, 0 )
	, m_in( 0 )
	, m_out( os )
{
	PDF_ASSERT( m_out != 0 ) ;
	*m_out << "%PDF-1.4\n" ;
}

/*!	\brief	write the PDF file trailer
	\internal

	This function writes the PDF file trailer. A PDF file trailer has a 
	trailer dictionary and a cross reference table. The trailer dictionary
	contains core information about the PDF file, e.g. number of object,
	link to document catalog. The cross reference table maps object IDs to
	offsets in the file. It allows fast look-up for the PDF objects stored
	in the file.
	\param	catalog		reference to the document catalog
	\param	info		reference to the document infor object
*/
void RealFile::WriteTrailer( const Ref& catalog, const Ref& info )
{
	PDF_ASSERT( m_out != 0 ) ;

	if ( info != Ref() )
		m_trailer.Set( "Info", info ) ;
	
	m_trailer.Set( "Root", catalog ) ;
	m_trailer.Set( "Size", m_objs.size( ) ) ;

	std::streampos xref_pos = m_out->tellp( ) ;
	*m_out << "xref\n"
	         << "0 " << m_objs.size( ) << "\n" ;

	for ( std::vector<std::size_t>::const_iterator i = m_objs.begin( ) ;
	                                               i != m_objs.end( ) ; ++i )
	{
		*m_out << std::setw( 10 ) << std::setfill( '0' )
		         << *i << (i == m_objs.begin() ? " 65535 f \n"
		                                       : " 00000 n \n" ) ;
	}
	
	*m_out << "trailer\n" << m_trailer << "\nstartxref\n" << (int)xref_pos
	       << "\n%%EOF\n" ;
}

void RealFile::ReadType( const Ref& link, Object& obj )
{
	PDF_ASSERT( m_in != 0 ) ;

	// use at() because it will check bounding
	std::size_t offset = m_objs.at( link.ID() ) ;
	m_in->seekg( offset ) ;

	if ( !ReadNextObj( obj ) )
	{
		std::ostringstream ss ;
		ss << "cannot read object ID " << link
		   << " offset: " << std::hex << offset
		   /*<< " current token: \"" << objstr.Get() << "\""*/ ;
		throw ParseError( ss.str() ) ;
	}
}

bool RealFile::ReadNextObj( Object& obj )
{
	std::size_t id, gen ;
	
	Token objstr ;
	
	if ( (*m_in >> id >> gen >> objstr)	&& objstr.Get()	== "obj" )
	{
		// read the underlying object
		Object r ;
		if ( *m_in >> r )
		{
			*m_in >> objstr ;	// endobj or stream
			
			if ( objstr.Get() == "endobj" )
			{
				obj.Swap( r ) ;
				return true ;
			}
			else if ( objstr.Get() == "stream" )
			{
				obj = ReadStream( r.As<Dictionary>() ) ;
				return true ;
			}
			
			// if the objstr is neither "endobj" nor "stream", it will
			// fall through to the following throw
		}
	}

	return false ;
}

template <typename T>
void RealFile::BasicRead( const Ref& link, T& result )
{
	PDF_ASSERT( m_in != 0 ) ;

	// use at() because it will check bounding
	std::size_t offset = m_objs.at( link.ID() ) ;
	m_in->seekg( offset ) ;
	
	std::size_t id, gen ;
	
	Token objstr ;
	
	if ( (*m_in >> id >> gen >> objstr)	&&
		 objstr.Get()	== "obj"	&&
	     link.ID()		== id		&&
	     link.Gen()		== gen )
	{
		// read the underlying object
		if ( *m_in >> result )
		{
			*m_in >> objstr ;	// endobj or stream
			
			if ( objstr.Get() == "endobj" )
				return ;
		
			// if the objstr is neither "endobj" nor "stream", it will
			// fall through to the following throw
		}
	}
	
	std::ostringstream ss ;
	ss << "cannot read object ID " << link
	   << " offset: 0x" << std::setfill('0') << std::setw(8) << std::hex << offset
	   << " current token: \"" << objstr.Get() << "\"" ;
	throw ParseError( ss.str() ) ;
}

void RealFile::ReadType( const Ref& link, bool& value )
{
	Object obj = ReadObj( link ) ;
	std::swap( obj.As<bool>(), value ) ;
}

void RealFile::ReadType( const Ref& link, Dictionary& dict )
{
	BasicRead( link, dict ) ;
}

void RealFile::ReadType( const Ref& link, Array& array )
{
	BasicRead( link, array ) ;
}

void RealFile::ReadType( const Ref& link, int& value )
{
	Object obj = ReadObj( link ) ;
	value = obj.To<int>( ) ;
}

void RealFile::ReadType( const Ref& link, double& value )
{
	Object obj = ReadObj( link ) ;
	value = obj.To<double>( ) ;
}

void RealFile::ReadType( const Ref& link, Name& value )
{
	BasicRead( link, value ) ;
}

void RealFile::ReadType( const Ref& link, std::string& value )
{
	String s ;
	BasicRead( link, s ) ;
	s.Swap( value ) ;
}

void RealFile::ReadType( const Ref& link, Ref& value )
{
	BasicRead( link, value ) ;
}

void RealFile::ReadType( const Ref& link, Stream& value )
{
	Object obj = ReadObj( link ) ;
	std::swap( obj.As<Stream>(), value ) ;
}

Object RealFile::ReadObj( const Ref& obj )
{
	Object result ;
	ReadType( obj, result ) ;
	return result ;
}

Stream RealFile::ReadStream( Dictionary& dict )
{
	PDF_ASSERT( m_in != 0 ) ;

	char ch ;
	if ( !m_in->get( ch ) || ( ch != '\r' && ch != '\n' ) )
		throw ParseError( "no newline after stream" ) ;

std::cout << "char after stream = " << (int)ch << std::endl ;

	if ( ch == '\r' && m_in->peek() == '\n' )
		m_in->get( ch ) ;

std::cout << "char after stream2 = " << (int)ch << std::endl ;

std::cout << "char after stream3 = " << (int)m_in->peek() << std::endl ;

	// Length may be indirect object
	Object length = dict["Length"] ;
	if ( length.Is<Ref>() )
	{
		std::streampos pos	= m_in->tellg( ) ;
		dict.Set( "Length", ReadObj( length ) ) ;
		m_in->seekg( pos ) ;
	}

std::cout << "char after stream3 = " << (int)m_in->peek() << std::endl ;

	return Stream( m_in->rdbuf(), m_in->tellg( ), dict ) ;
}

Ref RealFile::WriteObj( const Object& obj )
{
	Ref r = AllocLink( ) ;
	WriteObj( obj, r ) ;
	return r ;
}

Ref RealFile::AllocLink( )
{
	std::size_t id = m_objs.size( ) ;
	m_objs.push_back( 0 ) ;
	return Ref( id, 0 ) ;
}

void RealFile::WriteObj( const Object& obj, const Ref& link )
{
	PDF_ASSERT( m_out != 0 ) ;
	PDF_ASSERT( link.ID( ) < m_objs.size( ) ) ;
	PDF_ASSERT( m_objs[link.ID()] == 0 ) ;
	
	m_objs[link.ID()] = m_out->tellp( ) ;
	*m_out << link.ID() << " 0 obj\n"
	       << obj << "\nendobj\n" ;
}

void RealFile::ReadXRef( std::size_t offset, Dictionary& trailer )
{
	PDF_ASSERT( m_in != 0 ) ;
	m_in->seekg( offset, std::ios::beg ) ;

	// reading xref	
	std::string line ;
	if ( ReadLine( *m_in, line ) )
	{
		// PDF 1.4 or below uses cross reference table
		if ( line == "xref" )
			ReadXRefTable( trailer ) ;
		
		// PDF 1.5 or above can use cross reference streams
		else
			ReadXRefStream( offset, trailer ) ;
	}
	else
		throw ParseError( "can't read xref marker" ) ;
}

void RealFile::ReadXRefStream( std::size_t offset, Dictionary& trailer )
{
	PDF_ASSERT( m_in != 0 ) ;
	m_in->seekg( offset, std::ios::beg ) ;
	
	Object obj ;
	ReadNextObj( obj ) ;
	Stream& s = obj.As<Stream>() ;
	
	std::cout << "read: " << s.Self() << std::endl ;
	
	std::vector<unsigned char> raw ;
	s.CopyData( raw ) ;
	
	PrintHex( std::cout, &raw[0], raw.size() ) ;
	
	throw ParseError( "PDF 1.5 object streams are not supported yet" ) ;
}

void RealFile::ReadXRefTable( Dictionary& trailer )
{
	while ( true )
	{
		std::string line ;
		
		// start ID and number of object
		std::size_t start, count ;
		if ( ReadLine( *m_in, line ) )
		{
			// the "trailer" keyword indicates the end of the xref sections
			if ( line == "trailer" )
				break ;
		
			std::istringstream ss ( line ) ;
			if ( !( ss >> start >> count ) )
				throw std::runtime_error( "can't read object count in xref" ) ;
		}
		else
			throw std::runtime_error( "can't read object count in xref" ) ;
		
		if ( start + count > m_objs.size() )
			m_objs.resize( start + count ) ;
		
		std::size_t index = 0 ;
		while ( index < count && ReadLine( *m_in, line ) )
		{
			std::istringstream ss( line ) ;
			std::size_t obj_offset, obj_gen ;
			char flag ;
			
			if ( ss >> obj_offset >> obj_gen >> flag )
				m_objs[start+index] = obj_offset ;
	
			else
				throw ParseError( "can't read object offset in xref" ) ;
			
			index++ ;
		}
	}
	
	// recursively read all xref and trailer sections in the file
	std::size_t prev_offset = 0 ;
	if ( ReadTrailer( trailer, prev_offset ) )
		ReadXRef( prev_offset, trailer ) ;
}

bool RealFile::ReadTrailer( Dictionary& trailer, std::size_t& prev_offset )
{	
	Dictionary t ;
	if ( !(*m_in >> t ) )
		return false ;

	bool found_prev = false ;
	for ( Dictionary::iterator i = t.begin( ) ; i != t.end( ) ; ++i )
	{
		if ( i->first == "Prev" )
		{
			found_prev = true ;
			prev_offset = i->second.To<int>() ;
		}

		// never over-write existing values in trailer dictionary,
		// as values in later trailers override previous ones.
		else if ( trailer.find( i->first ) == trailer.end( ) )
			trailer.insert( *i ) ;
	}

	return found_prev ;
}

std::size_t RealFile::ReadXRefOffset( )
{
	// PDF spec said the startxref word will appear at the last 1024 bytes
	// in the file
	const int end_size = 1024 ;
	m_in->rdbuf()->pubseekoff( -end_size, std::ios::end ) ;
	
	char bytes[end_size], startxref[] = "startxref" ;
	std::size_t count =  m_in->rdbuf()->sgetn( bytes, sizeof(bytes) ) ;
	char *result = std::search( bytes, bytes + count,
	                            startxref, startxref + sizeof(startxref) - 1 ) ;
	if ( result != bytes + sizeof(startxref) )
	{
        char *ptr = result + sizeof(startxref) ;
        while ( ptr != bytes + end_size && *ptr != '%' )
            ptr++ ;

        if ( ptr != bytes + end_size )
        {
		    *ptr = '\0' ;
		    std::size_t offset = std::atoi( result + + sizeof(startxref) ) ;
		    return offset ;
	    }
	}
	throw ParseError( "can't read xref offset" ) ;
}

std::istream& RealFile::ReadLine( std::istream& is, std::string& line )
{
    std::string str ;
    char ch ;
    while ( is.get( ch ) && ch != '\n' && ch != '\r' )
    {
        str.push_back( ch ) ;
    }

    // if CR followed by LF, then treat them as a single end-of-line character
    if ( ch == '\r' && is.peek() == '\n' )
        is.get( ) ;

    // commit if read success
    if ( is )
        str.swap( line ) ;
    
    return is ;
}

Ref RealFile::Root( ) const
{
	// "Root" is required and must be a indirect reference
	return m_trailer["Root"] ;
}

Ref RealFile::DocInfo( ) const
{
	// "Info" is optional, but must be indirect reference
	const Object& info = m_trailer["Info"] ;
	return info.Is<Ref>() ? info.As<Ref>() : Ref() ;
}

ElementPool* RealFile::Pool( )
{
	return &m_pool ;
}

} // end of namespace
