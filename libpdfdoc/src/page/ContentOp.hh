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

/**	\file	ContentOp.hh
    \brief	definition the ContentOp class
    \date	Feb 24, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_CONTENTOP_HH_EADER_INCLUDED__
#define __PDF_CONTENTOP_HH_EADER_INCLUDED__

#include "core/Object.hh"
#include "core/Token.hh"

#include <vector>

namespace pdf {

class TokenSrc ;

///	brief description
/**	\internal
	The ContentOp class represents
*/
class ContentOp
{
private :
	typedef std::vector<Object> Operands ;
	
public :
	typedef Operands::const_iterator	const_iterator ;
	typedef Operands::iterator			iterator ;

public :
	ContentOp( ) ;
	
	template <typename InputIt>
	ContentOp( const Token& t, InputIt first, InputIt last )
	: m_operator( t )
	, m_operands( first, last )
	{
	}

	const Token& Operator() const ;
	
	const_iterator begin( ) const ;
	const_iterator end( ) const ;
	iterator begin( ) ;
	iterator end( ) ;
	
	std::size_t Count( ) const ;
	
	Object& operator[]( std::size_t idx ) ;
	const Object& operator[]( std::size_t idx ) const ;

	friend TokenSrc& operator>>( TokenSrc& src, ContentOp& op ) ;

	void Swap( ContentOp& op ) ;

private :
	Token		m_operator ;
	Operands	m_operands ;
} ;

} // end of namespace

#endif // CONTENTOP_HH_
