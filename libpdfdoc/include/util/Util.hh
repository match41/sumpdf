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
	\file	Util.hh
	\brief	Utility functions
	\date	Sun Mar 9 2008
	\author	Nestal Wan
	
	This file contains some utility functions that is not PDF specific.
*/

#ifndef __PDF_UTIL_HEADER_INCLUDED__
#define __PDF_UTIL_HEADER_INCLUDED__

#include <cstddef>
#include <iosfwd>
#include <string>
#include <utility>

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

std::string Demangle( const char *name ) ;

struct DeletePtr
{
	typedef void result_type ;
	
	template <typename T>
	void operator()( T *t )
	{
		delete t ;
	}
} ;

template <typename T>
struct NewPtr
{
	typedef T* result_type ;
	
	T* operator()( )
	{
		return new T ;
	}
	
	template <typename A1>
	T* operator()( A1 a )
	{
		return new T(a) ;
	}
	
	template <typename A1, typename A2>
	T* operator()( A1 a1, A2 a2 )
	{
		return new T(a1, a2) ;
	}

	template <typename A1, typename A2, typename A3>
	T* operator()( A1 a1, A2 a2, A3 a3 )
	{
		return new T(a1, a2, a3) ;
	}
} ;

std::ostream& PrintHex( std::ostream& os, const void *buf, std::size_t size ) ;

#ifdef WIN32
#define strcasecmp _stricmp
#endif

} // end of namespace

#endif
