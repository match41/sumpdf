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

/**	\file	TextState.hh
    \brief	definition the TextFormat class
    \date	Jan 4, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_TEXTFORMAT_HEADER_INCLUDED__
#define __PDF_TEXTFORMAT_HEADER_INCLUDED__

#include "util/Export.hh"

#include <string>
#include <iosfwd>

namespace pdf {

class Font ;
class ResourcesDict ;

///	PDF text state.
/**	\ingroup graphics
	This class represents the text state of a PDF file. It contains properties
	like font size, spacing and other parameters affecting text appearance.
*/
class LIBPDFDOC_API TextState
{
public :
	explicit TextState( double size = 0, Font *font = 0 ) ;

	// compiler generated copy constructor and assignment operator are fine.

	void CharSpace( double val ) ;
	void WordSpace( double val ) ;
	void HScale( double val ) ;
	void Leading( double val ) ;
	void ChangeFont( double size, Font *font ) ;
	void RenderMode( int val ) ;
	void TextRise( double val ) ;

	double CharSpace( ) const ;
	double WordSpace( ) const ;
	double HScale( ) const ;
	double Leading( ) const ;
	double FontSize( ) const ;
	Font* FontFace( ) const ;
	int RenderMode( ) const ;
	double TextRise( ) const ;

	std::ostream& Print(
		std::ostream&		os,
		ResourcesDict		*res,
		const TextState& 	prev = TextState() ) const ;

	bool operator==( const TextState& rhs ) const ;
	bool operator!=( const TextState& rhs ) const ;

	double ScaleFactor( ) const ;
	double Width( const std::wstring& str ) const ;
	double Height( ) const ;

private :
	double	m_char_space ;
	double	m_word_space ;
	double	m_hori_scale ;
	double	m_leading ;
	double	m_font_size ;
	Font	*m_font ;
	int		m_render_mode ;
	double	m_text_rise ;
} ;

std::ostream& operator<<( std::ostream& os, const TextState& ts ) ;

} // end of namespace

#endif // TEXTFORMAT_HH_
