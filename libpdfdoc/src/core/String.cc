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

#include <boost/bind.hpp>

#include <algorithm>
#include <functional>
#include <cctype>
#include <cstdlib>
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <iterator>
#include <cassert>

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
		if ( t.Get() == "(" )
			obj.DecodeLiteralString( src ) ;
		
		else if ( t.Get() == "<" )
			obj.DecodeHexString( src ) ;
	
		else
			src.SetState( std::ios::failbit ) ;
	}
	return src ;
}

void String::DecodeLiteralString( TokenSrc& is )
{
	int bracket_balance = 0 ;

	char ch ;
	while ( is.GetChar( ch ) )
	{
		// escape character
		switch ( ch )
		{
			case '\\' :
				if ( !HandleEscapeCharacter( is, ch ) )
				{
					bool ch_used = false ;
				
					// handle the \ddd octal digits here
					if ( ch >= '0' && ch < '8' )
					{
						char val = ch - '0' ;
std::cout << "val = " << (int)val << std::endl ;
						
						bool quit = false ;
						
						// at most 2 more octal digits
						for ( int i = 0 ; i < 2 && !quit ; i++ )
						{
							// no character in input, quit
							if ( !is.GetChar( ch ) )
								quit = true ;
							
							else if ( ch >= '0' && ch < '8' )
							{
								val <<= 3 ;
								val |= (ch - '0') ;
std::cout << "val" << i << " = " << (int)val << std::endl ;
								ch_used = true ;
							}
							
							else
							{
								ch_used = false ;
								break ;
							}
						}
						
						m_value.push_back( val ) ;
						
						if ( quit )
							break ;
					}
					
					// fall through. PDF specs indicates for unknown character 
					// after escape character '\' will ignore the escape
					// character, i.e. the character after '\' will still be
					// counted.
					if ( !ch_used )
					{
						if ( ch == '(' )
							bracket_balance++ ;
						else if ( ch == ')' )
						{
							bracket_balance-- ;
							if ( bracket_balance < 0 )
								return ;
						}
						else
							m_value.push_back( ch ) ;
					}
					continue ;
				}
				break ;
			
			case '(' :
				bracket_balance++ ;
				break ;
			
			case ')' :
				bracket_balance-- ;
				if ( bracket_balance < 0 )
					return ;
		}
		
		m_value.push_back( ch ) ;
	} 
}

bool String::HandleCharacter( TokenSrc& is, char& ch )
{
	return false ;
}

bool String::HandleEscapeCharacter( TokenSrc& is, char& ch )
{
std::cout << "found esp char = \'" << ch << "\'" << std::endl ;

	if ( is.GetChar( ch ) )
	{
std::cout << "esp char = \'" << ch << "\'" << std::endl ;
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
std::cout << "invalid esp char! \'" << ch << "\'" << std::endl ;
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
	char ch ;
	while ( ReadXDigit( is, ch ) )
	{
		if ( ch == '>' )
			break ;
			
		char ch2 ;
		if ( std::isxdigit( ch ) && ReadXDigit( is, ch2 ) &&
		     std::isxdigit( ch2 ) )
		{
			char result[3] = { ch, ch2, '\0' } ;
			m_value.push_back( static_cast<char>(std::strtol( result, 0, 16 )));
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
		os << '<' << std::hex << std::setw( 2 ) ;
		std::transform(
			b.m_value.begin( ),
			b.m_value.end( ),
		    std::ostream_iterator<unsigned short>( os ), ToInt ) ;
		os << '>' ;
	}
	else
		os << "(" << b.m_value << ")" ;

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
