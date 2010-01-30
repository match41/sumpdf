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
#include "util/Debug.hh"

#include <boost/bind.hpp>

#include <istream>

#include <iostream>

namespace pdf {

Dictionary::Dictionary( )
{
}

///	Swaps two dictionaries.
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
	return i != m_map.end() && i->second.Is<void>() ? m_map.end() : i ;
}

Dictionary::const_iterator Dictionary::find( const Name& name ) const
{
	const_iterator i = m_map.find( name ) ;
	return i != m_map.end() && i->second.Is<void>() ? m_map.end() : i ;
}

///	Insert an entry to the dictionary.
/**	This function will add an entry to the dictionary. If the value of the
	entry is a null Object, the entry will not be added. It will be erase
	instead if it already exists.
	\param	v	The new entry to be added. It is an std::pair of <tt>const
				Name</tt> and \c Object.
	\return	A pair of iterator and a boolean. The iterator denotes the position
			of the newly added entry. The boolean is true if the object is
			newly inserted, i.e. it doesn't exists before. If \a v.second
			is a null Object, the iterator returned will be \c end().
	\sa Add()
*/
std::pair<Dictionary::iterator, bool> Dictionary::insert( const value_type& v )
{
	std::pair<iterator, bool> r = m_map.insert( v ) ;
	if ( v.second.Is<void>() )
	{
		m_map.erase( r.first ) ;
		return std::make_pair( m_map.end(), false ) ;
	}
	else
		return r ;
}

/// Returns the number of entries in this dictionary.
/**	Unlike some containers in STL, this function is not constant time. It needs
	to search the map to exclude any null objects accidentally being added. Use
	empty() instead whenever possible because empty() is faster then size().
	\return	The number of entries in the dictionary, excluding null objects.
	\sa	empty()
*/
std::size_t Dictionary::size( ) const
{
	return std::count_if(
		m_map.begin(),
		m_map.end(),
		!boost::bind( &Object::Is<void>,
			boost::bind( &value_type::second, _1 ) ) ) ;  
}

/// Returns true if the dictionary is empty.
/**	Like size(), this function is not constant time either. It will search
	for any non-null value in the entries and return true immediately when
	found. Therefore it is a bit faster then size(). It is recommended over
	size() if you just want to know if it is empty but not the actually
	number of entries.
	\return	\c true if the dictionary is empty, otherwise \c false.
	\sa	size()
*/
bool Dictionary::empty( ) const
{
	return std::find_if(
		m_map.begin(),
		m_map.end(),
		!boost::bind( &Object::Is<void>,
			boost::bind( &value_type::second, _1 ) ) ) == m_map.end() ;  
}

///	Erase all entries in the dictionary.
void Dictionary::clear( )
{
	m_map.clear( ) ;
}

///	Adds an entry to the dictionary.
/**	It's like insert() but it takes the \a key and \a value separately as
	two arguments. If \a value is a null Object, it will not be added.
	\param	key		The key of the new entry.
	\param	value	The value of the new entry. If it is a null Object, this
					function will do nothing.
	\sa insert()
*/
void Dictionary::Add( const Name& key, const Object& value )
{
	PDF_ASSERT( !value.Is<Stream>() ) ;

	insert( std::make_pair( key, value ) ) ;
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
		throw ParseError(
			"dictionary not start with \"<<\" but \"" + t.Get( ) + "\"" ) ;
	
	Dictionary temp ;
	while ( src >> t && t.Get() != ">>" )
	{
		src.PutBack( t ) ;
		
		Name	key ;
		Object	value ;
		
		// null value means absent entry
		if ( src >> key >> value && !value.Is<void>( ) )
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
		PDF_ASSERT( !i->second.Is<Stream>() ) ;
		PDF_ASSERT( !i->first.empty() ) ;
		
		try
		{
			// according to PDF spec, an absent key-pair is considered null
			if ( !i->second.Is<void>( ) )
				os << i->first << ' ' << i->second << '\n' ;
		}
		catch ( std::exception& )
		{
			std::cerr << "cannot write " << i->first << std::endl ;
			throw ;
		}
	}
	return os << ">>" ;
}

bool Dictionary::operator==( const Dictionary& dict ) const
{
	return m_map == dict.m_map ;
}

/*!	\brief	look-up the dictionary

	This operator will search the dictionary and try to find an entry with key
	\a key. According to the PDF specification, a dictionary entry whose value
	is a null Object is equivalent to an absent entry. As a result, if the
	entry is not found, a null Object will be returned.
	
	\param	key	the key of the entry to be found
	\return	a reference to the value if \a key is found. otherwise
			Object::NullObj() will be returned.
	\sa	Object::NullObj()
*/
const Object& Dictionary::operator[]( const Name& key ) const
{
	const_iterator i = m_map.find( key ) ;
	return i == m_map.end( ) ? Object::NullObj() : i->second ;
}

/*!	\brief	look-up the dictionary and create a value if it does not exists

	This operator will search the dictionary and try to find an entry with
	key \a key . If the entry cannot be found, it will be created with a
	default null object as value.

	\param	key	the key of the entry to be found
	\return	a reference to the value
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
