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
	iterator i = m_map.find( name ) ;
	return i->second.IsNull() ? m_map.end() : i ;
}

Dictionary::const_iterator Dictionary::find( const Name& name ) const
{
	const_iterator i = m_map.find( name ) ;
	return i->second.IsNull() ? m_map.end() : i ;
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
		
		// null value means absent entry
		if ( src >> key >> value && !value.IsNull( ) )
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
	{
		// according to PDF spec, an absent key-pair is considered null
		if ( !i->second.IsNull( ) )
			os << i->first << ' ' << i->second << '\n' ;
	}
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

/*!	\brief	look-up the dictionary

	This operator will search the dictionary and try to find an entry with key
	\a key .
	\param	key	the key of the entry to be found
	\return		a reference to the value if \a key is found. otherwise
				a reference to a default null object
	\sa	Object::Object()
*/
const Object& Dictionary::operator[]( const Name& key ) const
{
	const_iterator i = find( key ) ;
	return i == end( ) ? Default() : i->second ;
}

/*!	\brief	look-up the dictionary and create a value if it does not exists

	This operator will search the dictionary and try to find an entry with
	key \a key . If the entry cannot be found, it will be created with a
	default null object as value.
	\param	key	the key of the entry to be found
	\return		a reference to the value
	\sa	Object::Object()
*/
Object& Dictionary::operator[]( const Name& key )
{
	return m_map[key] ;
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
