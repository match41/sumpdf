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
	\file	TextState.cc
	\brief	implementation of the TextFormat class
	\date	Jan 4, 2010
	\author	Nestal Wan
*/

#include "graphics/TextState.hh"

#include "font/BaseFont.hh"
#include "page/Resources.hh"
#include "util/Debug.hh"

#include <cassert>
#include <iostream>

namespace pdf {

/**	Constructor.
*/
TextState::TextState( )
	: m_font( 0 )
{
	m_char_space = m_word_space = m_hori_scale = m_leading = m_font_size =
	m_text_rise  = 0 ;
	m_render_mode = 0 ;
}

///	Returns the leading.
/**	The leading parameter is the vertical distance between the baselines
	of adjacent lines of text. It always applies to the vertical coordinate
	in text space, regardless of the writing mode.
	
	\return	The leading of current text state.
	\sa SetLeading()
*/
double TextState::Leading( ) const
{
	return m_leading ;
}

void TextState::SetLeading( double val )
{
	m_leading = val ;
}

void TextState::SetFont( double size, Font *font )
{
	assert( font != 0 ) ;

	m_font_size	= size ;
	m_font		= font ;
}

void TextState::SetCharSpace( double val )
{
	m_char_space = val ;
}

void TextState::SetWordSpace( double val )
{
	m_word_space = val ;
}

void TextState::SetHScale( double val )
{
	m_hori_scale = val ;
}

void TextState::SetRenderMode( int val )
{
	m_render_mode = val ;
}

void TextState::SetTextRise( double val )
{
	m_text_rise = val ;
}

double TextState::CharSpace( ) const
{
	return m_char_space ;
}

double TextState::WordSpace( ) const
{
	return m_word_space ;
}

double TextState::HScale( ) const
{
	return m_hori_scale ;
}

double TextState::FontSize( ) const
{
	return m_font_size ;
}

Font* TextState::GetFont( ) const
{
	return m_font ;
}

int TextState::RenderMode( ) const
{
	return m_render_mode ;
}

double TextState::TextRise( ) const
{
	return m_text_rise ;
}

std::ostream& TextState::Print(
	std::ostream& 		os,
	const Resources		*res,
	const TextState& 	prev ) const
{
	PDF_ASSERT( res != 0 ) ;
	PDF_ASSERT( dynamic_cast<const BaseFont*>(m_font) != 0 ) ;

	if ( m_font_size	!= prev.m_font_size ||
		 m_font			!= prev.m_font )
		os	<< res->FindFont( static_cast<const BaseFont*>(m_font) )
			<< ' ' << m_font_size << " Tf\n" ;

	return os ;
}

} // end of namespace
