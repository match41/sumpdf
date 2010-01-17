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

/**	\file	FontPool.hh
    \brief	definition the FontPool class
    \date	Jan 17, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_FONTPOOL_HH_EADER_INCLUDED__
#define __PDF_FONTPOOL_HH_EADER_INCLUDED__

#include <ft2build.h>
#include FT_CACHE_H

namespace pdf {

class BaseFont ;

///	brief description
/**	The FontPool class represents
*/
class FontPool
{
public :
	explicit FontPool( FT_Library lib ) ;
	~FontPool( ) ;

	FT_Face GetFace( BaseFont *font ) ;
	
	FT_Glyph GetGlyph( FT_Face face, wchar_t ch ) ;

private :
	static FT_Error RequestFace(
		FTC_FaceID	face_id,
		FT_Library	library,
		FT_Pointer	request_data,
		FT_Face		*aface ) ;

private :
	FT_Library		m_ft ;
	FTC_Manager		m_mgr ;
	FTC_ImageCache	m_img ;
	FTC_CMapCache	m_cmap ;
} ;

} // end of namespace

#endif // FONTPOOL_HH_
