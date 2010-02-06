/***************************************************************************\
 *   Copyright (C) 2009 by Nestal Wan                                      *
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

/*!
	\file	RefCounter.cc
	\brief	definition the RefCountObj class
	\date	Dec 11, 2009
	\author	nestal
*/

#include "RefCounter.hh"

namespace pdf {

RefCounter::RefCounter( )
	: m_count( 1 )
{
}

RefCounter::~RefCounter( )
{
}

void RefCounter::AddRef( )
{
	++m_count ;
}

bool RefCounter::Release( )
{
	if ( --m_count == 0 )
	{
		delete this ;
		return true ;
	}
	else
		return false ;
}

std::size_t RefCounter::UseCount( ) const
{
	return m_count ;
}

} // end of namespace
