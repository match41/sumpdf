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

/**	\file	FCFontDb.hh
    \brief	definition the FCFontDb class
    \date	Feb 11, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_FCFONTDB_HEADER_INCLUDED__
#define __PDF_FCFONTDB_HEADER_INCLUDED__

#include "font/FontDb.hh"

namespace pdf {

///	brief description
/**	\internal
	The FCFontDb class represents
*/
class FCFontDb : public FontDb
{
public :
	FCFontDb( ) ;
	~FCFontDb( ) ;

	FT_LibraryRec_* Library() ;
	
	std::vector<unsigned char> FindFont(
		const std::string& base_name,
		const std::string& style ) ;

	FT_FaceRec_* LoadFont(
		const unsigned char	*data,
		std::size_t			size ) ;

private :
	FT_LibraryRec_	*m_ft ;
} ;

} // end of namespace

#endif // FCFONTDB_HH_
