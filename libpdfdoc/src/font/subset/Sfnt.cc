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

// boost headers
#include <boost/bind.hpp>

// stdc++ headers
#include <cstring>
#include <iostream>
#include <streambuf>
#include <map>

namespace pdf {

struct Sfnt::Table
{
	u32		tag ;
	u32		checksum ;
	u32		offset ;
	u32		length ;
} ;

struct Sfnt::Impl
{
	FT_Face		face ;

	typedef std::vector<Table> TableVec ;
	TableVec 	tables ;
	
	typedef std::map<unsigned long, Table*> TableMap ;
	TableMap	table_map ;
	
	TT_Header	*head ;
	
	std::vector<unsigned long> loca ;
} ;

// non-member helpers
namespace
{
	u32 Checksum( const unsigned char *b, std::size_t length )
	{
		std::size_t len = length / 4;
		u32 d0 = 0;
		u32 d1 = 0;
		u32 d2 = 0;
		u32 d3 = 0;
		size_t ptr = 0;
		size_t k;
		for (k = 0; k < len; ++k)
		{
			d3 += static_cast<u32>(b[ptr++]) & 0xff;
			d2 += static_cast<u32>(b[ptr++]) & 0xff;
			d1 += static_cast<u32>(b[ptr++]) & 0xff;
			d0 += static_cast<u32>(b[ptr++]) & 0xff;
		}
		return d0 + (d1 << 8) + (d2 << 16) + (d3 << 24);
	}
	
	const unsigned long required_tables[] =
	{
		TTAG_cmap, TTAG_cvt , TTAG_fpgm, TTAG_glyf, TTAG_head,
		TTAG_hhea, TTAG_hmtx, TTAG_loca, TTAG_maxp, TTAG_prep,
	};
}

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

std::vector<unsigned char> Sfnt::ReadTable( const Table& tab ) const
{
	std::vector<unsigned char> table( tab.length ) ;
	
	unsigned long size = tab.length ;
	FT_Error e = FT_Load_Sfnt_Table( m_impl->face, tab.tag, 0, &table[0],
		&size ) ;
	if ( e != 0 || size != tab.length )
		throw FontException( "cannot load table" ) ;
	
	return table ;
}

/// load the "loca" table, which gives the offset of the data for each glyph
void Sfnt::LoadLocation( )
{
	std::vector<unsigned char> loca = ReadTable( FindTable( TTAG_loca ) ) ;
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
	// load the first 12 bytes from the font file
	unsigned char tmp[12] ;
	unsigned long count = sizeof(tmp) ;
	FT_Error e = FT_Load_Sfnt_Table( m_impl->face, 0, 0, tmp, &count ) ;
	if ( e != 0 )
		throw FontException( "can't load truetype font" ) ;

	// the first 4 byte is called "scaler type", which should be 0x00010000
	// the next 2 byte is the number of table in the file. we want this
	ReadStream str( tmp, sizeof(tmp) ) ;
	u32 scaler ;
	u16 table_count, search_range, entry_selector, range_shift ;
	str	>> scaler >> table_count >> search_range >> entry_selector
		>> range_shift ;

	std::vector<unsigned char> table_dir( table_count * sizeof(Table) ) ;
	count = table_dir.size() ;
	e = FT_Load_Sfnt_Table( m_impl->face, 0, 12, &table_dir[0], &count ) ;
	if ( e != 0 )
		throw FontException( "can't load truetype font" ) ;
	ReadStream dir_str( &table_dir[0], table_dir.size() ) ;

	Impl::TableVec& tables = m_impl->tables ;

	// read the tag and length of each table in the font
	for ( u16 i = 0 ; i < table_count ; i++ )
	{
		Table tab = {} ;
		dir_str >> tab.tag >> tab.checksum >> tab.offset >> tab.length ;
		
		tables.push_back( tab ) ;
	}
	
	for ( Impl::TableVec::iterator i = tables.begin() ;
		i != tables.end() ; ++i )
		m_impl->table_map.insert( std::make_pair( i->tag, &*i ) ) ;
}

void Sfnt::Write(
	std::streambuf	*str,
	const unsigned	*glyphs,
	std::size_t 	size ) const
{
	using namespace boost ;
	static const u32 entry_selectors[] =
	// 0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20
	{  0, 0, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4};

	u16 table_used = static_cast<u16>(m_impl->tables.size()) ;
	if ( table_used >= Count(entry_selectors) )
		throw FontException( "too many tables" ) ;
	
	int selector = entry_selectors[table_used];

	WriteStream out(str) ;
	out	<< u32(0x00010000) << table_used
		<< u16((1 << selector) * 16)
		<< u16(selector)
		<< u16((table_used - (1 << selector)) * 16) ;

	if ( glyphs == 0 && size == 0 )
	{
		// write the table directory entries for all tables
		std::for_each( m_impl->tables.begin(), m_impl->tables.end(),
			bind( &Sfnt::WriteTableDirEntry, this, ref(out), _1 ) ) ;
		
		// sort the table by offset to retain original order
		Impl::TableVec tables = m_impl->tables ;
		std::sort( tables.begin(), tables.end(),
			(bind( &Table::offset, _1 ) < bind( &Table::offset, _2 )) ) ; 
		
		// write the tables
		std::for_each( tables.begin(), tables.end(),
			bind( &Sfnt::WriteTable, this, str, _1 ) ) ;
	}
	// only write the required tables
	else
	{
		std::for_each(
			Begin(required_tables),
			End(required_tables),
			bind( &Sfnt::WriteTableDirEntry,
				this,
				ref(out),
				bind( &Sfnt::FindTable, this, _1 ) ) ) ;
		
		std::for_each(
			Begin(required_tables),
			End(required_tables),
			bind( &Sfnt::WriteTable,
				this,
				str,
				bind( &Sfnt::FindTable, this, _1 ) ) ) ;
	}
}

void Sfnt::WriteTableDirEntry( WriteStream& s, const Table& tab ) const
{
	s << tab.tag << tab.checksum << tab.offset << tab.length ;
}

void Sfnt::WriteTable( std::streambuf *s, const Table& tab ) const
{
	std::vector<unsigned char> data = ReadTable( tab ) ;
	std::size_t padding = ((tab.length + 3) & (~3)) - tab.length ;
	PDF_ASSERT( padding < 4 ) ;
	unsigned char zeros[4] = {} ;
	data.insert( data.end(), zeros, zeros + padding ) ;
	s->sputn( reinterpret_cast<char*>( &data[0] ), data.size() ) ;
}

Sfnt::Table Sfnt::FindTable( unsigned long tag ) const
{
	Impl::TableMap::const_iterator i = m_impl->table_map.find( tag ) ;
	if ( i != m_impl->table_map.end() )
	{
		PDF_ASSERT( i->second != 0 ) ;
		return *i->second ;
	}
	throw FontException( "required table not found!" ) ;
}

} // end of namespace
