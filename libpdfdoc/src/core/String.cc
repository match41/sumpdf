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

/*!	\file	String.cc
	\brief	implementation the String class
	\date	Sun Mar 9 2008
	\author	Nestal Wan
*/

#include "String.hh"

#include "Token.hh"
#include "TokenSrc.hh"

#include "util/Debug.hh"

#include <boost/bind.hpp>

#include <algorithm>
#include <functional>
#include <cctype>
#include <cstdlib>
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <iterator>
#include <sstream>

namespace pdf {

/// Construct a string object with an std::string
String::String( const std::string& str )
	: m_value( str )
{
}

/// Returns the underlying string as std::string
const std::string& String::Get( ) const
{
	return m_value ;
}

/// Automatic conversion to std::string
String::operator std::string( ) const
{
	return m_value ;
}

/// Read from std::istream.
/**	This function will create a TokenSrc for reading. It is recommended to use
	the extraction operator of TokenSrc instead.
*/
std::istream& operator>>( std::istream& is, String& b )
{
	TokenSrc src( is ) ;
	return (src >> b).Stream() ;
}

TokenSrc& operator>>( TokenSrc& src, String& obj )
{
	Token t ;
	
	if ( src >> t )
	{
		std::istringstream is( t.Get() ) ;
	
		char ch ;
		if ( is.get(ch) )
		{
			if ( ch == '(' )
				obj.DecodeLiteralString( is ) ;
			
			else if ( ch == '<' )
				obj.DecodeHexString( src ) ;
			
			else
				is.setstate( std::ios::failbit ) ;
		}
		
		if ( !is )
			src.SetState( std::ios::failbit ) ;
	}
	return src ;
}

void String::DecodeLiteralString( std::istream& is )
{
	// assume the first char is '('
	int bracket_balance = 1 ;

	DecodeState state = done ;

	char ch ;
	while ( state == extra || is.get( ch ) )
	{
		state = done ;
	
		// escape character
		switch ( ch )
		{
			case '\\' :
				if ( !HandleEscapeCharacter( is, ch ) )
				{
					state = HandleOctal( is, ch ) ;
					if ( state == quit )
						break ;
					
					// fall through. PDF specs indicates for unknown character 
					// after escape character '\' will ignore the escape
					// character, i.e. the character after '\' will still be
					// counted.
					continue ;
				}
				break ;
			
			case '(' :
				bracket_balance++ ;
				break ;
			
			case ')' :
				bracket_balance-- ;
				if ( bracket_balance <= 0 )
					return ;
		}
		
		m_value.push_back( ch ) ;
	} 
}

//void String::HandleOctal( TokenSrc& is, char& ch, bool& quit, bool& used )
String::DecodeState String::HandleOctal( std::istream& is, char& ch )
{
	DecodeState ret = extra ;
	
	// handle the \ddd octal digits here
	if ( ch >= '0' && ch < '8' )
	{
		char val = ch - '0' ;
		
		// at most 2 more octal digits
		for ( int i = 0 ; i < 2 ; i++ )
		{
			// no character in input, quit
			if ( !is.get( ch ) )
				return quit ;
			
			else if ( ch >= '0' && ch < '8' )
			{
				val <<= 3 ;
				val |= (ch - '0') ;
				
				ret = done ;
			}
			
			else
			{
				ret = extra ;
				break ;
			}
		}
		
		m_value.push_back( val ) ;
	}
	return ret ;
}

bool String::HandleEscapeCharacter( std::istream& is, char& ch )
{
	if ( is.get( ch ) )
	{
		switch ( ch )
		{
			case 'n'  : ch = '\n' ;     break ;
			case 'r'  : ch = '\r' ;     break ;
			case 't'  : ch = '\t' ;     break ;
			case 'b'  : ch = '\b' ;     break ;
			case 'f'  : ch = '\f' ;     break ;
			case '('  : ch = '(' ;      break ;
			case ')'  : ch = ')' ;      break ;
			case '\\' : ch = '\\' ;     break ;
			
			// unknown character after escape is not an error.
			// it will be ignored.
			default :
				return false ;
		}
	}
	return true ;
}

TokenSrc& String::ReadXDigit( TokenSrc& is, char& digit )
{
	// skip space characters
	while ( is.GetChar( digit ) && std::isspace( digit ) )
	{
	}
	return is ;
}

void String::DecodeHexString( TokenSrc& is )
{
	// the 3rd byte is never written to keep it null-terminated
	char ch[3] = {} ;
	
	while ( ReadXDigit( is, ch[0] ) )
	{
		if ( ch[0] == '>' )
			break ;
		
		if ( std::isxdigit( ch[0] ) )
		{
			if ( !ReadXDigit( is, ch[1] ) )
			{
				is.SetState( std::ios::failbit ) ;
				break ;
			}
			else
			{
				bool quit = false ;
				if ( ch[1] == '>' )
				{
					ch[1] = '0' ; // treat as 0 for missing second character
					quit = true ;
				}
				else if ( !std::isxdigit( ch[1] ) )
				{
					is.SetState( std::ios::failbit ) ;
					break ;
				}
				
				m_value.push_back( static_cast<char>(std::strtol( ch, 0, 16 )));
				
				if ( quit )
					break ;
			}
		}
		else
			is.SetState( std::ios::failbit ) ;
	}
}

namespace
{
	unsigned short ToInt( char ch )
	{
		return static_cast<unsigned char>( ch ) ;
	}

	bool IsPrint( char ch )
	{
		return
			std::isprint( static_cast<unsigned char>( ch ) ) != 0 ||
			ch == '\n' ;
	}
}

std::ostream& operator<<( std::ostream& os, const String& b )
{
	if ( b.IsHex() )
	{
		os << '<' ;
		for ( std::string::const_iterator i = b.m_value.begin() ;
			i != b.m_value.end() ; ++i )
		{
			os	<< std::hex << std::setw( 2 ) << std::setfill('0')
				<< static_cast<unsigned short>(static_cast<unsigned char>(*i)) ;
		}
		os << '>' << std::dec ;
	}
	else
	{
		os.put( '(' ) ;
		for ( std::string::const_iterator i = b.m_value.begin() ;
			i != b.m_value.end() ; ++i )
		{
			if ( *i == '(' || *i == ')' )
				os.put( '\\' ) ;
			os.put( *i ) ;
		}
		os.put( ')' ) ;
	}
	return os ;
}

bool String::operator==( const String& str ) const
{
	return m_value == str.m_value ;
}

bool String::IsHex( ) const
{
	return std::find_if(
		m_value.begin( ),
		m_value.end( ),
		!boost::bind( IsPrint, _1 ) ) != m_value.end( ) ;
}

} // end of namespace
