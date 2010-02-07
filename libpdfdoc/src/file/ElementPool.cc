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

/**	\file	ElementPool.cc
	\brief	implementation of the ElementPool class
	\date	Feb 7, 2010
	\author	Nestal Wan
*/

#include "ElementPool.hh"

#include "util/Debug.hh"

namespace pdf {

/**	constructor
	
*/
ElementPool::ElementPool( )
{
}

void ElementPool::Add( const Ref& key, RefCounter *element )
{
	if ( key != Ref() )
		m_pool.insert( MapEntry( key, element ) ) ;
}

Ref ElementPool::Find( RefCounter *element )
{
	MapType::right_iterator i = m_pool.right.find( element ) ;
	return i != m_pool.right.end() ? i->second : Ref() ;
}

bool ElementPool::Has( const Ref& key ) const
{
	return m_pool.left.find( key ) != m_pool.left.end( ) ;
}

void ElementPool::Clear( )
{
	m_pool.clear() ;
}

} // end of namespace
