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

#include "Win32FontDb.hh"

#include "FontException.hh"

#include <windows.h>

#include <cstring>

namespace pdf {

/**	constructor
	
*/
Win32FontDb::Win32FontDb( )
{
}

std::vector<unsigned char> Win32FontDb::FindFont( 
	const std::string& base_name,
	const std::string& style )
{
	HDC hdc = CreateCompatibleDC( NULL ) ;
	HFONT hfont = CreateFont(
		0,				// height
		0,				// width,
		0,				// escapement
		0,				// orientation
		FW_REGULAR,		// weight
		stricmp( style.c_str(), "italic" ) == 0,	// italic
		FALSE,			// underline
		FALSE,			// strikeout
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
