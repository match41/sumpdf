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

/*!
	\file	String.hh
	\brief	definition the String class
	\date	Sun Mar 9 2008
	\author	Nestal Wan
*/

#ifndef __PDF_STRING_HEADER_INCLUDED__
#define __PDF_STRING_HEADER_INCLUDED__

#include <string>
#include <iosfwd>

namespace pdf {

class Token ;
class TokenSrc ;

/*!	\brief	PDF string object
	\internal
	
	This class represents the PDF string object. A string object consists of a
	series of bytes—unsigned integer values in the range 0 to 255. String
	objects are not integer objects, but are stored in a more compact
	format.	
*/
class String
{
private :
	std::string	m_value ;

public :
	explicit String( const std::string& str = std::string( ) ) ;
	
	friend std::istream& operator>>( std::istream& is, String& b ) ;
	friend std::ostream& operator<<( std::ostream& os, const String& b ) ;
	friend TokenSrc& operator>>( TokenSrc& src, String& obj ) ;
	
	const std::string& Get( ) const ;

	operator std::string( ) const ;

	bool operator==( const String& str ) const ;

	bool IsHex( ) const ;

private :
	static TokenSrc& ReadXDigit( TokenSrc& is, char& digit ) ;

private :
	void DecodeLiteralString( TokenSrc& is ) ;
	void DecodeHexString( TokenSrc& is ) ;

	bool HandleEscapeCharacter( TokenSrc& is, char& ch ) ;
	
	enum DecodeState { done, extra, quit } ;
	DecodeState HandleOctal( TokenSrc& is, char& ch ) ;
} ;

} // end of namespace

#endif
