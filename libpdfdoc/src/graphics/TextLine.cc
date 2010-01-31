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
	\file	TextLine.cc
	\brief	implementation of the TextLine class
	\date	Jan 4, 2010
	\author	Nestal Wan
*/

#include "graphics/TextLine.hh"

#include "graphics/CharVisitor.hh"

#include "core/Object.hh"
#include "core/String.hh"
#include "core/Token.hh"
#include "font/Font.hh"
#include "font/Glyph.hh"
#include "util/Debug.hh"
#include "util/Exception.hh"
#include "util/Util.hh"

#include <algorithm>
#include <iterator>
#include <ostream>
#include <set>

#include <iostream>

namespace pdf {

///	constructor
TextLine::TextLine( const TextState& state, const Matrix& transform )
    : m_trans( transform ),
      m_state( state )
{
}

const Matrix& TextLine::Transform() const
{
	return m_trans ;
}

void TextLine::SetTransform( const Matrix& t )
{
	m_trans = t ;
}

bool TextLine::IsEmpty( ) const
{
	return m_text.empty( ) ;
}

void TextLine::AppendText( const std::wstring& text )
{
	Font *f = m_state.GetFont() ;
	if ( f == 0 )
		throw Exception( "invalid font" ) ;
	else
	{
		m_text.insert( m_text.end(), text.begin(), text.end() ) ;
	}
}

std::ostream& TextLine::Print(
	std::ostream& 	os,
	Matrix&			current,
	TextState& 		state,
	const Resources	*res ) const
{
	if ( m_trans.IsTranslate() && current.IsTranslate() )
		os	<< (m_trans.Dx()-current.Dx()) << ' ' 
			<< (m_trans.Dy()-current.Dy()) << " Td\n" ;
	else
		os	<< m_trans.M11() << ' ' << m_trans.M12() << ' '
			<< m_trans.M21() << ' ' << m_trans.M22() << ' '
			<< m_trans.Dx()  << ' ' << m_trans.Dy( ) << " Tm\n" ; 
	
	// replace current matrix
	current = m_trans ;
	
	m_state.Print( os, res, state ) ;   
	return
		os	<< String( std::string( m_text.begin(), m_text.end() ) )
			<< ' ' << "Tj\n" ;
}

std::ostream& operator<<( std::ostream& os, const TextLine& t )
{
	os << "<TextLine transform=\"" << t.Transform() << "\">\n" ;
//	std::copy(
//		t.begin(),
//		t.end(), 
//		std::ostream_iterator<TextBlock>( os, "\n" ) ) ;
	return os << "</TextLine>" ;
}

bool TextLine::operator==( const TextLine& rhs ) const
{
	return
		m_trans == rhs.m_trans &&
		m_state	== rhs.m_state &&
		m_text	== rhs.m_text ;
}

bool TextLine::operator!=( const TextLine& rhs ) const
{
	return !operator==( rhs ) ;
}

const TextState& TextLine::Format() const
{
	return m_state ;
}

void TextLine::SetFormat( const TextState& fmt )
{
	m_state = fmt ;
}

const std::wstring& TextLine::Text() const
{
	return m_text ;
}

/// Width of the text block in text space.
///	Return the width of the string in text space unit.
/**	This function calculates the width of a string in text space unit. It will
	be further transform to actual user space units by the text matrix. To
	convert it to glyph unit (PDF glyph space unit), multiply this value by
	1000.
	\return	The width of the string in text space unit.
	\sa	TextLine::Transform()
*/
double TextLine::Width( ) const
{
	return m_state.Width( m_text ) ;
}

void TextLine::VisitChars( CharVisitor *v ) const
{
	Matrix tm ;
	Font *font	= m_state.GetFont( ) ; 
	PDF_ASSERT( font != 0 ) ;
	
	for ( std::wstring::const_iterator
		i = m_text.begin() ; i != m_text.end() ; ++i )
	{
		const Glyph *glyph = font->GetGlyph( *i ) ;

		if ( glyph != 0 && glyph->IsOutline() )
		{
			v->OnChar( *i, tm, *glyph, m_state.ScaleFactor() ) ;

			// update X position
			tm.Dx( tm.Dx() + glyph->AdvanceX() * m_state.ScaleFactor() ) ;
		}
		else
		{
			// TODO: handle non-scalable font here
		}
	}
}

} // end of namespace
