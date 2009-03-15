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

/*!	\brief	brief description
	
	this class represents
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

	const Object& operator[]( const Name& name ) const ;
	Object& operator[]( const Name& name ) ;

	std::size_t size( ) const ;
	bool empty( ) const ;

	friend std::istream& operator>>( std::istream& is, Dictionary& dict ) ;
	friend std::ostream& operator<<( std::ostream& os,
	                                 const Dictionary& dict ) ;
	friend TokenSrc& operator>>( TokenSrc& src, Dictionary& dict ) ;
	
	bool operator==( const Dictionary& dict ) const ;
	
	void Add( const Name& key, const Object& value ) ;

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
