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

/**	\file	GraphicsState.cc
	\brief	implementation of the GraphicsState class
	\date	Feb 16, 2010
	\author	Nestal Wan
*/

#include "graphics/GraphicsState.hh"

#include "core/Object.hh"
#include "core/Token.hh"

#include "font/BaseFont.hh"

#include "page/ContentOp.hh"
#include "page/Resources.hh"

#include "util/Debug.hh"
#include "util/Util.hh"

#include <iostream>

namespace pdf {

struct GraphicsState::HandlerMap
{
	/// command handler
	typedef bool (GraphicsState::*Handler)( ContentOp&, const Resources* ) ;
	typedef std::map<Token, Handler>	Map ;

	static const Map::value_type	m_val[] ;
	static const Map				m_map ;
} ;

const GraphicsState::HandlerMap::Map::value_type
	GraphicsState::HandlerMap::m_val[] =
{
	// text state commands
	std::make_pair( "Tf",	&GraphicsState::OnTf ),
	std::make_pair( "TL",	&GraphicsState::OnTL ),
} ;

const GraphicsState::HandlerMap::Map GraphicsState::HandlerMap::m_map(
    Begin( GraphicsState::HandlerMap::m_val ),
    End( GraphicsState::HandlerMap::m_val ) ) ;

/**	constructor
	
*/
GraphicsState::GraphicsState( const TextState& ts )
	: m_text( ts )
{
}

const TextState& GraphicsState::GetTextState() const
{
	return m_text ;
}

TextState& GraphicsState::GetTextState()
{
	return m_text ;
}

std::ostream& GraphicsState::Print(
	std::ostream&			os,
	const Resources			*res,
	const GraphicsState&	prev ) const
{
	m_text.Print( os, res, prev.m_text ) ;
	return os ;
}

/**	Handles PDF operators in the content stream.
	\return	\c true if the state is changed by the command. Otherwise \c false.
*/
bool GraphicsState::OnCommand( ContentOp& op, const Resources *res )
{
	HandlerMap::Map::const_iterator i = HandlerMap::m_map.find( op.Operator() );
	if ( i != HandlerMap::m_map.end() )
		return (this->*(i->second))( op, res ) ;

	return false ;
}

bool GraphicsState::IsGSCommand( const Token& cmd )
{
	HandlerMap::Map::const_iterator i = HandlerMap::m_map.find( cmd ) ;
	return i != HandlerMap::m_map.end() ;
}

bool GraphicsState::OnTf( ContentOp& op, const Resources *res )
{
	PDF_ASSERT( res != 0 ) ;

	if ( op.Count() >= 2 && op[0].Is<Name>() && op[1].IsNumber() )
	{
		BaseFont *f = res->FindFont( op[0].As<Name>() ) ;
		if ( f == 0 )
			std::cout << "unknown font: " << op[1] << std::endl ;
		else
		{
			double font_size = op[1].To<double>() ;
			
			if ( m_text.FontSize()	!= font_size	||
				 m_text.GetFont()	!= f )
			{
				m_text.SetFont( font_size, f ) ;
				return true ;
			}
		}
	}
	return false ;
}

bool GraphicsState::OnTL( ContentOp& op, const Resources *res )
{
	if ( op.Count() > 0 && op[0].IsNumber() )
	{
		m_text.SetLeading( op[0].To<double>() ) ;
		return true ;
	}
	else
		return false ;
}

Font* GraphicsState::GetFont( ) const
{
	return m_text.GetFont( ) ;
}

bool GraphicsState::operator==( const GraphicsState& gs ) const
{
	// TODO: add more members
	return m_text == gs.m_text ;
}

bool GraphicsState::operator!=( const GraphicsState& gs ) const
{
	return !operator==( gs ) ;
}

std::ostream& operator<<( std::ostream& os, const GraphicsState& gs )
{
	return os << gs.m_text ;
}

void GraphicsState::LineWidth( double value )
{
	m_line_width = value ;
}

double GraphicsState::LineWidth( ) const
{
	return m_line_width ;
}

void GraphicsState::LineCap( int value )
{
	m_line_cap = value ;
}

int GraphicsState::LineCap( ) const
{
	return m_line_cap ;
}

void GraphicsState::LineJoin( int value )
{
	m_line_join = value ;
}

int GraphicsState::LineJoin( ) const
{
	return m_line_join ;
}

void GraphicsState::MiterLimit( double value )
{
	m_miter_limit = value ;
}

double GraphicsState::MiterLimit( ) const
{
	return m_miter_limit ;
}

} // end of namespace
