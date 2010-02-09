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

	// skip all spaces
	while ( std::isspace( is.peek() ) && is )
		is.get() ;

	int ich = is.peek() ;
	if ( ich != std::char_traits<char>::eof() )
	{
		switch ( ich )
		{
			case '(': Token::DecodeBracketString( is, text ) ; break ;
//			case '<': Token::DecodeHexString( is, text ) ; break ;
			default : Token::DecodeToken( is, text ) ; break ;
		}
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

void Token::DecodeBracketString( std::istream& is, std::string& text )
{
	// assume the first char is (
	is.ignore() ;
	int bracket = 1 ;
	text.push_back( '(' ) ;
	
	char ch ;
	while ( bracket > 0 && is.get( ch ) )
	{
		if ( ch == '(' )
			bracket++ ;
		else if ( ch == ')' )
			bracket-- ;
	
		text.push_back( ch ) ;
	}
}

void Token::DecodeHexString( std::istream& is, std::string& text )
{
	char ch ;
	while ( is.get( ch ) )
	{
		text.push_back( ch ) ;
		if ( ch == '>' )
			break ;
	}
}

void Token::DecodeToken( std::istream& is, std::string& text )
{
	int ich ;
	while ( (ich = is.peek()) != std::char_traits<char>::eof() )
	{
		char ch = std::char_traits<char>::to_char_type( ich ) ;
		
		if ( !Token::IsCharInToken( ch, text ) )
			break ;
		
		text.push_back( ch ) ;
		is.ignore( ) ;
	}
}

/**	\brief	Check if a character is a PDF delimiter.
	\return	whether the character is a delimiter.
*/
bool Token::IsDelimiter( char ch )
{
	static const std::string delimiters = "\t\r\n\f ()<>[]{}/%" ;
	return delimiters.find( ch ) != delimiters.npos ;
}

/**	\brief	Check if a new coming character belongs to the same token.

	It checks if the character should be added to the current token, or start
	a new token.
	\param	ch		The new character.
	\param	token	The current token.
	\return	whether the character belong to the token.
*/
bool Token::IsCharInToken( char ch, const std::string& token )
{
	// new token. must put the char in
	if ( token.empty() )
		return true ;
	
	// treat << and >> as one token
	else if ( ch == '<' || ch == '>' )
		return ch == token[0] && token.size() <= 1 ;
	
	// for delimitor, start a new token
	else if ( IsDelimiter( ch ) )
		return false ;
	
	// only keep delimitors in one token
	else
		return !IsDelimiter( token[0] ) ;
}

/**	\brief	Get the string of the token.
	\return	the string.
*/
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

bool Token::IsSpace( ) const
{
	return !m_token.empty() && std::isspace( *m_token.begin() ) ; 
}

} // end of namespace
