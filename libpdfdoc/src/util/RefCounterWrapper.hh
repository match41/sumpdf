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

/**
    \file	RefCountObjWrapper.hh
    \brief	definition the RefCountObjWrapper class
    \date	Jan 1, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_REFCOUNTOBJWRAPPER_HEADER_INCLUDED__
#define __PDF_REFCOUNTOBJWRAPPER_HEADER_INCLUDED__

#include "RefCounter.hh"

namespace pdf {

/**	\brief	brief description

	The RefCountObjWrapper class represent
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
