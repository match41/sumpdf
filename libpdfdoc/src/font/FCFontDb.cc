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

/**	\file	FCFontDb.cc
	\brief	implementation of the FCFontDb class
	\date	Feb 11, 2010
	\author	Nestal Wan
*/

#include "FCFontDb.hh"

#include "FontException.hh"

// freetype headers
#include <ft2build.h>
#include FT_FREETYPE_H

// fontconfig headers
#ifndef HAVE_FONTCONFIG
	#error This file requires fontconfig to compile.
#else
#include <fontconfig/fontconfig.h>
#endif

namespace pdf {

/**	constructor
	
*/
FCFontDb::FCFontDb( )
	: m_ft( 0 )
{
	FT_Init_FreeType( &m_ft ) ;
}

FCFontDb::~FCFontDb( )
{
	FT_Done_FreeType( m_ft ) ;
}

FT_LibraryRec_* FCFontDb::Library()
{
	return m_ft ;
}

FT_FaceRec_* FCFontDb::LoadFont(
	const std::string& base_name,
	const std::string& style )
{
	FcPattern *sans = FcPatternBuild( NULL,
		FC_FAMILY,		FcTypeString, 	base_name.c_str(),
		FC_WEIGHT,		FcTypeInteger, 	FC_WEIGHT_NORMAL,
		FC_STYLE,		FcTypeString, 	style.c_str(),
		FC_WIDTH,		FcTypeInteger,	FC_WIDTH_NORMAL,
		FC_SCALABLE,	FcTypeBool,		true,
	    NULL ) ;
	if ( sans == 0 )
		throw FontException( "cannot create font pattern" ) ;

	FcResult result ;
	FcPattern *matched = FcFontMatch( 0, sans, &result ) ;

	FcChar8 *filename ;
	if ( FcPatternGetString(matched, FC_FILE, 0, &filename ) != FcResultMatch )
		throw FontException( "cannot find font " + base_name ) ;

	const char *file = reinterpret_cast<const char*>( filename ) ;
	if ( file == 0 )
		throw FontException( "cannot find font " + base_name ) ;
	
	int idx ;
	if ( FcPatternGetInteger( matched, FC_INDEX, 0, &idx ) != FcResultMatch )
		throw FontException( "cannot find font " + base_name ) ;

	// TODO: how to embed font with index != 0?
	if ( idx != 0 )
		throw FontException( "font collection is not supported yet" ) ;

	FT_Face face ;
	FT_Error e = FT_New_Face( m_ft, file, 0, &face ) ;

	return e != 0 ? face : 0 ;
}

} // end of namespace
