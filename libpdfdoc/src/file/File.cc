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
	\file	File.cc
	\brief	implementation the File class
	\date	Fri Mar 21 2008
	\author	Nestal Wan
*/

#include "File.hh"

#include "core/Array.hh"
#include "core/Object.hh"
#include "core/Stream.hh"
#include "core/Token.hh"
#include "core/TokenSrc.hh"
#include "util/Exception.hh"
#include "util/Util.hh"

#include "util/SymbolInfo.hh"

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
	: m_objs( 1, 0 ), m_ifile( is ), m_ofile( 0 )
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
	: m_objs( 1, 0 ), m_ifile( 0 ), m_ofile( os )
{
	assert( m_ofile != 0 ) ;
	*m_ofile << "%PDF-1.4\n" ;
}

void File::WriteTrailer( const Ref& catalog, const std::string& producer,
	                                         const std::string& creator )
{
	assert( m_ofile != 0 ) ;
	
	Dictionary info ;
	info["Producer"]		= producer ;
	info["CreationDate"]	= creator ;

	m_trailer["Info"]	= WriteObj( info ) ;
	m_trailer["Root"]	= catalog ;
	m_trailer["Size"]	= m_objs.size( ) ;

	std::streampos xref_pos = m_ofile->tellp( ) ;
	*m_ofile << "xref\n"
	         << "0 " << m_objs.size( ) << "\n" ;

	for ( std::vector<std::size_t>::const_iterator i = m_objs.begin( ) ;
	                                               i != m_objs.end( ) ; ++i )
	{
		*m_ofile << std::setw( 10 ) << std::setfill( '0' )
		         << *i << (i == m_objs.begin() ? " 65535 f \n"
		                                       : " 00000 n \n" ) ;
	}
	
	*m_ofile << "trailer\n" << m_trailer << "\nstartxref\n" << (int)xref_pos
	         << "\n%%EOF\n" ;
}

Object File::ReadObj( const Ref& obj )
{
	assert( m_ifile != 0 ) ;

	// use at() because it will check bounding
	std::size_t offset = m_objs.at( obj.ID() ) ;
	m_ifile->seekg( offset ) ;
	
	std::size_t id, gen ;
	
	Token objstr ;
	
	if ( (*m_ifile >> id >> gen >> objstr)	&&
	     objstr.Get()		== "obj"		&&
	     obj.ID()			== id			&&
	     obj.Generation( )	== gen )
	{
		// from now on, we must use TokenSrc to read the PDF objects.
		// it is because when reading the objects, some tokens may be
		// PutBack() to the TokenSrc. if we use operator>>(std::istream&)
		// to read the objects, it will internally construct and destruct
		// the TokenSrc objects, and the PutBack()'ed tokens will be lost.
		TokenSrc src( *m_ifile ) ;
		
		// read the underlying object
		Object r ;
		if ( src >> r )
		{
			src >> objstr ;	// endobj or stream
			
			if      ( objstr.Get() == "endobj" ) return r ;
			else if ( objstr.Get() == "stream" ) return ReadStream( r ) ;
			
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

Object File::ReadStream( const Dictionary& dict )
{
	assert( m_ifile != 0 ) ;

	char ch ;
	if ( !m_ifile->get( ch ) || ( ch != '\r' && ch != '\n' ) )
		throw ParseError( "no newline after stream" ) ;
	
	if ( ch == '\r' && m_ifile->peek() == '\n' )
		m_ifile->get( ch ) ;
	
	// Length may be indirect object
	Object length = dict["Length"] ;
	if ( length.Type() == Object::ref )
	{
		std::streampos pos	= m_ifile->tellg( ) ;
		length = ReadObj( length ) ;
		m_ifile->seekg( pos ) ;
	}

	std::vector<unsigned char> data( length.As<int>( ) ) ;
	if ( m_ifile->rdbuf()->sgetn( reinterpret_cast<char*>( &data[0]),
	                                                        data.size() )
			!= static_cast<std::streampos>( data.size() ) )
		throw ParseError( "cannot read stream data" ) ;

	return Stream( data, dict ) ;
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
	assert( m_ofile != 0 ) ;
	assert( link.ID( ) < m_objs.size( ) ) ;
	assert( m_objs[link.ID()] == 0 ) ;
	
	m_objs[link.ID()] = m_ofile->tellp( ) ;
	
	// if the object to write is a stream, write reference instead of
	// the object directly
	*m_ofile << link.ID() << " 0 obj\n" << obj << "\nendobj\n" ;
}

void File::ReadXRef( std::size_t offset, Dictionary& trailer )
{
	assert( m_ifile != 0 ) ;
	m_ifile->rdbuf()->pubseekoff( offset, std::ios::beg ) ;
	
	// reading xref	
	std::string line ;
	if ( !ReadLine( *m_ifile, line ) /*|| line != "xref" */)
		throw ParseError( "can't read xref marker" ) ;
	
	// start ID and number of object
	std::size_t start, count ;
	if ( ReadLine( *m_ifile, line ) )
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
	while ( ReadLine( *m_ifile, line ) && index < count )
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
	if ( !(*m_ifile >> t ) )
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
	m_ifile->rdbuf()->pubseekoff( -end_size, std::ios::end ) ;
	
	char bytes[end_size], startxref[] = "startxref" ;
	std::size_t count =  m_ifile->rdbuf()->sgetn( bytes, sizeof(bytes) ) ;
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

} // end of namespace
