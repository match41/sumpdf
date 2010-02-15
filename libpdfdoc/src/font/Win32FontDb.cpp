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

/**	\file	Win32FontDb.cc
	\brief	implementation of the Win32FontDb class
	\date	Feb 14, 2010
	\author	Nestal Wan
*/

#include "Win32FontDb.hpp"

#include "FontException.hh"

#include "util/Debug.hh"

#include <cstring>

#include <windows.h>

namespace pdf {

namespace
{
	int weight_map[] =
	{
		FW_THIN,	 	// thin,
		FW_EXTRALIGHT,	// extra_light,
		FW_ULTRALIGHT,	// ultra_light,
		FW_LIGHT,	 	// light,
		FW_LIGHT,	 	// book,
		FW_REGULAR,	 	// regular_weight,
		FW_NORMAL,	 	// normal_weight,
		FW_MEDIUM,	 	// medium_weight,
		FW_DEMIBOLD,	// demi_bold,
		FW_SEMIBOLD,	// semi_bold,
		FW_BOLD,	 	// bold,
		FW_EXTRABOLD,	// extra_bold,
		FW_ULTRABOLD,	// ultra_bold,
		FW_BLACK,	 	// black,
		FW_HEAVY,	 	// heavy,
		FW_BLACK,		// extra_black,
		FW_BLACK,		// ultra_black,
	} ;
}

/**	constructor
	
*/
Win32FontDb::Win32FontDb( )
{
}

std::vector<unsigned char> Win32FontDb::FindFont( 
	const std::string& base_name,
	font::Weight		weight,
	font::Slant			slant,
	font::Width			width )
{
	PDF_ASSERT( weight >= font::thin && weight <= font::ultra_black ) ;
	PDF_ASSERT(
		width >= font::ultra_condensed &&
		width <= font::ultra_expanded ) ;

	HDC hdc = CreateCompatibleDC( NULL ) ;
	HFONT hfont = CreateFont(
		0,						// height
		0,						// width,
		0,						// escapement
		0,						// orientation
		weight_map[width],		// weight
		slant != font::roman ? TRUE : FALSE,	// italic
		FALSE,					// underline
		FALSE,					// strikeout
		DEFAULT_CHARSET,		// charset
		OUT_DEFAULT_PRECIS,		// match precision
		CLIP_DEFAULT_PRECIS,	// clip precision
		DEFAULT_QUALITY,		// quality
		DEFAULT_PITCH,			// pitch
		base_name.c_str() ) ;	// family name
	
	SelectObject( hdc, (HGDIOBJ)hfont ) ;
	DWORD size = GetFontData( hdc, 0, 0, 0, 0 ) ;
	if ( size != GDI_ERROR )
	{
		std::vector<unsigned char> result( size ) ;
		GetFontData( hdc, 0, 0, &result[0], size ) ;
		return result ;
	}
	return std::vector<unsigned char>( ) ;	
}

} // end of namespace
