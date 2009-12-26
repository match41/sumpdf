/***************************************************************************\
 *   Copyright (C) 2009 by Nestal Wan                                      *
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
 \***************************************************************************/

/*!
	\file	RefCountObj.cpp
	\brief	definition the RefCountObj class
	\date	Dec 11, 2009
	\author	nestal
*/

#include "RefCountObj.hh"

namespace pdf {

RefCountObj::RefCountObj( )
	: m_count( 1 )
{
}

void RefCountObj::AddRef( )
{
	++m_count ;
}

bool RefCountObj::Release( )
{
	if ( --m_count == 0 )
	{
		delete this ;
		return true ;
	}
	else
		return false ;
}

std::size_t RefCountObj::UseCount( ) const
{
	return m_count ;
}

} // end of namespace
