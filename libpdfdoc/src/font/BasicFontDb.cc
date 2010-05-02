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

/**	\file	BasicFontDb.cc
	\brief	implementation of the BasicFontDb class
	\date	Feb 14, 2010
	\author	Nestal Wan
*/

#include "BasicFontDb.hh"

#include "FontException.hh"

#include "util/Debug.hh"
#include "util/Util.hh"

#include <boost/format.hpp>

// freetype headers
#include <ft2build.h>
#include FT_FREETYPE_H

namespace pdf {

/**	constructor
	
*/
BasicFontDb::BasicFontDb( )
	: m_ft( 0 )
{
	FT_Init_FreeType( &m_ft ) ;
}

BasicFontDb::~BasicFontDb( )
{
	FT_Done_FreeType( m_ft ) ;
}

FT_LibraryRec_* BasicFontDb::Library()
{
	return m_ft ;
}

FT_FaceRec_* BasicFontDb::LoadFont(
	const unsigned char	*data,
	std::size_t			size )
{
	PDF_ASSERT( data != 0 ) ;
	PDF_ASSERT( size > 0 ) ;

	FT_Face face = 0 ;
	FT_Error e = FT_New_Memory_Face( m_ft, data, size, 0, &face ) ;
	
	using boost::format ;
	if ( e != 0 )
		throw FontException( format("cannot create font face: %1%") % e ) ;
	
	return face ;
}

} // end of namespace
