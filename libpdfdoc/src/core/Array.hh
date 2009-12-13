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

/*!	\brief	brief description

	This class represents the PDF array object.
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

	template <typename InputIt>
	Array( InputIt first, InputIt last )
		: m_array( first, last )
	{
	}

	friend std::istream& operator>>( std::istream& is, Array& array ) ;
	friend TokenSrc& operator>>( TokenSrc& src, Array& obj ) ;

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
