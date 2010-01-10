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
	\file	Text.cc
	\brief	implementation of the Test class
	\date	Jan 4, 2010
	\author	Nestal Wan
*/

#include "graphics/Text.hh"
#include "graphics/GraphicsVisitor.hh"

#include "core/Token.hh"

#include "util/Util.hh"

#include <iostream>
#include <cassert>
#include <set>

namespace pdf {

namespace
{
	typedef	std::set<Token>	TokenSet ;

	const TokenSet::value_type	pos_cmd[] =
	{
		// text position
		Token("Td"), Token("TD"), Token("Tm"), Token("T*"),
	} ;
	const TokenSet pos_cmds( Begin( pos_cmd ), End( pos_cmd ) ) ;
}

/**	constructor
*/
Text::Text( )
	: m_lines( 1 )
{
}

Text::iterator Text::begin()
{
	return m_lines.begin( ) ;
}

Text::iterator Text::end()
{
	return m_lines.end( ) ;
}

Text::const_iterator Text::begin() const
{
	return m_lines.begin( ) ;
}

Text::const_iterator Text::end() const
{
	return m_lines.end( ) ;
}

void Text::OnCommand(
	const Token& 	cmd,
	Object 			*args,
	std::size_t		count,
	Resources		*res )
{
	assert( !m_lines.empty() ) ;

	// text position command. create new line
	if ( pos_cmds.find( cmd ) != pos_cmds.end() && !m_lines.back().IsEmpty() )
	{
std::cout << "create new line" << std::endl ;
		m_lines.push_back( TextLine() ) ;
	}
	
	m_lines.back().OnCommand( cmd, args, count, res ) ;
}

void Text::AddLine( const TextLine& line )
{
	m_lines.push_back( line ) ;
}

void Text::Visit( GraphicsVisitor *visitor )
{
	assert( visitor != 0 ) ;
	
	visitor->VisitText( this ) ;
}

std::size_t Text::Count( ) const
{
	return m_lines.size( ) ;
}

} // end of namespace
