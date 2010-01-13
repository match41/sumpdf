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

/**	\file	RealRealText.cc
	\brief	implementation of the RealRealText class
	\date	Jan 13, 2010
	\author	Nestal Wan
*/

#include "RealText.hh"

#include "graphics/GraphicsVisitor.hh"

#include "core/Token.hh"

#include "util/Util.hh"

#include <algorithm>
#include <iterator>
#include <iostream>
#include <cassert>
#include <set>

namespace pdf {

namespace
{
	typedef	std::set<Token>	TokenSet ;

	const TokenSet::value_type	pos_cmd[] =
	{
		// RealText position
		Token("Td"), Token("TD"), Token("Tm"), Token("T*"),
	} ;
	const TokenSet pos_cmds( Begin( pos_cmd ), End( pos_cmd ) ) ;
}

/**	constructor
*/
RealText::RealText( )
	: m_lines( 1 )
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

void RealText::OnCommand(
	const Token& 	cmd,
	Object 			*args,
	std::size_t		count,
	Resources		*res )
{
	assert( !m_lines.empty() ) ;

	// RealText position command. create new line
	if ( pos_cmds.find( cmd ) != pos_cmds.end() && !m_lines.back().IsEmpty() )
	{
std::cout << "create new line" << std::endl ;
		m_lines.push_back( TextLine() ) ;
	}
	
	m_lines.back().OnCommand( cmd, args, count, res ) ;
}

void RealText::AddLine( const TextLine& line )
{
	m_lines.push_back( line ) ;
}

void RealText::Visit( GraphicsVisitor *visitor )
{
	assert( visitor != 0 ) ;
	
	visitor->VisitText( this ) ;
}

std::size_t RealText::Count( ) const
{
	return m_lines.size( ) ;
}

void RealText::Output( std::ostream& os ) const
{
	os << "BT\n" ;
	std::copy(
		m_lines.begin(),
		m_lines.end(),
		std::ostream_iterator<TextLine>( os ) ) ;
		
	os << "ET\n" ;
}

} // end of namespace
