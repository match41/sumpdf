/***************************************************************************
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
 ***************************************************************************/

/*!
	\file	Token.cc
	\brief	implementation the Token class
	\date	Sun Mar 9 2008
	\author	Nestal Wan
*/

#include "Token.hh"

#include <boost/bind.hpp>

#include <algorithm>
#include <cstdlib>
#include <istream>
#include <cctype>

namespace pdf {

/*!	default constructor. This is the default constructor. It initializes the
	object to an empty token.
*/
Token::Token( const std::string& t )
	: m_token( t )
{
}

/*!	extracts a token from input stream. This function does the actual parsing.
	It will extract the characters one-by-one using istream::peek() and
	istream::ignore(). Characters will not be putback()'d back to the input
	stream.
	\param	is		input stream to be extracted from
	\param	token	reference to output token
	\return	the input stream \a is
*/
std::istream& operator>>( std::istream& is, Token& token )
{
	std::string text ;

	// skip spaces
	while ( std::isspace( is.peek() ) && is )
		is.get() ;
	
	int ich ;
	while ( (ich = is.peek()) != std::char_traits<char>::eof() )
	{
		char ch = std::char_traits<char>::to_char_type( ich ) ;
		
		if ( !Token::IsCharInToken( ch, text ) )
			break ;
		
		text.push_back( ch ) ;
		is.ignore( ) ;
	}
	
	// only commit when parsing is successful
	if ( !text.empty() )
		text.swap( token.m_token ) ;

	// set stream state to failed if extracted token is empty
	// to prevend infinite loop
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

void Token::Swap( std::string& token )
{
	m_token.swap( token ) ;
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

template <>
std::string Token::As<std::string>( ) const
{
	return m_token ;
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
