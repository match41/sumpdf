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

/**	\file	MockFontDb.cc
	\brief	implementation of the MockFontDb class
	\date	Feb 12, 2010
	\author	Nestal Wan
*/

#include "MockFontDb.hh"

// freetype headers
#include <ft2build.h>
#include FT_FREETYPE_H

namespace pdfut {

using namespace pdf ;

/**	constructor
	
*/
MockFontDb::MockFontDb( )
	: m_ft( 0 )
{
	::FT_Init_FreeType( &m_ft ) ;
}

FT_LibraryRec_* MockFontDb::Library()
{
	return m_ft ;
}

FT_FaceRec_* MockFontDb::LoadFont(
	const std::string& base_name,
	const std::string& style )
{
	FT_Face face = 0 ;
	FT_Error e = FT_New_Face(
		m_ft,
		(std::string(TEST_DATA_DIR) +"FreeMonoBoldOblique.ttf").c_str(),
		0,
		&face ) ;
	return e != 0 ? face : 0 ;
}

} // end of namespace
