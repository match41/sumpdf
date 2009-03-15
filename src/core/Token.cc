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
	\file	Token.cc
	\brief	implementation the Token class
	\date	Sun Mar 9 2008
	\author	Nestal Wan
*/

#include "Token.hh"

#include <cstdlib>
#include <istream>
#include <cctype>

#include <iostream>

namespace pdf {

Token::Token( const std::string& t )
	: m_token( t )
{
}

std::istream& operator>>( std::istream& is, Token& token )
{
	std::string text ;

	// skip spaces
	while ( std::isspace( is.peek() ) && is )
		is.get() ;
	
	char ch ;
	while ( is.get( ch ) )
	{
		if ( !Token::IsCharInToken( ch, text ) )
		{
			is.putback( ch ) ;
			break ;
		}
		
		text.push_back( ch ) ;
	}
	
	// only commit when parsing is successful
	if ( !text.empty() )
	{
		is.clear( ) ;
		text.swap( token.m_token ) ;
	}
	else
		is.setstate( is.failbit ) ;
	
	return is ;
}

bool Token::IsDelimitor( char ch )
{
	static const std::string delimiters = "\t\r\n\f ()<>[]{}/%" ;
	return delimiters.find( ch ) != delimiters.npos ;
}

bool Token::IsCharInToken( char ch, const std::string& token )
{
	// current token. must put the char in
	if ( token.empty() )
		return true ;
	
	// treat << and >> as one token
	else if ( ch == '<' || ch == '>' )
		return ch == token[0] && token.size() <= 1 ;
	
	// for delimitor, start a new token
	else if ( IsDelimitor( ch ) )
		return false ;
	
	// only keep delimitors in one token
	else
		return !IsDelimitor( token[0] ) ;
}

const std::string& Token::Get( ) const
{
	return m_token ;
}

bool Token::operator==( const Token& t ) const
{
	return m_token == t.m_token ;
}

bool Token::operator<( const Token& t ) const
{
	return m_token < t.m_token ;
}

void Token::Swap( Token& token )
{
	m_token.swap( token.m_token ) ;
}

template <>
int Token::As<int>( ) const
{
	return std::atoi( m_token.c_str( ) ) ;
}

template <>
double Token::As<double>( ) const
{
	return std::atof( m_token.c_str( ) ) ;
}

bool Token::IsInt( ) const
{
	for ( std::string::const_iterator i = m_token.begin() ;
	                                  i!= m_token.end() ; ++i )
	{
		if ( !std::isdigit( *i ) )
			return false ;
	}
	return true ;
}

} // end of namespace
