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
	\file	Token.hh
	\brief	definition the Token class
	\date	Sun Mar 9 2008
	\author	Nestal Wan
*/

#ifndef __PDF_TOKEN_HEADER_INCLUDED__
#define __PDF_TOKEN_HEADER_INCLUDED__

#include <iosfwd>
#include <string>

namespace pdf {

/*!	\brief	tokens in a PDF file
	\internal
	
	A token is a basic unit of the PDF file. The PDF parser will divide the
	bytes in a PDF file into tokens. Each token is a non-divisable unit of
	data. The meaning of tokens will be determined base on context.
*/
class Token
{
public :
	explicit Token( const std::string& t = std::string() ) ;

	friend std::istream& operator>>( std::istream& is, Token& token ) ;

	const std::string& Get( ) const ;

	bool operator==( const Token& t ) const ;

	bool operator<( const Token& t ) const ;

	void Swap( Token& token ) ;

	template <typename T>
	T As( ) const ;

	bool IsInt( ) const ;

private :
	static bool IsCharInToken( char ch, const std::string& text ) ;
	static bool IsDelimitor( char ch ) ;

private :
	std::string	m_token ;
} ;

} // end of namespace

#endif
