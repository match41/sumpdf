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
	\file	Dictionary.cc
	\brief	implementation the Dictionary class
	\date	Thu Mar 6 2008
	\author	Nestal Wan
*/

#include "Dictionary.hh"

#include "Array.hh"
#include "Token.hh"
#include "TokenSrc.hh"
#include "util/Exception.hh"

#include <istream>

#include <iostream>

namespace pdf {

Dictionary::Dictionary( )
{
}

void Dictionary::swap( Dictionary& dict )
{
	m_map.swap( dict.m_map ) ;
}

Dictionary::iterator Dictionary::begin( )
{
	return m_map.begin( ) ;
}

Dictionary::iterator Dictionary::end( )
{
	return m_map.end( ) ;
}

Dictionary::const_iterator Dictionary::begin( ) const
{
	return m_map.begin( ) ;
}

Dictionary::const_iterator Dictionary::end( ) const
{
	return m_map.end( ) ;
}

Dictionary::iterator Dictionary::find( const Name& name )
{
	return m_map.find( name ) ;
}

Dictionary::const_iterator Dictionary::find( const Name& name ) const
{
	return m_map.find( name ) ;
}

std::pair<Dictionary::iterator, bool> Dictionary::insert( const value_type& v )
{
	return m_map.insert( v ) ;
}

std::size_t Dictionary::size( ) const
{
	return m_map.size( ) ;
}

bool Dictionary::empty( ) const
{
	return m_map.empty( ) ;
}

void Dictionary::Add( const Name& key, const Object& value )
{
	m_map.insert( std::make_pair( key, value ) ) ;
}

std::istream& operator>>( std::istream& is, Dictionary& dict )
{
	TokenSrc s( is ) ;
	return (s >> dict).Stream() ;
}

TokenSrc& operator>>( TokenSrc& src, Dictionary& dict )
{
	Token t ;
	if ( src >> t && t.Get() != "<<" )
		throw ParseError( "dictionary not start with << " + t.Get( ) ) ;
	
	Dictionary temp ;
	while ( src >> t && t.Get() != ">>" )
	{
		src.PutBack( t ) ;
		
		Name	key ;
		Object	value ;
		
		if ( src >> key >> value )
			temp.m_map.insert( std::make_pair( key, value ) ) ;
	}
	
	if ( src )
		temp.swap( dict ) ;
	
	return src ;
}

std::ostream& operator<<( std::ostream& os, const Dictionary& dict )
{
	os << "<<\n" ;
	for ( Dictionary::const_iterator i  = dict.begin( ) ;
	                                 i != dict.end( ) ; ++i )
		os << i->first << ' ' << i->second << '\n' ;
	return os << ">>" ;
}

bool Dictionary::operator==( const Dictionary& dict ) const
{
	return m_map == dict.m_map ;
}

const Object& Dictionary::Default( )
{
	static const Object null_obj ;
	return null_obj ;
}

const Object& Dictionary::operator[]( const Name& name ) const
{
	const_iterator i = find( name ) ;
	return i == end( ) ? Default() : i->second ;
}

Object& Dictionary::operator[]( const Name& name )
{
	return m_map[name] ;
}

void Dictionary::erase( iterator pos )
{
	m_map.erase( pos ) ;
}

void Dictionary::erase( const Name& name )
{
	m_map.erase( name ) ;
}

} // end of namespace

namespace std
{
	void swap( pdf::Dictionary& dict1, pdf::Dictionary& dict2 )
	{
		dict1.swap( dict2 ) ;
	}
}
