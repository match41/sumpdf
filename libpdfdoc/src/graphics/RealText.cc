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
#include "core/Object.hh"
#include "page/Resources.hh"
#include "util/Util.hh"

#include <algorithm>
#include <iterator>
#include <iostream>
#include <cassert>
#include <set>

namespace pdf {

const RealText::HandlerMap::value_type	RealText::m_handler_map_values[] =
{
	std::make_pair( "Td",	&RealText::OnTd ),
	std::make_pair( "TD",	&RealText::OnTD ),
	std::make_pair( "Tm",	&RealText::OnTm ),
	std::make_pair( "T*",	&RealText::OnTstar ),
} ;

const RealText::HandlerMap RealText::m_handler_map(
    Begin( RealText::m_handler_map_values ),
    End( RealText::m_handler_map_values ) ) ;

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

    HandlerMap::const_iterator i = m_handler_map.find( cmd ) ;
    if ( i != m_handler_map.end() )
        (this->*(i->second))( args, count, res ) ;
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

void RealText::OnTd( Object* args, std::size_t count, Resources* )
{
	if ( count >= 2 )
	{
		m_line_mat = m_line_mat * Matrix( 1, 0, 0, 1, args[0], args[1] ) ;
		AddNewLine( ) ;
	}
}

void RealText::OnTD( Object* args, std::size_t count, Resources *res )
{
	if ( count >= 2 )
	{
		double	ty	= args[1] ;
		m_state.SetLeading( -ty ) ;
		
		m_line_mat = m_line_mat * Matrix( 1, 0, 0, 1, args[0], args[1] ) ;
		AddNewLine( ) ;
	}
}

void RealText::OnTm( Object* args, std::size_t count, Resources* )
{
	if ( count >= 6 )
	{
		m_line_mat = Matrix(
			args[0], args[1], args[2], args[3], args[4], args[5] ) ;
		
		AddNewLine( ) ;
	}
}

void RealText::OnTstar( Object* , std::size_t , Resources * )
{
	m_line_mat = m_line_mat *
		Matrix( 1, 0, 0, 1, 0, m_state.Leading() ) ;
	AddNewLine( ) ;
}

void RealText::AddNewLine( )
{
	assert( !m_lines.empty() ) ;

	// remove empty lines first
	if ( m_lines.back().IsEmpty() )
		m_lines.pop_back() ;
	
	m_lines.push_back( TextLine( m_line_mat, m_state ) ) ;
}

} // end of namespace
