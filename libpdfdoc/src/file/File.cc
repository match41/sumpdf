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
	\file	File.cc
	\brief	implementation the File class
	\date	Fri Mar 21 2008
	\author	Nestal Wan
*/

#include "File.hh"

#include "core/Array.hh"
#include "core/Object.hh"
#include "core/Token.hh"
#include "core/TokenSrc.hh"

#include "util/Exception.hh"
#include "util/SymbolInfo.hh"
#include "util/Util.hh"

#include "stream/Stream.hh"

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
File::File( std::istream *is )
	: m_objs( 1, 0 ), m_in( is ), m_out( 0 )
{
	ReadXRef( ReadXRefOffset( ), m_trailer ) ;
	
	std::size_t obj_count = int( m_trailer["Size"] ) ;
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
File::File( std::ostream *os )
	: m_objs( 1, 0 ), m_in( 0 ), m_out( os )
{
	assert( m_out != 0 ) ;
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
	\param	producer	if the document was converted to PDF from another
						format, the name of the application (for example,
						libpdfdoc) that converted it to PDF.
	\param	creator		if the document was converted to PDF from another
						format, the name of the application (for example,
						Adobe FrameMaker�) that created the original document
						from which it was converted.
*/
void File::WriteTrailer( const Ref& catalog, const std::string& producer,
	                                         const std::string& creator )
{
	assert( m_out != 0 ) ;
	
	Dictionary info ;
	info["Producer"]		= producer ;
	info["Creator"]			= creator ;

	m_trailer["Info"]	= WriteObj( info ) ;
	m_trailer["Root"]	= catalog ;
	m_trailer["Size"]	= m_objs.size( ) ;

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

Object File::ReadObj( const Ref& obj )
{
	assert( m_in != 0 ) ;

	// use at() because it will check bounding
	std::size_t offset = m_objs.at( obj.ID() ) ;
	m_in->seekg( offset ) ;
	
	std::size_t id, gen ;
	
	Token objstr ;
	
	if ( (*m_in >> id >> gen >> objstr)	&&
	     objstr.Get()	== "obj"	&&
	     obj.ID()		== id		&&
	     obj.Gen()		== gen )
	{
		// from now on, we must use TokenSrc to read the PDF objects.
		// it is because when reading the objects, some tokens may be
		// PutBack() to the TokenSrc. if we use operator>>(std::istream&)
		// to read the objects, it will internally construct and destruct
		// the TokenSrc objects, and the PutBack()'ed tokens will be lost.
		TokenSrc src( *m_in ) ;
		
		// read the underlying object
		Object r ;
		if ( src >> r )
		{
			src >> objstr ;	// endobj or stream
			
			if      ( objstr.Get() == "endobj" ) return r ;
			else if ( objstr.Get() == "stream" )
				return ReadStream( r.As<Dictionary>() ) ;
			
			// if the objstr is neither "endobj" nor "stream", it will
			// fall through to the following throw
		}
	}
	
	std::ostringstream ss ;
	ss << "cannot read object ID " << obj
	   << " offset: " << std::hex << offset
	   << " current token: " << objstr.Get() ;
	throw ParseError( ss.str() ) ;
}

Object File::ReadStream( Dictionary& dict )
{
	assert( m_in != 0 ) ;

	char ch ;
	if ( !m_in->get( ch ) || ( ch != '\r' && ch != '\n' ) )
		throw ParseError( "no newline after stream" ) ;
	
	if ( ch == '\r' && m_in->peek() == '\n' )
		m_in->get( ch ) ;
	
	// Length may be indirect object
	Object length = dict["Length"] ;
	if ( length.Type() == Object::ref )
	{
		std::streampos pos	= m_in->tellg( ) ;
		dict["Length"] = ReadObj( length ) ;
		m_in->seekg( pos ) ;
	}
	
	return Stream( m_in->rdbuf(), m_in->tellg( ), dict ) ;
}

Ref File::WriteObj( const Object& obj )
{
	Ref r = AllocLink( ) ;
	WriteObj( obj, r ) ;
	return r ;
}

Ref File::AllocLink( )
{
	std::size_t id = m_objs.size( ) ;
	m_objs.push_back( 0 ) ;
	return Ref( id, 0 ) ;
}

void File::WriteObj( const Object& obj, const Ref& link )
{
	assert( m_out != 0 ) ;
	assert( link.ID( ) < m_objs.size( ) ) ;
	assert( m_objs[link.ID()] == 0 ) ;
	
	m_objs[link.ID()] = m_out->tellp( ) ;
	*m_out << link.ID() << " 0 obj\n"
	       << obj << "\nendobj\n" ;
}

void File::ReadXRef( std::size_t offset, Dictionary& trailer )
{
	assert( m_in != 0 ) ;
	m_in->rdbuf()->pubseekoff( offset, std::ios::beg ) ;
	
	// reading xref	
	std::string line ;
	if ( !ReadLine( *m_in, line ) /*|| line != "xref" */)
		throw ParseError( "can't read xref marker" ) ;
	
	// start ID and number of object
	std::size_t start, count ;
	if ( ReadLine( *m_in, line ) )
	{
		std::istringstream ss ( line ) ;
		if ( !( ss >> start >> count ) )
			throw std::runtime_error( "can't read object count in xref" ) ;
	}
	else
		throw std::runtime_error( "can't read object count in xref" ) ;
	
	if ( start + count > m_objs.size() )
		m_objs.resize( start + count ) ;
	
	std::size_t index = 0 ;
	while ( ReadLine( *m_in, line ) && index < count )
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
	
	std::size_t prev_offset = 0 ;
	if ( ReadTailer( trailer, prev_offset ) )
		ReadXRef( prev_offset, trailer ) ;
}

bool File::ReadTailer( Dictionary& trailer, std::size_t& prev_offset )
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
			prev_offset = int(i->second) ;
		}

		// never over-write existing values in trailer dictionary,
		// as values in later trailers override previous ones.
		else if ( trailer.find( i->first ) == trailer.end( ) )
			trailer.insert( *i ) ;
	}

	return found_prev ;
}

std::size_t File::ReadXRefOffset( )
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

std::istream& File::ReadLine( std::istream& is, std::string& line )
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

Ref File::Root( ) const
{
	return m_trailer["Root"] ;
}

ResourcePool* File::Pool( )
{
	return &m_pool ;
}

} // end of namespace
