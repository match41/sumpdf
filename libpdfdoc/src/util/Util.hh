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
	\file	Util.hh
	\brief	Utility functions
	\date	Sun Mar 9 2008
	\author	Nestal Wan
	
	This file contains some utility functions that is not PDF specific.
*/

#ifndef __PDF_UTIL_HEADER_INCLUDED__
#define __PDF_UTIL_HEADER_INCLUDED__

#include <cstddef>
#include <utility>
#include <string>

namespace pdf {

/*!	\brief	get the begin iterator from an array
	\internal
	
	This function returns the begin "iterator" of an array. It is useful to
	treat an array like an STL container.
	
	For example:
\code
int array[10] = { 1, 2, 3, 4, 5 } ;
std::vector<int> v ;
std::copy( Begin(array), End(array), std::back_inserter( v ) ;
\endcode

	\param	array	reference to the array
	\return	the begin iterator of the array. i.e. \a array itself
	\sa End(), Count()
*/
template <typename T, std::size_t n>
T* Begin( T (&array)[n] )
{
	return array ;
}

/*!	\brief	get the end iterator from an array
	\internal
	
	This function returns the end "iterator" of an array. It is useful to
	treat an array like an STL container.
	
	\param	array	reference to the array
	\return	the end iterator of the array. i.e. \a array+n
	\sa Begin(), Count()
*/
template <typename T, std::size_t n>
T* End( T (&array)[n] )
{
	return array + n ;
}

/*!	\brief	get the number of elements in the array
	\internal

	This function will return the number of elements in the array.
	\return	the number of elements in the array
	\sa Begin(), End()
*/
template <typename T, std::size_t n>
std::size_t Count( T (&array)[n] )
{
	return n ;
}

struct DeRef
{
	template <typename T>
	const T& operator()( const T *t )
	{
		return *t ;
	}
} ;

struct SelectSecond
{
	template <typename K, typename T>
	const T& operator()( const std::pair<K,T>& p )
	{
		return p.second ;
	}
} ;

std::string Demangle( const char *name ) ;

} // end of namespace

#endif
