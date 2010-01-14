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

/**
    \file	TextState.hh
    \brief	definition the TextFormat class
    \date	Jan 4, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_TEXTFORMAT_HH_EADER_INCLUDED__
#define __PDF_TEXTFORMAT_HH_EADER_INCLUDED__

#include <cstddef>

namespace pdf {

class Font ;
class Resources ;

/**	\brief	brief description

	The TextFormat class represent
*/
class TextState
{
public :
	TextState( ) ;

	// compiler generated copy constructor and assignment operator are fine.

	void SetCharSpace( double val ) ;
	void SetWordSpace( double val ) ;
	void SetHScale( double val ) ;
	void SetLeading( double val ) ;
	void SetFont( double size, Font *font ) ;
	void SetRenderMode( int val ) ;
	void SetTextRise( double val ) ;

	double CharSpace( ) const ;
	double WordSpace( ) const ;
	double HScale( ) const ;
	double Leading( ) const ;
	double FontSize( ) const ;
	Font* GetFont( ) const ;
	int RenderMode( ) const ;
	double TextRise( ) const ;

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

} // end of namespace

#endif // TEXTFORMAT_HH_
