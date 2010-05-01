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

/**	\file	BasicFontDb.hh
    \brief	definition the BasicFontDb class
    \date	Feb 14, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_BASICFONTDB_HH_EADER_INCLUDED__
#define __PDF_BASICFONTDB_HH_EADER_INCLUDED__

#include "font/FontDb.hh"

#include <map>
#include <string>

struct FT_LibraryRec_ ;

namespace pdf {

///	brief description
/**	\internal
	The BasicFontDb class represents
*/
class BasicFontDb : public FontDb 
{
public :
	BasicFontDb( ) ;
	~BasicFontDb( ) ;

	FT_LibraryRec_* Library() ;

	FT_FaceRec_* LoadFont(
		const unsigned char	*data,
		std::size_t			size ) ;

protected :
	FT_LibraryRec_	*m_ft ;
	
	struct FontEntry ;
	typedef std::map<std::string, FontEntry*>	FontMap ;
	FontMap	m_map ;
	
	typedef std::vector<FontEntry*> NamelessFont ;
	NamelessFont	m_nameless ;
} ;

} // end of namespace

#endif // BASICFONTDB_HH_
