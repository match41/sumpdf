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

#include "page/Resources.hh"

#include "util/Debug.hh"
#include "util/Util.hh"

#include <iostream>

namespace pdf {

struct GraphicsState::HandlerMap
{
	/// command handler
	typedef bool (GraphicsState::*Handler)(
		Object			*args,
		std::size_t		count,
		Resources		*res ) ;
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
bool GraphicsState::OnCommand(
	const Token& 	cmd,
	Object 			*args,
	std::size_t		count,
	Resources		*res )
{
	HandlerMap::Map::const_iterator i = HandlerMap::m_map.find( cmd ) ;
	if ( i != HandlerMap::m_map.end() )
		return (this->*(i->second))( args, count, res ) ;

	return false ;
}

bool GraphicsState::IsGSCommand( const Token& cmd )
{
	HandlerMap::Map::const_iterator i = HandlerMap::m_map.find( cmd ) ;
	return i != HandlerMap::m_map.end() ;
}

bool GraphicsState::OnTf( Object* args, std::size_t count, Resources *res )
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

bool GraphicsState::OnTL( Object* args, std::size_t count, Resources *res )
{
	if ( count > 0 && args[0].IsNumber() )
	{
		m_text.SetLeading( args[0].To<double>() ) ;
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

} // end of namespace
