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

#include "core/Name.hh"
#include "font/BaseFont.hh"
#include "font/Glyph.hh"
#include "page/ResourcesDict.hh"
#include "util/Debug.hh"

#include <cassert>
#include <iostream>

namespace pdf {

/**	Constructor.
*/
TextState::TextState( double size, Font *font )
	: m_font_size( size )
	, m_font( font )
{
	m_char_space = m_word_space = m_hori_scale = m_leading = 
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
	ResourcesDict		*res,
	const TextState& 	prev ) const
{
	PDF_ASSERT( res != 0 ) ;
	PDF_ASSERT( dynamic_cast<BaseFont*>(m_font) != 0 ) ;

	if ( m_font_size	!= prev.m_font_size ||
		 m_font			!= prev.m_font )
	{
		Name fname = res->AddFont( static_cast<BaseFont*>(m_font) ) ;
		PDF_ASSERT( !fname.empty( ) ) ;
		
		os	<< fname << ' ' << m_font_size << " Tf\n" ;
	}
	return os ;
}

std::ostream& operator<<( std::ostream& os, const TextState& ts )
{
	return os	<< "<TextState font=\"" << ts.GetFont()->BaseName() << "\" "
				<< "size=\"" << ts.FontSize() << "\" />\n" ;
}

bool TextState::operator==( const TextState& rhs ) const
{
	return 
		m_char_space	== rhs.m_char_space	&&
		m_word_space	== rhs.m_word_space	&&
		m_hori_scale	== rhs.m_hori_scale	&&
		m_leading		== rhs.m_leading		&&
		m_font_size		== rhs.m_font_size		&&
		m_font 			== rhs.m_font 			&&
		m_render_mode	== rhs.m_render_mode	&&
		m_text_rise		== rhs.m_text_rise	;
}

bool TextState::operator!=( const TextState& rhs ) const
{
	return !operator==( rhs ) ;
}

///	Returns the scale factor.
/**	The scale factor is the factor that transform glyph unit to text space
	unit directly. For example, if \a i is in glyph unit \a i * ScaleFactor()
	will be in text space unit.
	\return	The scale factor.
*/
double TextState::ScaleFactor( ) const
{
	PDF_ASSERT( m_font != 0 ) ;
	return m_font_size / m_font->UnitsPerEM( ) ;
}

double TextState::Width( const std::wstring& str ) const
{
	double font_unit = 0.0 ;
	
	for ( std::wstring::const_iterator i = str.begin() ; i < str.end() ; ++i )
	{
		const Glyph *g = m_font->GetGlyph( *i ) ;
		if ( g != 0 )
			font_unit += g->AdvanceX() ;
	}
	
	return font_unit * ScaleFactor() ;
}

} // end of namespace
