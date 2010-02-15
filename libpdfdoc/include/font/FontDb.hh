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

/**	\file	FontDb.hh
    \brief	definition the FontDb class
    \date	Feb 11, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_FONTDB_HH_EADER_INCLUDED__
#define __PDF_FONTDB_HH_EADER_INCLUDED__

#include "FontType.hh" 

#include <memory>
#include <string>
#include <vector>

struct FT_LibraryRec_ ;
struct FT_FaceRec_ ;

namespace pdf {

///	Font database.
/**	\internal
	The FontDb class is an interface for searching and loading fonts. It keeps
	the Freetype library pointer and uses it for loading font.
*/
class FontDb
{
public :
	virtual ~FontDb( ) ;
	
	virtual FT_LibraryRec_* Library() = 0 ;
	
	virtual std::vector<unsigned char> FindFont( 
		const std::string&	base_name,
		font::Weight		weight	= font::normal_weight,
		font::Slant			slant	= font::roman,
		font::Width			width	= font::normal_width ) = 0 ;
	
	virtual FT_FaceRec_* LoadFont(
		const unsigned char	*data,
		std::size_t			size ) = 0 ;
} ;

std::auto_ptr<FontDb> CreateFontDb( ) ;

} // end of namespace

#endif // FONTDB_HH_
