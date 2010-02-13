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

/**	\file	MockFontDb.hh
    \brief	definition the MockFontDb class
    \date	Feb 12, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_MOCKFONTDB_HH_EADER_INCLUDED__
#define __PDF_MOCKFONTDB_HH_EADER_INCLUDED__

#include "font/FontDb.hh"

struct FT_LibraryRec_ ;

namespace pdfut {

///	brief description
/**	\internal
	The MockFontDb class represents
*/
class MockFontDb : public pdf::FontDb
{
public :
	MockFontDb( ) ;
	
	FT_LibraryRec_* Library() ;
	
	std::vector<unsigned char> FindFont(
		const std::string& base_name,
		const std::string& style ) ;

	FT_FaceRec_* LoadFont(
		const unsigned char	*data,
		std::size_t			size ) ;
	
private :
	FT_LibraryRec_* m_ft ;
} ;

} // end of namespace

#endif // MOCKFONTDB_HH_
