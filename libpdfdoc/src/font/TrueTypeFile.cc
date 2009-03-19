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
	\file	TrueTypeFile.cc
	\brief	implementation the TrueTypeFile class
	\date	Fri Mar 6 2009
	\author	Nestal Wan
*/

#include "TrueTypeFile.hh"
#include "TrueTypeTable.hh"

#include "util/Exception.hh"
#include "util/Util.hh"

#include <iostream>

namespace font {

TrueTypeFile::TrueTypeFile( const std::string& filename )
	: m_file( filename.c_str(), std::ios::in | std::ios::binary )
{
	if ( !m_file )
		throw pdf::Exception( "cannot read from file " + filename ) ;
	
	ReadTableDir( ) ;
}

void TrueTypeFile::ReadTableDir( )
{
	// skip scale type
	Skip( sizeof(boost::uint32_t) ) ;
	
	// read number of tables
	boost::uint16_t count ;
	Read( count ) ;
	
	// skip search range, entry selector and range shift
	Skip( 3 * sizeof(boost::uint16_t) ) ;
	
	// read the table entry
	for ( boost::uint16_t i = 0 ; i < count ; i++ )
	{
		//std::string tag( ' ', 4 ) ;
		char tag[4] ;
		uint32_t checksum, offset, length ;
		
		for ( std::size_t j = 0 ; j < pdf::Count(tag) ; j++ )
			Read( tag[j] ) ;
		Read( checksum ) ;
		Read( offset ) ;
		Read( length ) ;
		
		m_tables.insert( std::make_pair( std::string( pdf::Begin(tag),
		                                              pdf::End(tag) ),
		                                 TrueTypeTable( m_file.rdbuf(),
		                                                offset, length ) ) ) ;

/*std::string stag( tag, tag + 4 ) ;
std::cout << stag << " " << offset << " " << length
          << " " << checksum << std::endl ;*/
	}
}

TrueTypeFile::~TrueTypeFile( )
{
}

void TrueTypeFile::Skip( std::size_t count )
{
	m_file.rdbuf()->pubseekoff( count, std::ios::cur, std::ios::in ) ;
}

template <typename T>
void TrueTypeFile::Read( T& i )
{
	i = 0 ;
	std::size_t s = sizeof( T ) ;
	while (s > 0)
	{
		int c = m_file.rdbuf()->sbumpc() ;
		if ( c < 0)
			throw pdf::Exception( "unexpected EOF" ) ;
		
		i = (i << 8) + c;
		s--;
	}
}

} // end of namespace
