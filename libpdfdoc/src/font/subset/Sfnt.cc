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

// headers in the same package
#include "Types.hh"
#include "ReadStream.hh"
#include "WriteStream.hh"

// headers from other packages
#include "font/FontException.hh"
#include "util/Debug.hh"
#include "util/Util.hh"

// freetype headers
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_TRUETYPE_TAGS_H
#include FT_TRUETYPE_TABLES_H

// stdc++ headers
#include <cstring>
#include <iostream>
#include <streambuf>
#include <map>

namespace pdf {

struct Sfnt::Impl
{
	FT_Face		face ;

	// map from tags to length
	typedef std::map<unsigned long, unsigned long> TableLen ;
	TableLen 	table_len ;
	
	TT_Header	*head ;
	
	std::vector<unsigned long> loca ;
} ;

/**	constructor
	
*/
Sfnt::Sfnt( FT_FaceRec_ *face )
: m_impl( new Impl )
{
	m_impl->face = face ;

	LoadTableInfo( ) ;

	// get the "head" table
	m_impl->head = reinterpret_cast<TT_Header*>(
		FT_Get_Sfnt_Table( m_impl->face, ft_sfnt_head ) ) ;
	if ( m_impl->head == 0 )
		throw FontException( "head table not loaded yet" ) ;
	
	// the value from the table should match with freetype
	PDF_ASSERT( m_impl->head->Units_Per_EM == m_impl->face->units_per_EM ) ;
	
	LoadLocation( ) ;
}

Sfnt::~Sfnt( )
{
}

std::vector<unsigned char> Sfnt::LoadTable( unsigned long tag ) const
{
	Impl::TableLen::const_iterator i = m_impl->table_len.find( tag ) ;
	if ( i != m_impl->table_len.end() )
	{
		std::vector<unsigned char> table( i->second ) ;
		
		unsigned long size = i->second ;
		FT_Error e = FT_Load_Sfnt_Table( m_impl->face, tag, 0, &table[0],
			&size ) ;
		if ( e != 0 || size != i->second )
			throw FontException( "cannot load table" ) ;
		
		return table ;
	}
	else
		return std::vector<unsigned char>() ;
}

/// load the "loca" table, which gives the offset of the data for each glyph
void Sfnt::LoadLocation( )
{
	std::vector<unsigned char> loca = LoadTable( TTAG_loca ) ;
	ReadStream str( &loca[0], loca.size() ) ;
	
	for ( long i = 0 ; i < m_impl->face->num_glyphs ; i++ )
	{
		// short format (16bits) of offset
		if ( m_impl->head->Index_To_Loc_Format == 0 )
		{
			// needs to multiply by 2 for short format of location,
			// according to truetype spec
			u16 v ;
			str >> v ;
			m_impl->loca.push_back( v*2 ) ;
		}
		// long format (32bits) of offset
		else
		{
			// no need to multiply by 2
			u32 v ;
			str >> v ;
			m_impl->loca.push_back( v ) ;
		}
	}
}

void Sfnt::LoadTableInfo( )
{
	// load the first 6 bytes from the font file
	unsigned char tmp[6] ;
	unsigned long count = sizeof(tmp) ;
	FT_Error e = FT_Load_Sfnt_Table( m_impl->face, 0, 0, tmp, &count ) ;
	if ( e != 0 )
		throw FontException( "can't load truetype font" ) ;

	// the first 4 byte is called "scaler type", which should be 0x00010000
	// the next 2 byte is the number of table in the file. we want this
	ReadStream str( tmp, sizeof(tmp) ) ;
	u32 scaler ;
	u16 table_count ;
	str >> scaler >> table_count ;
	
	// read the tag and length of each table in the font
	for ( u16 i = 0 ; i < table_count ; i++ )
	{
		unsigned long length = 0, tag = 0 ;
		e = FT_Sfnt_Table_Info( m_impl->face, i, &tag, &length ) ;
		if ( e != 0 )
			throw FontException( "can't get info for table" ) ;
		
		m_impl->table_len.insert( std::make_pair( tag, length ) ) ;
	}
}

void Sfnt::Write( std::streambuf *str ) const
{
	static const u32 entry_selectors[] =
	// 0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20
	{  0, 0, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4};

	u16 table_used = static_cast<u16>(m_impl->table_len.size()) ;
	if ( table_used >= Count(entry_selectors) )
		throw FontException( "too many tables" ) ;
	
	int selector = entry_selectors[table_used];

	WriteStream out(str) ;
	out	<< u32(0x00010000) << table_used
		<< u16((1 << selector) * 16)
		<< u16(selector)
		<< u16((table_used - (1 << selector)) * 16) ;

}

} // end of namespace
