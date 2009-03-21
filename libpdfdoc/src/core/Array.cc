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
	\file	Array.cc
	\brief	implementation the Array class
	\date	Thu Mar 6 2008
	\author	Nestal Wan
*/

#include "Array.hh"

#include "Dictionary.hh"
#include "Token.hh"
#include "TokenSrc.hh"

#include "util/Exception.hh"

#include <iostream>

namespace pdf {

Array::Array( )
{
}

void Array::swap( Array& array )
{
	m_array.swap( array.m_array ) ;
}

void Array::push_back( const Object& obj )
{
	m_array.push_back( obj ) ;
}


std::istream& operator>>( std::istream& is, Array& array )
{
/*	Token first ;
	return is >> first ? array.Decode( is, first ) : is ;*/
	TokenSrc s( is ) ;
	return (s >> array).Stream() ;
}

TokenSrc& operator>>( TokenSrc& src, Array& array )
{
	Token t ;
	if ( !(src >> t) || t.Get() != "[" )
		throw ParseError( "bad array" ) ;

	Array temp ;
	while ( src >> t && t.Get() != "]" )
	{
		src.PutBack( t ) ;
		
		Object obj ;
		if ( src >> obj )
			temp.m_array.push_back( obj ) ;
	}
	
	// commit
	if ( src )
		temp.swap( array ) ;
	
	return src ;
}

std::ostream& operator<<( std::ostream& os, const Array& array )
{
	os << '[' ;
	for ( Array::const_iterator i = array.begin( ) ; i != array.end( ) ; ++i )
	{
		os << *i ;
		if ( i + 1 != array.end( ) )
			os << ' ' ;
	}
	os << ']' ;
	return os ;
}

Array::iterator Array::begin( )
{
	return m_array.begin( ) ;
}

Array::iterator Array::end( )
{
	return m_array.end( ) ;
}

Array::const_iterator Array::begin( ) const
{
	return m_array.begin( ) ;
}

Array::const_iterator Array::end( ) const
{
	return m_array.end( ) ;
}

std::size_t Array::size( ) const
{
	return m_array.size( ) ;
}

bool Array::empty( ) const
{
	return m_array.empty( ) ;
}

void Array::clear( )
{
	m_array.clear( ) ;
}

bool Array::operator==( const Array& array ) const
{
	return m_array == array.m_array ;
}

Object& Array::operator[]( std::size_t index )
{
	return m_array.at(index) ;
}

const Object& Array::operator[]( std::size_t index ) const
{
	return m_array.at(index) ;
}

Object& Array::at( std::size_t i )
{
	return m_array.at( i ) ;
}

const Object& Array::at( std::size_t i ) const
{
	return m_array.at( i ) ;
}

} // end of namespace

namespace std
{
	void swap( pdf::Array& dict1, pdf::Array& dict2 )
	{
		dict1.swap( dict2 ) ;
	}
}
