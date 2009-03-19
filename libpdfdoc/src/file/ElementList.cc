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
	\file	ElementList.cc
	\brief	implementation the ElementList class
	\date	Sat Apr 19 2008
	\author	Nestal Wan
*/

#include "ElementList.hh"

#include "IElement.hh"

namespace pdf {

ElementList::ElementList( std::size_t size )
	: m_vec( size )
{
}

std::size_t ElementList::size( ) const
{
	return m_vec.size( ) ;
}

bool ElementList::empty( ) const
{
	return m_vec.empty( ) ;
}

ElementList::iterator ElementList::begin( )
{
	return iterator( m_vec.begin( ) ) ;
}

ElementList::iterator ElementList::end( )
{
	return iterator( m_vec.end( ) ) ;
}

ElementList::const_iterator ElementList::begin( ) const
{
	return const_iterator( m_vec.begin( ) ) ;
}

ElementList::const_iterator ElementList::end( ) const
{
	return const_iterator( m_vec.end( ) ) ;
}

void ElementList::push_back( IElement *element )
{
	m_vec.push_back( element ) ;
}

} // end of namespace
