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
\***************************************************************************/

/**	\file	Sfnt.cc
	\brief	implementation of the Sfnt class
	\date	Apr 9, 2010
	\author	Nestal Wan
*/

#include "Sfnt.hh"

#include "font/FontException.hh"
#include "util/Debug.hh"

// freetype headers
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_TRUETYPE_TAGS_H
#include FT_TRUETYPE_TABLES_H

// boost headers
#include <boost/detail/endian.hpp>
#include <boost/cstdint.hpp>

// stdc++ headers
#include <cstring>
#include <iostream>
#include <map>

namespace pdf {

namespace
{
	typedef boost::uint32_t	u32 ;
	typedef boost::uint16_t	u16 ;
	typedef boost::uint8_t	u8 ;

	template <typename T>
	T SwapByte( T t ) ;
	
#ifdef __GNUC__
	template <>
	u32 SwapByte( u32 t )
	{
		return __builtin_bswap32( t ) ;
	}
	template <>
	u16 SwapByte( u16 t )
	{
		return ((t & 0xff) << 8 ) | (t >> 8) ;
	}
#endif

	class Stream
	{
	public :
		Stream( const unsigned char *p, std::size_t size )
		: m_ptr( p )
		, m_size( size )
		{
		}

		template <typename T>
		Stream& operator>>( T& v )
		{
			if ( m_size >= sizeof(v) )
			{
				std::memcpy( &v, m_ptr, sizeof(v) ) ;

#ifdef BOOST_LITTLE_ENDIAN
				v = SwapByte( v ) ;
#endif
				
				m_ptr	+= sizeof(T) ;
				m_size	-= sizeof(T) ;
			}
			
			return *this ;
		}
	
	private :
		const unsigned char	*m_ptr ;
		std::size_t			m_size ;
	} ;
}

struct Sfnt::Impl
{
	// map from tags to length
	std::map<unsigned long, unsigned long> tables ;
	
	TT_Header	*head ;
} ;

/**	constructor
	
*/
Sfnt::Sfnt( FT_FaceRec_ *face )
: m_impl( new Impl )
{
	LoadTableInfo( face ) ;

	m_impl->head = reinterpret_cast<TT_Header*>(
		FT_Get_Sfnt_Table( face, ft_sfnt_head ) ) ;
	if ( m_impl->head == 0 )
		throw FontException( "head table not loaded yet" ) ;
		
	PDF_ASSERT( m_impl->head->Units_Per_EM == face->units_per_EM ) ;
}

Sfnt::~Sfnt( )
{
}

void Sfnt::LoadTableInfo( FT_FaceRec_ *face )
{
	unsigned char tmp[6] ;
	unsigned long count = sizeof(tmp) ;
	FT_Error e = FT_Load_Sfnt_Table( face, 0, 0, tmp, &count ) ;
	if ( e != 0 )
		throw FontException( "can't load truetype font" ) ;
	
	Stream str( tmp, sizeof(tmp) ) ;
	u32 scaler ;
	u16 table_count ;
	str >> scaler >> table_count ;
	
	for ( u16 i = 0 ; i < table_count ; i++ )
	{
		unsigned long length = 0, tag = 0 ;
		e = FT_Sfnt_Table_Info( face, i, &tag, &length ) ;
		PDF_ASSERT( e == 0 ) ;
		
		m_impl->tables.insert( std::make_pair( tag, length ) ) ;
	}
	
}

} // end of namespace
