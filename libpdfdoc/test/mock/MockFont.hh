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

/**	\file	MockFont.hh
    \brief	definition the MockFont class
    \date	Jan 18, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_MOCKFONT_HH_EADER_INCLUDED__
#define __PDF_MOCKFONT_HH_EADER_INCLUDED__

#include "font/BaseFont.hh"

#include "MockGlyph.hh"

namespace pdf
{
	class FontDescriptor ;
}

namespace pdfut {

///	brief description
/**	The MockFont class represents
*/
class MockFont : public pdf::BaseFont
{
public :
	MockFont( ) ;

	// whatever... I don't care
	template <typename T>
	MockFont( T ) { }

	std::string BaseName( ) const ;
	pdf::Ref Write( pdf::File *file, const pdf::FontSubsetInfo * ) const ;
	pdf::FontDescriptor* Descriptor( ) ;
	pdf::FontEncoding* Encoding( ) ;
	const pdf::Glyph*	GetGlyph( wchar_t ch ) const ;
	double FromFontUnit( unsigned val ) const ; 
	unsigned UnitsPerEM() const ;
	double Height( ) const ;

private :
	MockGlyph	m_glyph ;
} ;

} // end of namespace

#endif // MOCKFONT_HH_
