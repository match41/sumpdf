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

/**	\file	CompositeFont.cc
	\brief	implementation of the CompositeFont class
	\date	Feb 15, 2010
	\author	Nestal Wan
*/

#include "CompositeFont.hh"

#include "SimpleFont.hh"

#include "core/Ref.hh"

#include "file/DictReader.hh"
#include "file/ArrayReader.hh"

#include "font/FontException.hh"

#include <iostream>

namespace pdf {

/**	constructor
	
*/
CompositeFont::CompositeFont( DictReader& dict, FontDb *ft )
{
	if ( !dict.Detach( "BaseFont", m_base_font ) )
		throw FontException( "missing BaseFont in Type0 font dictionary" ) ;
	
	ArrayReader descendant ;
	if ( !dict.Detach( "DescendantFonts", descendant ) ||
		descendant->size() == 0 )
		throw FontException("missing DescendantFonts in Type0 font dictionary");

	DictReader cidfont ;
	descendant.Detach( 0, cidfont ) ;
	std::cout << *cidfont << std::endl ;
	
	// trying to reuse the code in SimpleFont
//	SimpleFont *sfont = new SimpleFont( cidfont, ft ) ;
}

CompositeFont::CompositeFont( const std::string& name, FontDb *ft )
{
	
}

// BaseFont virtual functions
Ref CompositeFont::Write( File *file ) const
{
	return Ref() ;
}

FontDescriptor* CompositeFont::Descriptor( )
{
	return 0 ;
}

std::string CompositeFont::BaseName( ) const
{
	return m_base_font.Str() ;
}

const Glyph* CompositeFont::GetGlyph( wchar_t ch ) const
{
	return 0 ;
}

unsigned CompositeFont::UnitsPerEM( ) const
{
	return 1 ;
}

double	CompositeFont::FromFontUnit( unsigned val ) const
{
	return val ;
}

} // end of namespace
