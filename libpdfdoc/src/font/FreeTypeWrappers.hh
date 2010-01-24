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

/**	\file	FreeTypeWrappers.hh
    \brief	definition the FreeTypeWrappers class
    \date	Jan 24, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_FREETYPEWRAPPERS_HH_EADER_INCLUDED__
#define __PDF_FREETYPEWRAPPERS_HH_EADER_INCLUDED__

// freetype headers
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

namespace pdf {

namespace ft
{
	struct Face
	{
		FT_Face		face ;
	} ;
	
	struct Library
	{
		FT_Library	lib ;
	} ;
	
	struct Glyph
	{
		FT_Glyph	glyph ;
	} ;
}

} // end of namespace

#endif // FREETYPEWRAPPERS_HH_
