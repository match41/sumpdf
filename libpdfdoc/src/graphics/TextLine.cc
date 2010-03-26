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
#include "graphics/TextState.hh"

#include "core/Array.hh"
#include "core/Object.hh"
#include "core/String.hh"
#include "core/Token.hh"
#include "font/Font.hh"
#include "font/Glyph.hh"
#include "util/Debug.hh"
#include "util/Exception.hh"
#include "util/Util.hh"

#include <boost/bind.hpp>

#include <algorithm>
#include <numeric>
#include <iterator>
#include <functional>
#include <ostream>
#include <set>

#include <iostream>
#include <iomanip>

namespace pdf {

///	constructor
TextLine::TextLine(
//	double xpos, double ypos,
	const GraphicsState& 	state,
	const Matrix& 			transform,
	const std::wstring&		text )
    : m_trans( transform )
    , m_state( state )
    , m_text( text )
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
	if ( f != 0 )
		m_text.insert( m_text.end(), text.begin(), text.end() ) ;
}

std::ostream& TextLine::Print(
	std::ostream& 			os,
	Matrix&					current,
	const GraphicsState& 	state,
	ResourcesDict			*res ) const
{
	// print transformation if different
	if ( m_trans != current )
	{
		os	<< m_trans.M11() << ' ' << m_trans.M12() << ' '
			<< m_trans.M21() << ' ' << m_trans.M22() << ' '
			<< m_trans.Dx()  << ' ' << m_trans.Dy( ) << " Tm\n" ; 
	}

	// replace current matrix and position
	current = m_trans ;
	
	m_state.Print( os, res, state ) ;
	return PrintText( os ) ;
}

std::ostream& TextLine::PrintText( std::ostream& os ) const
{
	if ( m_space.empty() )
		return
			os	<< String( std::string( m_text.begin(), m_text.end() ) )
				<< " Tj\n" ;
	else
	{
		Array a ;
		std::size_t idx = 0 ;
		for ( std::vector<Space>::const_iterator i = m_space.begin() ;
			i != m_space.end() ; ++i )
		{
			if ( i->index != 0 )
			{
				PDF_ASSERT( i->index > idx ) ;
				
				a.push_back( std::string(
					m_text.begin() + idx,
					m_text.begin() + i->index ) ) ;
				
				idx = i->index ;
			}
			
			a.push_back( i->width * 1000.0 / m_state.GetTextState().FontSize() ) ;
		}
		
		if ( idx < m_text.size() )
			a.push_back( std::string( m_text.begin() + idx, m_text.end() ) ) ;
		
		return os << a << " TJ\n" ;
	}
}

std::ostream& operator<<( std::ostream& os, const TextLine& t )
{
	os << "<TextLine transform=\"" << t.Transform() << "\">\n" ;
	t.PrintText( os ) ;
	return os << "</TextLine>" ;
}

bool TextLine::operator==( const TextLine& rhs ) const
{
	return
//		m_xpos	== rhs.m_xpos	&&
//		m_ypos	== rhs.m_ypos	&&
		m_trans == rhs.m_trans	&&
		m_state	== rhs.m_state	&&
		m_text	== rhs.m_text ;
}

bool TextLine::operator!=( const TextLine& rhs ) const
{
	return !operator==( rhs ) ;
}

const GraphicsState& TextLine::Format() const
{
	return m_state ;
}

void TextLine::SetFormat( const GraphicsState& fmt )
{
	m_state = fmt ;
}

const std::wstring& TextLine::Text() const
{
	return m_text ;
}

///	Returns the width of the string in text space unit.
/**	This function calculates the width of a string in text space unit. It will
	be further transform to actual user space units by the text matrix. To
	convert it to glyph unit (PDF glyph space unit), multiply this value by
	1000.
	\return	The width of the string in text space unit.
	\sa	TextLine::Transform()
*/
double TextLine::Width( ) const
{
	return m_state.GetTextState().Width( m_text ) -
		std::accumulate(
			m_space.begin(),
			m_space.end(),
			0.0,
			
			// _1 + _2.width
			boost::bind(
				std::plus<double>(),
				_1,
				boost::bind( &Space::width, _2 ) ) ) ;
}

void TextLine::VisitChars( CharVisitor *v ) const
{
	double	offset = 0.0 ;
	Font *font	= m_state.GetFont( ) ; 
	
	std::vector<Space>::const_iterator sp = m_space.begin( ) ;
	
	for ( std::size_t idx = 0 ; idx < m_text.size() ; ++idx )
	{
		// has space?
		if ( sp != m_space.end() && idx == sp->index )
		{
			offset -= sp->width ;
			++sp ;
		}
	
		PDF_ASSERT( font != 0 ) ;
		const Glyph *glyph = font->GetGlyph( m_text[idx] ) ;

		const TextState& ts = m_state.GetTextState() ;

		if ( glyph != 0 && glyph->IsOutline() )
		{
			v->OnChar( m_text[idx], offset, glyph, ts ) ;

			// update X position
			offset += glyph->AdvanceX() * ts.ScaleFactor() ;
		}
		else
		{
			// TODO: handle non-scalable font here
//std::cout << "can't find char_code = " << (unsigned)m_text[idx] << std::endl ;
		}
	}
}

void TextLine::AppendSpace( double width )
{
//	PDF_ASSERT( m_space.empty() || m_space
	
	// no text, just translate the matrix
	if ( m_text.empty() )
	{
		// TODO: vertical writing mode
		Matrix m( 1,0,0,1, width, 0 ) ;
		m_trans = m_trans * m ;
	}
	// there is a space at the end of the string, no need to add a new one.
	// just merge them as one.
	if ( !m_space.empty() && m_space.back().index == m_text.size() )
		m_space.back().width += width ;
	
	else
	{
		Space spc = { m_text.size(), width } ;
		m_space.push_back( spc ) ;
	}
}

} // end of namespace
