/***************************************************************************
 *   Copyright (C) 2006 by Nestal Wan                                      *
 *   me@nestal.net                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
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
	\file	SimpleFont.hh
	\brief	definition the SimpleFont class
	\date	Sat Mar 22 2008
	\author	Nestal Wan
*/

#ifndef __PDF_REAL_FONT_HEADER_INCLUDED__
#define __PDF_REAL_FONT_HEADER_INCLUDED__

#include "BaseFont.hh"

#include "core/Name.hh"
#include "core/Object.hh"

#include <vector>

namespace freetype
{
	class Library ;
	class Face ;
}

namespace pdf {

class Dictionary ;

/*!	\brief	general font for PDF files
	\internal

	This class represent a simple Type 1 or TrueType font.
*/
class SimpleFont : public BaseFont
{
public :
	enum Type { truetype, type1, mmtype1, type3, type0 } ;

public :
	SimpleFont( ) ;
	SimpleFont( freetype::Library *lib, const std::string& filename ) ;

	void Init( Object& link, ElementReader *src ) ;
	void Write( const Ref& link, IElementDest *dest ) const ;

	std::string BaseName( ) const ;

	ElementList GetChildren( ) const ;

private :
	SimpleFont( const Name& base_font, Type type ) ;

	static const Name m_font_types[] ;

	static const Name& SubType( Type t ) ;
	static Type        SubType( const Name& t ) ;

private :
	Name	m_base_font ;
	Type	m_type ;
	int		m_first_char, m_last_char ;

	std::vector<int>	m_widths ;
	Object				m_descriptor ;
	Object				m_to_unicode ;
	Object				m_encoding ;	//!< name or dictionary
} ;

} // end of namespace

#endif
