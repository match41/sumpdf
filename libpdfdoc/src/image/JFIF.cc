/***************************************************************************\
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
\************************************************debug::EnableTrace() ;***************************/

/**	\file	JFIF.cc
	\brief	implementation of the JFIF class
	\date	Oct 30, 2010
	\author	Nestal Wan
*/

#include "JFIF.hh"

#include "util/Debug.hh"
#include "util/Endian.hh"

#include <boost/cstdint.hpp>
#include <streambuf>

using boost::uint16_t ;

namespace {

bool ReadBigEndian( std::streambuf *buf, uint16_t& val )
{
	if ( buf->sgetn( reinterpret_cast<char*>( &val ), sizeof( val ) ) ==
		sizeof( val ) )
	{
		val = pdf::ToBigEndian( val ) ;
		return true ;
	}
	else
		return false ;
}

typedef std::streambuf::int_type	inttype ;
typedef std::streambuf::int_type	inttype ;
typedef std::streambuf::traits_type ttype ;

// JFIF markers. the names are from page 32 of the JPEG spec:
// http://www.w3.org/Graphics/JPEG/itu-t81.pdf
const inttype marker	= 0xFF ;
const inttype SOI		= 0xD8 ;
const inttype EOI		= 0xD9 ;
const inttype RST0		= 0xD0 ;
const inttype RST7		= 0xD7 ;
const inttype TEM		= 0x01 ;

}

namespace img {

using namespace pdf ;

/**	constructor
	
*/
JFIF::JFIF( std::streambuf *src, std::vector<unsigned char> *buf )
	: m_src ( src )
	, m_buf ( buf )
{
}

std::size_t JFIF::Size( ) const
{
	std::size_t offset = 0, start = 0 ;

	inttype c ;
	while ( (c = m_src->sbumpc()) != ttype::eof() )
	{
		offset++ ;
		if ( m_buf != 0 )
			m_buf->push_back( ttype::to_char_type( c ) ) ;
	
		// JPEG marker??
		if ( c == marker )
		{
			inttype m ;
			if ( (m = m_src->sbumpc()) != ttype::eof() )
			{
				offset++ ;
				if ( m_buf != 0 )
					m_buf->push_back( ttype::to_char_type( m ) ) ;
			
				// SOI: start of image
				if ( m == SOI )
				{
					debug::Trace() << "found SOI at " << std::dec << offset << std::endl ;
					
					// one marker has 2 bytes
					PDF_ASSERT( offset >= 2 ) ;
					start = offset - 2 ;
				}
				
				// EOI: end of image
				else if ( m == EOI )
				{
					debug::Trace() << "found EOI at " << std::dec << offset << std::endl ;
					return offset - start ;
				}
				
				else if ( m >= RST0 && m <= RST7 )
				{
					debug::Trace() << "found RST" << std::endl ;
				}
				
				else if ( m == TEM )
				{
					debug::Trace() << "found TEM" << std::endl ;
				}
				
				// other markers. need to read size and skip it
				else
				{
					debug::Trace() << "found marker 0x" << std::hex << m << std::endl ;
				
					uint16_t size ;
					if ( ReadBigEndian( m_src, size ) )
					{
						debug::Trace() << "skipping " << std::dec << size << " bytes" << std::endl ; 
						m_src->pubseekoff( size - sizeof(uint16_t), std::ios::cur ) ;
						
						offset += size ;
					}
				}
			}
		}
	}

	// cannot find EOI marker.
	return 0 ;
}


} // end of namespace
