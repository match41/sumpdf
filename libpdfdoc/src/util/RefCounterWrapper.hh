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

/**	\file	RefCounterWrapper.hh
    \brief	definition the RefCounterWrapper class
    \date	Jan 1, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_REFCOUNTERWRAPPER_HEADER_INCLUDED__
#define __PDF_REFCOUNTERWRAPPER_HEADER_INCLUDED__

#include "util/RefCounter.hh"

namespace pdf {

///	A simple template to turn any class to a reference counted one.
/**	\internal
	The RefCountObjWrapper class represent a wrapper around any
	object (i.e. \a	T) to make it supports reference counting.
	\tparam	T	The class of the object being wrapped. It must be copy-
				constructible.
*/
template <typename T>
class RefCounterWrapper : public RefCounter
{
public :
	RefCounterWrapper( const T& t )
	: m_t( t )
	{
	}
	
	T& Get( )
	{
		return m_t ;
	}
	
	const T& Get( ) const
	{
		return m_t ;
	}

private :
	T m_t ;
} ;

} // end of namespace

#endif // REFCOUNTOBJWRAPPER_HH_
