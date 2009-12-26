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
	\file	Dictionary.hh
	\brief	definition the Dictionary class
	\date	Thu Mar 6 2008
	\author	Nestal Wan
*/

#ifndef __PDF_DICTIONARY_HEADER_INCLUDED__
#define __PDF_DICTIONARY_HEADER_INCLUDED__

#include "Object.hh"

#include <algorithm>
#include <map>

namespace pdf {

class Token ;
class TokenSrc ;

/*!	\brief	PDF dictionary object
	
	This class represents a PDF dictionary object. According to the PDF
	specification, a dictionary object is an associative table containing
	pairs of objects, known as the dictionary's entries. The first element of
	each entry is the key and the second element is the value. The key must be
	a name. The value can be any kind of object, including another dictionary.
	A dictionary entry whose value is null (see is equivalent to an absent
	entry.
*/
class Dictionary
{
private :
	typedef std::map<Name, Object>	MapType ;
	MapType							m_map ;

public :
	typedef MapType::value_type		value_type ;
	typedef MapType::iterator		iterator ;
	typedef MapType::const_iterator	const_iterator ;

public :
	Dictionary( ) ;

	void swap( Dictionary& dict ) ;

	iterator begin( ) ;
	iterator end( ) ;
	
	const_iterator begin( ) const ;
	const_iterator end( ) const ;
	
	iterator find( const Name& name ) ;
	const_iterator find( const Name& name ) const ;

	std::pair<iterator,bool> insert( const value_type& v ) ;
	void erase( iterator pos ) ;
	void erase( const Name& name ) ;

	const Object& operator[]( const Name& key ) const ;
	Object& operator[]( const Name& key ) ;

	std::size_t size( ) const ;
	bool empty( ) const ;
	void clear( ) ;

	friend std::istream& operator>>( std::istream& is, Dictionary& dict ) ;
	friend std::ostream& operator<<( std::ostream& os,
	                                 const Dictionary& dict ) ;
	friend TokenSrc& operator>>( TokenSrc& src, Dictionary& dict ) ;
	
	bool operator==( const Dictionary& dict ) const ;
	
	void Add( const Name& key, const Object& value ) ;

	/*!	\brief	get and remove a value from the dictionary
	
		This function will search the dictionary for \a key and swap it to
		\a value . The value is then removed.
		\param	key		the key to search
		\param	value	the result
		\return			true if \a key is found in the dictionary, otherwise
						false
		\throw	BadType	if \a key is found but the value is not of type \a T
	*/
	template <typename T>
	bool Extract( const Name& key, T& value )
	{
		iterator i = m_map.find( key ) ;
		if ( i != m_map.end( ) )
		{
			std::swap( value, i->second.As<T>() ) ;
			m_map.erase( i ) ;
			return true ;
		}
		else
			return false ;
	}

private :
	static const Object& Default( ) ;
} ;

} // end of namespace

namespace std
{
	void swap( pdf::Dictionary& dict1, pdf::Dictionary& dict2 ) ;
}

#endif
