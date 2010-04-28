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

/**	\file	Type1Encoding.cc
	\brief	implementation of the Type1Encoding class
	\date	Apr 24, 2010
	\author	Nestal Wan
*/

#include "Type1Encoding.hh"

#include "CodeMap.hh"

#include "util/Util.hh"

// freetype headers
#include <ft2build.h>
#include FT_FREETYPE_H

#include <iostream>

namespace pdf {

/**	constructor
	
*/
Type1Encoding::Type1Encoding( FT_Face face )
{
	// select an non-unicode charmap first
	for ( int i = 0 ; i < face->num_charmaps ; i++ )
	{
		if ( face->charmaps[i]->encoding != FT_ENCODING_UNICODE )
		{
			FT_Select_Charmap( face, face->charmaps[i]->encoding ) ;
			break ;
		}
	}
	// traverse all characters
	unsigned		gindex ;
	unsigned long 	char_code = ::FT_Get_First_Char( face, &gindex ) ;

	while ( gindex != 0 )
	{
		char name[1024] = {} ;
		if ( ::FT_Get_Glyph_Name( face, gindex, name, sizeof(name) ) == 0 )
		{
			wchar_t unicode = 0 ;
			if ( NameToUnicode( name, unicode ) && char_code < Count(m_map) )
			{
				m_map[char_code] = unicode ;
				std::cout << "glyph: " << name << " char: " << char_code << " "
				<< "unicode: " << (int)unicode << std::endl ;
			}
		}
		
		char_code = ::FT_Get_Next_Char( face, char_code, &gindex ) ;
	}
}

std::wstring Type1Encoding::Decode( const std::string& bytes ) const
{
	std::wstring rs ;
	for ( std::string::const_iterator i = bytes.begin() ; i != bytes.end() ; ++i )
		rs.push_back( m_map[*i] ) ;
	return rs ;
}

std::size_t Type1Encoding::Encode(
	std::wstring::const_iterator first,
	std::wstring::const_iterator last,
	std::ostream& out ) const
{
	return 0 ;
}

} // end of namespace
