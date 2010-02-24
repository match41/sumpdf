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

/**	\file	ContentOp.cc
	\brief	implementation of the ContentOp class
	\date	Feb 24, 2010
	\author	Nestal Wan
*/

#include "ContentOp.hh"

#include "core/TokenSrc.hh"

#include "util/Debug.hh"

#include <algorithm>
#include <iterator>
#include <ostream>

namespace pdf {

/**	constructor
	
*/
ContentOp::ContentOp( )
{
}

const Token& ContentOp::Operator() const
{
	return m_operator ;
}

ContentOp::iterator ContentOp::begin( )
{
	return m_operands.begin( ) ;
}

ContentOp::iterator ContentOp::end( )
{
	return m_operands.end( ) ;
}

ContentOp::const_iterator ContentOp::begin( ) const
{
	return m_operands.begin( ) ;
}

ContentOp::const_iterator ContentOp::end( ) const
{
	return m_operands.end( ) ;
}

TokenSrc& operator>>( TokenSrc& src, ContentOp& op )
{
	Object		obj ;
	ContentOp	tmp ;

	while ( true )
	{
		if ( src >> obj )
		{
			// swapping is faster
			tmp.m_operands.push_back( Object() ) ;
			obj.Swap( tmp.m_operands.back() ) ;
		}
		
		// if it is not an object, then it should be a command operator
		else
		{
			src.ResetState( ) ;
			
			// all successful, commit
			if ( src >> tmp.m_operator )
				op.Swap( tmp ) ;
			
			// leave the state of the TokenSrc to indicate success or error
			break ;
		}
	}
	return src ;
}

std::ostream& operator<<( std::ostream& os, const ContentOp& op )
{
	os << op.Operator().Get() << " " ;
	std::copy( op.begin(), op.end(), std::ostream_iterator<Object>( os, " " ) );
	return os << std::endl ;
}

void ContentOp::Swap( ContentOp& op )
{
	m_operator.Swap( op.m_operator ) ;
	m_operands.swap( op.m_operands ) ;
}

std::size_t ContentOp::Count( ) const
{
	return m_operands.size( ) ;
}

Object& ContentOp::operator[]( std::size_t idx )
{
	PDF_ASSERT( idx < m_operands.size() ) ;
	return m_operands[idx] ;
}

const Object& ContentOp::operator[]( std::size_t idx ) const
{
	PDF_ASSERT( idx < m_operands.size() ) ;
	return m_operands[idx] ;
}

bool ContentOp::operator==( const ContentOp& rhs ) const
{
	return
		m_operator == rhs.m_operator &&
		m_operands == rhs.m_operands ;
}

bool ContentOp::operator!=( const ContentOp& rhs ) const
{
	return !operator==( rhs ) ;
}

} // end of namespace
