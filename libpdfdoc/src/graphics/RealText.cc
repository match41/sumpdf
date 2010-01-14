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

#include "core/Array.hh"
#include "core/Object.hh"
#include "core/Token.hh"
#include "font/BaseFont.hh"
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
std::cout << "Td: " << args[0] << " " << args[1] << std::endl ;

	if ( count >= 2 )
	{
		m_line_mat = m_line_mat * Matrix( 1, 0, 0, 1, args[0], args[1] ) ;
		AddNewLine( ) ;
	}
}

void RealText::OnTD( Object* args, std::size_t count, Resources *res )
{
std::cout << "TD: " << std::endl ;
	
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
std::cout << "Tm: " << std::endl ;

	if ( count >= 6 )
	{
		// unlike Td and TD, the Tm command will replace the current
		// matrix.
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
	AddNewLine( m_line_mat ) ;
}

void RealText::AddNewLine( const Matrix& mat )
{
	assert( !m_lines.empty() ) ;

	// remove empty lines first
	if ( m_lines.back().IsEmpty() )
	{
std::cout << "pop" << std::endl ;
		m_lines.pop_back() ;
	}
std::cout << "new line: " << std::endl ;
	m_lines.push_back( TextLine( mat, m_state ) ) ;
}

void RealText::OnTj( Object* args, std::size_t count, Resources *res )
{
	assert( !m_lines.empty() ) ;
	
	if ( count >= 1 )
	{
		const std::string& s = args[0].As<std::string>() ;
		m_lines.back().AppendText( std::wstring( s.begin(), s.end() ) ) ;
	}
}

void RealText::OnTJ( Object* args, std::size_t count, Resources *res )
{
	assert( !m_lines.empty() ) ;

	Matrix tm = m_line_mat ;

	Array& a = args[0].As<Array>() ;
	for ( Array::iterator i = a.begin() ; i != a.end() ; ++i )
	{
		if ( i->Is<std::string>() )
		{
			std::string& s = i->As<std::string>() ;
			m_lines.back().AppendText( std::wstring( s.begin(), s.end() ) ) ;
		}
		else if ( i->Is<double>() || i->Is<int>() )
		{
			double disp = i->To<double>() ;
			
			// TODO: depend on writing mode, advance horizonal or vertical
			// assume vertical here.
			Matrix m ;
			m.Dx( -disp ) ;
			tm = tm * m ;
//			AddNewLine( tm ) ;
		}
	}
}

void RealText::OnSingleQuote( Object* args, std::size_t count, Resources *res )
{
}

void RealText::OnDoubleQuote( Object* args, std::size_t count, Resources *res )
{
}

void RealText::OnTf( Object* args, std::size_t count, Resources *res )
{
	assert( res != 0 ) ;

	if ( count >= 2 )
	{
		BaseFont *f = res->FindFont( args[0].As<Name>() ) ;
		if ( f == 0 )
			std::cout << "unknown font: " << args[1] << std::endl ;
		else
		{
			m_state.SetFont( args[1], f ) ;
			m_lines.back().ChangeState( m_state ) ;
		}
	}
}

} // end of namespace
