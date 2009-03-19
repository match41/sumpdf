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
	\brief	definition the Util class
	\date	Sun Mar 9 2008
	\author	Nestal Wan
*/

#ifndef __PDF_UTIL_HEADER_INCLUDED__
#define __PDF_UTIL_HEADER_INCLUDED__

#include <cstddef>
#include <utility>
#include <string>

namespace pdf {

/*!	\brief	brief description
	
	this class represents
*/
template <typename T, std::size_t n>
T* Begin( T (&array)[n] )
{
	return array ;
}

template <typename T, std::size_t n>
T* End( T (&array)[n] )
{
	return array + n ;
}

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
