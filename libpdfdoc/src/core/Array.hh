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
	\file	Array.hh
	\brief	definition the Array class
	\date	Thu Mar 6 2008
	\author	Nestal Wan
*/

#ifndef __PDF_ARRAY_HEADER_INCLUDED__
#define __PDF_ARRAY_HEADER_INCLUDED__

#ifndef __PDF_OBJECT_HEADER_INCLUDED__
#include "Object.hh"
#endif

#include <iosfwd>
#include <vector>

namespace pdf {

class Token ;

/*!	\brief	The PDF array object.
	\internal

	This class represents the PDF array object. An array object contains
	multiple PDF objects. They may be of different types. This class provides
	an interface like STL vector to provide access to them.
*/
class Array
{
private :
	typedef std::vector<Object> VecType ;
	VecType	m_array ;

public :
	typedef VecType::iterator 		iterator ;
	typedef VecType::const_iterator	const_iterator ;

public :
	Array( ) ;
	explicit Array( std::size_t count, const Object& obj = Object() ) ;

	template <typename InputIt>
	Array( InputIt first, InputIt last )
		: m_array( first, last )
	{
	}

	template <typename T, std::size_t n>
	Array( const T (&array)[n] )
		: m_array( array, array + n )
	{
	}

	friend std::istream& operator>>( std::istream& is, Array& array ) ;

	void swap( Array& array ) ;

	iterator begin( ) ;
	iterator end( ) ;
	const_iterator begin( ) const ;
	const_iterator end( ) const ;

	std::size_t size( ) const ;
	bool empty( ) const ;
	void clear( ) ;

	template <typename InputIt>
	void insert( iterator it, InputIt first, InputIt last )
	{
		m_array.insert( it, first, last ) ;
	}

	void push_back( const Object& obj ) ;

	bool operator==( const Array& array ) const ;
	bool operator!=( const Array& array ) const ;
	bool operator<( const Array& array ) const ;

	/// converts the array to an STL container
	template <template <typename> class Tem, typename T>
	operator Tem<T>( ) const
	{
		return Tem<T>( begin( ), end( ) ) ;
	}

	Object& operator[]( std::size_t index ) ;
	const Object& operator[]( std::size_t index ) const ;

	Object& at( std::size_t i ) ;
	const Object& at( std::size_t i ) const ;
} ;

std::ostream& operator<<( std::ostream& is, const Array& array ) ;

} // end of namespace

namespace std
{
	void swap( pdf::Array& dict1, pdf::Array& dict2 ) ;
}

#endif
