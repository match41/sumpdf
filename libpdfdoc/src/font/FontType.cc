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

/**	\file	FontType.cc
	\brief	implementation of the FontType class
	\date	Jan 28, 2010
	\author	Nestal Wan
*/

#include "font/FontType.hh"

#include "FontException.hh"
#include "util/Util.hh"

#include <cstring>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_XFREE86_H

namespace pdf { namespace font {

/**	constructor
*/
Type GetType( FT_FaceRec_ *face )
{
	const char *format = ::FT_Get_X11_Font_Format( face ) ;

	if ( format == 0 )
		return unknown ;
	
	else if ( ::strcasecmp( format, "Truetype" ) == 0 )
		return truetype ;
	
	else if ( ::strcasecmp( format, "Type 1" ) == 0 )
		return type1 ;
	
	// treat as type1
	else if ( ::strcasecmp( format, "PCF" ) == 0 )
		return type1 ;
	
	// OpenType CFF
	else if ( ::strcasecmp( format, "CFF" ) == 0 )
	{
		if ( FT_IS_SFNT( face ) )
			return opentype_cff ;
		else
			return type2 ;
	}
	else
		throw FontException()
			<< expt::ErrMsg( "unknown font type: " + std::string(format) ) ;
}

Weight ParseWeight( const char *str )
{
	Weight weight = font::normal_weight ;
	if ( ::strstr( str, "Bold" ) != 0 )
		weight = font::bold ;
	
	return weight ;
}

Slant ParseSlant( const char *str )
{
	Slant slant = font::roman ;
	if ( ::strstr( str, "Italic" ) != 0 )
		slant = font::italic ;
	else if ( ::strstr( str, "Oblique" ) != 0 )
		slant = font::oblique ;
	
	return slant ;
}

} } // end of namespace
