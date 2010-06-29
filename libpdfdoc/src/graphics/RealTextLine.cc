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

#include "graphics/RealTextLine.hh"

#include "graphics/CharVisitor.hh"
#include "graphics/TextState.hh"

#include "core/Array.hh"
#include "core/Object.hh"
#include "core/String.hh"
#include "core/Token.hh"
#include "font/Font.hh"
#include "font/FontEncoding.hh"
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
#include <sstream>

namespace pdf {

///	constructor
RealTextLine::RealTextLine(
	const GraphicsState& 	state,
	const Matrix& 			transform,
	const std::wstring&		text )
    : m_trans( transform )
    , m_state( state )
    , m_text( text )
{
}

RealTextLine* RealTextLine::Clone( ) const
{
	return new RealTextLine( *this ) ;
}

const Matrix& RealTextLine::Transform() const
{
	return m_trans ;
}

void RealTextLine::SetTransform( const Matrix& t )
{
	m_trans = t ;
}

bool RealTextLine::IsEmpty( ) const
{
	return m_text.empty( ) ;
}

void RealTextLine::AppendText( const std::wstring& text )
{
	Font *f = m_state.FontFace() ;
	if ( f != 0 )
		m_text.insert( m_text.end(), text.begin(), text.end() ) ;
}

void RealTextLine::AddChar( double xpos, wchar_t ch )
{
	// calculate the space need to be advanced
	double adv = xpos - Width() ;
	if ( adv != 0 )
		AppendSpace( -adv ) ;
	
	PDF_ASSERT_EQUAL( Width(), xpos ) ;

	m_text.push_back( ch ) ;
}

std::ostream& RealTextLine::Print(
	std::ostream& 	os,
	Matrix&			current,
	GraphicsState& 	prev_gs,
	ResourcesDict	*res ) const
{
	// print transformation if different
	if ( m_trans != current )
	{
		// translate only
		if ( m_trans.M11() == current.M11() &&
			 m_trans.M12() == current.M12() &&
			 m_trans.M21() == current.M21() &&
			 m_trans.M22() == current.M22() && 
			 m_trans.M11() == 1 &&
			 m_trans.M12() == 0 &&
			 m_trans.M21() == 0 &&
			 m_trans.M22() == 1 )
			os	<< (m_trans.Dx() - current.Dx()) << ' '
				<< (m_trans.Dy() - current.Dy()) << " Td\n" ;
		else
			os	<< m_trans.M11() << ' ' << m_trans.M12() << ' '
				<< m_trans.M21() << ' ' << m_trans.M22() << ' '
				<< m_trans.Dx()  << ' ' << m_trans.Dy( ) << " Tm\n" ; 
	}

	// replace current matrix and position
	current = m_trans ;
	
	m_state.Print( os, res, prev_gs ) ;
	prev_gs = m_state ;
	
	return PrintText( os ) ;
}

std::ostream& RealTextLine::PrintText( std::ostream& os ) const
{
	if ( m_space.empty() )
		return
			os	<< String( EncodeText( m_text.begin(), m_text.end() ) )
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

				a.push_back( EncodeText(
					m_text.begin() + idx,
					m_text.begin() + i->index ) ) ;

				idx = i->index ;
			}
			
			a.push_back( i->width * 1000.0 / m_state.Text().FontSize() ) ;
		}
		
		if ( idx < m_text.size() )
			a.push_back( std::string( m_text.begin() + idx, m_text.end() ) ) ;
		
		return os << a << " TJ\n" ;
	}
}

std::string RealTextLine::EncodeText(
	std::wstring::const_iterator first,
	std::wstring::const_iterator last ) const
{
	Font *f = m_state.FontFace() ;
	PDF_ASSERT( f != 0 ) ;
	
	std::ostringstream oss ;
	if ( f->Encoding() != 0 )
		f->Encoding()->Encode( first, last, oss ) ;
	else
		oss << std::string( first, last ) ;
	
	return oss.str() ;
}

std::ostream& operator<<( std::ostream& os, const RealTextLine& t )
{
	os << "<TextLine transform=\"" << t.Transform() << "\">\n" ;
	t.PrintText( os ) ;
	return os << "</TextLine>" ;
}

bool RealTextLine::operator==( const RealTextLine& rhs ) const
{
	return
		m_trans == rhs.m_trans	&&
		m_state	== rhs.m_state	&&
		m_text	== rhs.m_text ;
}

bool RealTextLine::operator!=( const RealTextLine& rhs ) const
{
	return !operator==( rhs ) ;
}

const GraphicsState& RealTextLine::Format() const
{
	return m_state ;
}

void RealTextLine::SetFormat( const GraphicsState& fmt )
{
	m_state = fmt ;
}

const std::wstring& RealTextLine::Text() const
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
double RealTextLine::Width( ) const
{
	return m_state.Text().Width( m_text ) -
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

double RealTextLine::Height( ) const
{
	return m_state.Text().Height( ) ;
}

void RealTextLine::VisitChars( CharVisitor *v ) const
{
	double	offset = 0.0 ;
	Font *font	= m_state.FontFace( ) ; 
	
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

		const TextState& ts = m_state.Text() ;

		if ( glyph != 0 && glyph->IsOutline() )
		{
			v->OnChar( m_text[idx], offset, glyph, ts ) ;

			// update X position
			offset += glyph->AdvanceX() * ts.ScaleFactor() ;
		}
		else
		{
			// TODO: handle non-scalable font here
std::cout << "can't find char_code in font " << font->BaseName() << " = " << (unsigned)m_text[idx] << std::endl ;
		}
	}
}

void RealTextLine::AppendSpace( double width )
{
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
