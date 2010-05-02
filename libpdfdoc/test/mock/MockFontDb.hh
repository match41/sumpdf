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

#include "font/BasicFontDb.hh"

struct FT_LibraryRec_ ;

namespace pdfut {

///	brief description
/**	\internal
	The MockFontDb class represents
*/
class MockFontDb : public pdf::BasicFontDb
{
public :
	MockFontDb( ) ;
	
	std::vector<unsigned char> FindFont(
		const std::string&	base_name,
		pdf::font::Weight	weight,
		pdf::font::Slant	slant,
		pdf::font::Width	width ) ;

	std::string FindFontPath( FT_FaceRec_ *face ) ;

	std::string LastQueryName( ) const ;
	pdf::font::Weight LastQueryWeight( ) const ;
	pdf::font::Slant LastQuerySlant( ) const ;
	pdf::font::Width LastQueryWidth( ) const ;

private :
	std::string			m_basename ;
	pdf::font::Weight	m_weight ;
	pdf::font::Slant	m_slant ;
	pdf::font::Width	m_width ;
} ;

} // end of namespace

#endif // MOCKFONTDB_HH_
