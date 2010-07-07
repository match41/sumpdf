/***************************************************************************\
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
\***************************************************************************/

/**	\file	Functional.hh
    \brief	definition the Functional class
    \date	Jul 7, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_FUNCTIONAL_HH_EADER_INCLUDED__
#define __PDF_FUNCTIONAL_HH_EADER_INCLUDED__

namespace pdf {

struct DeRef
{
	template <typename T>
	const T& operator()( const T *t )
	{
		return *t ;
	}
} ;

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

} // end of namespace

#endif // FUNCTIONAL_HH_
