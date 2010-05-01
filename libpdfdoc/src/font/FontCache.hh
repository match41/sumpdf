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

/**	\file	FontCache.hh
    \brief	definition the FontCache class
    \date	May 1, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_FONTCACHE_HH_EADER_INCLUDED__
#define __PDF_FONTCACHE_HH_EADER_INCLUDED__

#include <map>
#include <memory>
#include <string>

struct FT_FaceRec_ ;

namespace pdf {

class BaseFont ;
class FontDb ;

///	brief description
/**	\internal
	The FontCache class represents
*/
class FontCache
{
public :
	FontCache( ) ;
	
	BaseFont* GetFont( FT_FaceRec_ *face ) ;
	BaseFont* GetFont(
		const unsigned char		*data,
		std::size_t 			size ) ;

	FontDb* Database() ;

private :
	//! font database for loading the font files.
	std::auto_ptr<FontDb>	m_font_db ;

	typedef std::map<std::string, BaseFont*>	FontMap ;
	FontMap	m_fonts ;
} ;

} // end of namespace

#endif // FONTCACHE_HH_
