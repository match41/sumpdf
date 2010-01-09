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

#include "core/Object.hh"
#include "core/Token.hh"

#include "util/Util.hh"

#include <cassert>
#include <set>

namespace pdf {

namespace
{
	typedef	std::set<Token>	TokenSet ;
	const TokenSet::value_type	state_cmd[] =
	{
		// text state
		Token("Tc"), Token("Tw"), Token("Tz"), Token("TL"), Token("Tf"),
		Token("Tr"), Token("Ts"),
	} ;
	const TokenSet state_cmds( Begin( state_cmd ), End( state_cmd ) ) ;
}

const TextLine::HandlerMap::value_type	TextLine::m_handler_map_values[] =
{
	std::make_pair( "Td",	&TextLine::OnTd ),
	std::make_pair( "TD",	&TextLine::OnTD ),
	std::make_pair( "Tm",	&TextLine::OnTm ),
	std::make_pair( "T*",	&TextLine::OnTstar ),
} ;

const TextLine::HandlerMap TextLine::m_handler_map(
    Begin( TextLine::m_handler_map_values ),
    End( TextLine::m_handler_map_values ) ) ;

/**	constructor
*/
TextLine::TextLine( )
    : m_blks( 1 )
{
}

TextLine::iterator TextLine::begin()
{
	return m_blks.begin( ) ;
}

TextLine::iterator TextLine::end()
{
	return m_blks.end( ) ;
}

TextLine::const_iterator TextLine::begin() const
{
	return m_blks.begin( ) ;
}

TextLine::const_iterator TextLine::end() const
{
	return m_blks.end( ) ;
}

void TextLine::OnCommand(
	const Token& 	cmd,
	const Object 	*args,
	std::size_t		count,
	Resources		*res )
{
	assert( !m_blks.empty() ) ;
	
	if ( state_cmds.find( cmd ) != state_cmds.end() )
	{
		m_blks.push_back( TextBlock() ) ;
		m_blks.back().OnCommand( cmd, args, count, res ) ;
	}
	else
	{
	    HandlerMap::const_iterator i = m_handler_map.find( cmd ) ;
	    if ( i != m_handler_map.end() )
	        (this->*(i->second))( args, count, res ) ;
    }
}

void TextLine::OnTd( const Object* args, std::size_t count, Resources* )
{
	if ( count >= 2 )
		m_trans = Matrix( 1, 0, 0, 1, args[0], args[1] ) ;
}

void TextLine::OnTD( const Object* args, std::size_t count, Resources *res )
{
	if ( count >= 2 )
	{
		double	ty	= args[1] ;

		m_blks.push_back( TextBlock() ) ;
		m_blks.back().Format().SetLeading( -ty ) ;
		
		m_trans = Matrix( 1, 0, 0, 1, args[0], args[1] ) ;
	}
}

void TextLine::OnTm( const Object* args, std::size_t count, Resources* )
{
	if ( count >= 6 )
		m_trans = Matrix(
			args[0], args[1], args[2], args[3], args[4], args[5] ) ;
}

void TextLine::OnTstar( const Object* , std::size_t , Resources *res )
{
	m_trans = Matrix( 1, 0, 0, 1, 0, m_blks.back().Format().Leading() ) ;
}

} // end of namespace
