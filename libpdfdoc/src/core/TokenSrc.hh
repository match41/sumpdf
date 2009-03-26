/***************************************************************************
 *   Copyright (C) 2006 by Nestal Wan                                      *
 *   me@nestal.net                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
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
 ***************************************************************************/

/*!
	\file	TokenSrc.hh
	\brief	definition the TokenSrc class
	\date	Wed Mar 4 2009
	\author	Nestal Wan
*/

#ifndef __PDF_TOKEN_SRC_HEADER_INCLUDED__
#define __PDF_TOKEN_SRC_HEADER_INCLUDED__

#include "Token.hh"
#include <vector>
#include <ios>

namespace pdf {

/*!	\brief	brief description
	
	this class represents
*/
class TokenSrc
{
public :
	TokenSrc( std::istream& file ) ;
	
	TokenSrc& ReadToken( Token& token ) ;
	void PutBack( const Token& token ) ;
	
	operator void*( ) const ;
	
	std::istream& Stream() ;
	
	void SetState( std::ios::iostate state ) ;
	void ResetState( ) ;
	
	bool HasCache( ) const ;
	
	TokenSrc& GetChar( char& ch ) ;
	
private :
	std::istream&		m_file ;
	std::vector<Token>	m_cache ;

	std::size_t			m_pos ;
} ;

TokenSrc& operator>>( TokenSrc& src, Token& token ) ;
// TokenSrc& operator>>( TokenSrc& src, int& val ) ;
// TokenSrc& operator>>( TokenSrc& src, double& val ) ;

} // end of namespace

#endif
