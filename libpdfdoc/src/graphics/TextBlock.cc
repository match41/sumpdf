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
	\file	TextBlock.cc
	\brief	implementation of the TextBlock class
	\date	Jan 4, 2010
	\author	Nestal Wan
*/

#include "graphics/TextBlock.hh"

#include "graphics/CharVisitor.hh"

#include "core/String.hh"
#include "font/Font.hh"
#include "font/Glyph.hh"
#include "util/Debug.hh"
#include "util/Matrix.hh"

#include <iostream>

namespace pdf {

///	default constructor
TextBlock::TextBlock( const std::string& text, const TextState& format )
	: m_chars( text.begin(), text.end() ),
	  m_format( format )
{
}

const TextState& TextBlock::Format() const
{
	return m_format ;
}

void TextBlock::SetFormat( const TextState& fmt )
{
	m_format = fmt ;
}

bool TextBlock::IsEmpty() const
{
	return m_chars.empty() ;
}

const std::wstring& TextBlock::Text() const
{
	return m_chars ;
}

void TextBlock::SetText( const std::wstring& text )
{
	m_chars = text ;
}

void TextBlock::AppendText( const std::wstring& text )
{
	m_chars.insert( m_chars.end(), text.begin(), text.end() ) ;
}

std::ostream& TextBlock::Print(
	std::ostream& 		os,
	const TextState& 	state,
	const Resources		*res ) const
{
	m_format.Print( os, res, state ) ;   
	std::string s( m_chars.begin(), m_chars.end() ) ;
	return os << String( s ) << ' ' << "Tj " ;
}

std::ostream& operator<<( std::ostream& os, const TextBlock& t )
{
	const std::wstring& str = t.Text() ; 
	return os 	<< "<TextBlock>\n<text>" << std::string(str.begin(), str.end())
				<< "</text>\n" << t.Format() << "</TextBlock>\n" ;
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
double TextBlock::Width( ) const
{
	return m_format.Width( m_chars ) ;
}

bool TextBlock::operator==( const TextBlock& rhs ) const
{
	return
		m_chars		== rhs.m_chars	&&
		m_format	== rhs.m_format ;
}

bool TextBlock::operator!=( const TextBlock& rhs ) const
{
	return !operator==( rhs ) ;
}

void TextBlock::VisitChars( CharVisitor *v ) const
{
	Matrix tm ;
	Font *font	= Format().GetFont( ) ; 
	PDF_ASSERT( font != 0 ) ;
	
	for ( std::wstring::const_iterator
		i = m_chars.begin() ; i != m_chars.end() ; ++i )
	{
		const Glyph *glyph = font->GetGlyph( *i ) ;

		if ( glyph != 0 && glyph->IsOutline() )
		{
			v->OnChar( m_chars[*i], tm, *glyph, m_format.ScaleFactor() ) ;

			// update X position
			tm.Dx( tm.Dx() + glyph->AdvanceX() * m_format.ScaleFactor() ) ;
		}
		else
		{
			// TODO: handle non-scalable font here
		}
	}
}

} // end of namespace
