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

#include "Text.hh"

#include "core/Token.hh"

#include "util/Util.hh"

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

void Text::OnCommand(
	const Token& 	cmd,
	const Object 	*args,
	std::size_t		count,
	Resources		*res )
{
	// text position command. create new line
	if ( pos_cmds.find( cmd ) != pos_cmds.end() )
		m_lines.push_back( TextLine() ) ;
	
	m_lines.back().OnCommand( cmd, args, count, res ) ;
}

} // end of namespace
