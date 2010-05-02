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

/**	\file	FontCache.cc
	\brief	implementation of the FontCache class
	\date	May 1, 2010
	\author	Nestal Wan
*/

#include "FontCache.hh"

#include "font/FontDb.hh"

#include "SimpleFont.hh"
#include "FontException.hh"

// freetype headers
#include <ft2build.h>
#include FT_FREETYPE_H

namespace pdf {

/**	constructor
	
*/
FontCache::FontCache( )
	: m_font_db( CreateFontDb() )
{
}

FontCache::~FontCache( )
{
}

BaseFont* FontCache::GetFont( FT_FaceRec_ *face )
{
	const char *psname = FT_Get_Postscript_Name( face ) ;
	if ( psname == 0 )
		throw FontException( "font type not supported" ) ;

	FontMap::iterator i = m_fonts.find( psname ) ;
	if ( i != m_fonts.end() )
	{
		i->second->AddRef() ;
		return i->second ;
	}
	else
	{
		BaseFont *result = new SimpleFont( face, m_font_db.get() ) ;
		m_fonts.insert( std::make_pair( psname, result ) ) ;
		return result ;
	}
}

BaseFont* FontCache::GetFont( const unsigned char *data, std::size_t size )
{
	std::vector<unsigned char> prog( data, data + size ) ;

	FT_Face face = m_font_db->LoadFont( &prog[0], size ) ;

	const char *psname = FT_Get_Postscript_Name( face ) ;
	if ( psname == 0 )
		throw FontException( "font type not supported" ) ;

	FontMap::iterator i = m_fonts.find( psname ) ;
	if ( i != m_fonts.end() )
	{
		FT_Done_Face( face ) ;
		i->second->AddRef() ;
		return i->second ;
	}
	else
	{
		BaseFont *result = new SimpleFont( face, prog ) ;
		m_fonts.insert( std::make_pair( psname, result ) ) ;
		return result ;
	}
}

FontDb* FontCache::Database()
{
	return m_font_db.get() ;
}

} // end of namespace
