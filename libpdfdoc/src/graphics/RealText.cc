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

/**	\file	RealText.cc
	\brief	implementation of the RealRealText class
	\date	Jan 13, 2010
	\author	Nestal Wan
*/

#include "RealText.hh"

#include "graphics/GraphicsVisitor.hh"

#include "core/Array.hh"
#include "core/Object.hh"
#include "core/Token.hh"
#include "font/BaseFont.hh"
#include "page/Resources.hh"
#include "util/Debug.hh"
#include "util/Util.hh"

#include <boost/bind.hpp>

#include <algorithm>
#include <iterator>
#include <iostream>
#include <set>

namespace pdf {

const RealText::HandlerMap::value_type	RealText::m_handler_map_values[] =
{
	// text positioning commands
	std::make_pair( "Td",	&RealText::OnTd ),
	std::make_pair( "TD",	&RealText::OnTD ),
	std::make_pair( "Tm",	&RealText::OnTm ),
	std::make_pair( "T*",	&RealText::OnTstar ),
	
	// text showing commands
	std::make_pair( "Tj",	&RealText::OnTj ),
	std::make_pair( "TJ",	&RealText::OnTJ ),
	std::make_pair( "\'",	&RealText::OnSingleQuote ),
	std::make_pair( "\"",	&RealText::OnDoubleQuote ),

	// text state commands
	std::make_pair( "Tf",	&RealText::OnTf ),
	std::make_pair( "TL",	&RealText::OnTL ),
} ;

const RealText::HandlerMap RealText::m_handler_map(
    Begin( RealText::m_handler_map_values ),
    End( RealText::m_handler_map_values ) ) ;

/**	constructor
*/
RealText::RealText( const TextState& ts )
	: m_lines( 1, TextLine( ts, Matrix() ) ),
	  m_state( ts )
{
}

RealText::iterator RealText::begin()
{
	return m_lines.begin( ) ;
}

RealText::iterator RealText::end()
{
	return m_lines.end( ) ;
}

RealText::const_iterator RealText::begin() const
{
	return m_lines.begin( ) ;
}

RealText::const_iterator RealText::end() const
{
	return m_lines.end( ) ;
}

TextLine& RealText::front()
{
	return m_lines.front() ;
}

TextLine& RealText::back()
{
	return m_lines.back() ;
}

const TextLine& RealText::front() const
{
	return m_lines.front() ;
}

const TextLine& RealText::back() const
{
	return m_lines.back() ;
}

const TextLine& RealText::at( std::size_t idx ) const
{
	return m_lines.at(idx) ;
}

TextLine& RealText::at( std::size_t idx )
{
	return m_lines.at(idx) ;
}

void RealText::OnCommand(
	const Token& 	cmd,
	Object 			*args,
	std::size_t		count,
	Resources		*res )
{
	PDF_ASSERT( !m_lines.empty() ) ;

	HandlerMap::const_iterator i = m_handler_map.find( cmd ) ;
	if ( i != m_handler_map.end() )
		(this->*(i->second))( args, count, res ) ;
}

void RealText::AddLine( const TextLine& line )
{
	PDF_ASSERT( !m_lines.empty() ) ;

	// remove empty lines first
	if ( m_lines.back().IsEmpty() )
		m_lines.pop_back() ;
	
	m_lines.push_back( line ) ;
}

void RealText::AddLine( double x, double y, const std::wstring& text )
{
	TextLine line( m_state, Matrix( 1,0,0,1, x, y ) ) ;
	line.AppendText( text ) ;
	return AddLine( line ) ;
}

void RealText::Visit( GraphicsVisitor *visitor )
{
	PDF_ASSERT( visitor != 0 ) ;
	
	visitor->VisitText( this ) ;
}

std::size_t RealText::Count( ) const
{
	return m_lines.size( ) ;
}

void RealText::Print( std::ostream& os, const Resources *res ) const
{
	os << "BT\n" ;
	
	// rendering state
	TextState	ts ;
	Matrix		trans ;

	using namespace boost ;
	std::for_each(
		m_lines.begin(),
		m_lines.end(),
		bind( &TextLine::Print, _1, ref(os), ref(trans), ref(ts), res ) ) ;

	os << "ET\n" ;
}

/// For debug purpose only. Prints XML.
std::ostream& operator<<( std::ostream& os, const RealText& t )
{
	os << "<RealText>\n" ;
	std::copy(
		t.begin(),
		t.end(),
		std::ostream_iterator<TextLine>(os, "\n" ) ) ;
	os << "</RealText>\n" ;
	return os ;
}

void RealText::OnTd( Object* args, std::size_t count, Resources* )
{
	if ( count >= 2 )
	{
		m_text_mat = m_line_mat =
			m_line_mat * Matrix( 1, 0, 0, 1, args[0], args[1] ) ;
		AddLine( TextLine( m_state, m_line_mat ) ) ;
	}
}

void RealText::OnTD( Object* args, std::size_t count, Resources *res )
{
	
	if ( count >= 2 )
	{
		double	ty	= args[1] ;
		m_state.SetLeading( -ty ) ;
		
		m_text_mat = m_line_mat =
			m_line_mat * Matrix( 1, 0, 0, 1, args[0], args[1] ) ;
		AddLine( TextLine( m_state, m_line_mat ) ) ;
	}
}

void RealText::OnTm( Object* args, std::size_t count, Resources* )
{
	if ( count >= 6 )
	{
		// unlike Td and TD, the Tm command will replace the current
		// matrix.
		m_text_mat = m_line_mat = Matrix(
			args[0], args[1], args[2], args[3], args[4], args[5] ) ;
		
		AddLine( TextLine( m_state, m_line_mat ) ) ;
	}
}

void RealText::OnTstar( Object* , std::size_t , Resources * )
{
	m_text_mat = m_line_mat = m_line_mat *
		Matrix( 1, 0, 0, 1, 0, -m_state.Leading() ) ;
	
	AddLine( TextLine( m_state, m_line_mat ) ) ;
}

///	Shows a Text string
void RealText::OnTj( Object* args, std::size_t count, Resources * )
{
	PDF_ASSERT( !m_lines.empty() ) ;
	
	if ( count >= 1 )
	{
		TextLine& current = m_lines.back() ;
		
		// must set the font properly before showing text
		if ( current.Format().GetFont() != 0 )
		{
			const std::string& s = args[0].As<std::string>() ;
			std::wstring ws( s.begin(), s.end() ) ;
			
			current.AppendText( ws ) ;
			
			m_text_mat.Dx( m_text_mat.Dx() + m_state.Width( ws ) ) ; 
		}
	}
}

///	\a array TJ
/**	Show one or more text strings, allowing individual glyph positioning. Each
	element of array can be a string or a number. If the element is a string,
	this operator shows the string. If it is a number, the operator adjusts the
	text position by that amount; that is, it translates the text matrix, Tm.
	The number is expressed in thousandths of a unit of text space. This amount
	is subtracted from the current horizontal or vertical coordinate, depending
	on the writing mode. In the default coordinate system, a positive adjustment
	has the effect of moving the next glyph painted either to the left or down
	by the given amount. 
*/
void RealText::OnTJ( Object* args, std::size_t count, Resources *res )
{
	PDF_ASSERT( !m_lines.empty() ) ;

	// current line
	TextLine& current = m_lines.back() ;

	double offset = 0.0 ;

	Array& a = args[0].As<Array>() ;
	for ( Array::iterator i = a.begin() ; i != a.end() ; ++i )
	{
		if ( i->Is<std::string>() )
		{
			std::string& s = i->As<std::string>() ;
			std::wstring ws( s.begin(), s.end() ) ;
			offset += m_state.Width( ws ) ;

			current.AppendText( ws ) ;
		}
		else if ( i->IsNumber() )
		{
			double disp = i->To<double>() / 1000.0 * m_state.FontSize() ;
			offset -= disp ;
			
			current.AppendSpace( disp ) ;
		}
	}
	
	// TODO: depend on writing mode, advance horizonal or vertical
	// assume vertical here.
	m_text_mat.Dx( m_text_mat.Dx() + offset ) ; 
}

void RealText::OnSingleQuote( Object* args, std::size_t count, Resources *res )
{
}

void RealText::OnDoubleQuote( Object* args, std::size_t count, Resources *res )
{
}

void RealText::OnTf( Object* args, std::size_t count, Resources *res )
{
	PDF_ASSERT( res != 0 ) ;

	if ( count >= 2 && args[0].Is<Name>() && args[1].IsNumber() )
	{
		BaseFont *f = res->FindFont( args[0].As<Name>() ) ;
		if ( f == 0 )
			std::cout << "unknown font: " << args[1] << std::endl ;
		else
		{
			double font_size = args[1].To<double>() ;
			
			TextLine& current = m_lines.back() ;
			
			if ( current.IsEmpty()					||
				 m_state.FontSize()	!= font_size	||
				 m_state.GetFont()	!= f )
			{
				m_state.SetFont( font_size, f ) ;

				if ( current.IsEmpty() )
					current.SetFormat( m_state ) ;
				else
					m_lines.push_back( TextLine( m_state, m_text_mat ) ) ;
			}
		}
	}
}

void RealText::OnTL( Object* args, std::size_t count, Resources *res )
{
	if ( count > 0 && args[0].IsNumber() )
		m_state.SetLeading( args[0].To<double>() ) ;
}

bool RealText::operator==( const RealText& rhs ) const
{
	// no need to compare current text state
	return m_lines		== rhs.m_lines ;
}

bool RealText::operator!=( const RealText& rhs ) const
{
	return !operator==( rhs ) ;
}

} // end of namespace
