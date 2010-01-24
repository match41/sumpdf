/***************************************************************************
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
 ***************************************************************************/

/*!
	\file	Font.hh
	\brief	definition the Font class
	\date	Fri Apr 11 2008
	\author	Nestal Wan
*/

#ifndef __PDF_FONT_HEADER_INCLUDED__
#define __PDF_FONT_HEADER_INCLUDED__

#include <string>

// freetype headers
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

namespace pdf {

class Glyph ;

/*!	\brief	brief description
	
	this class represents
*/
class Font
{
public :
	virtual ~Font( ) ;
	
	virtual std::string BaseName( ) const = 0 ;
	
	virtual FT_Face		Face( ) const = 0 ;
//	virtual FT_Glyph	GetGlyph( wchar_t ch, FT_Glyph_Metrics *met ) const =0;
	virtual const Glyph*	GetGlyph( wchar_t ch ) const = 0 ;
	
	virtual double Width( const std::wstring& text, double size ) const = 0 ;
	virtual double Width( const Glyph& glyph ) const = 0 ;
	virtual unsigned UnitsPerEM( ) const = 0 ;
} ;

} // end of namespace

#endif
