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
	\file	Rect.cc
	\brief	implementation the Rect class
	\date	Sat Mar 15 2008
	\author	Nestal Wan
*/

#include "Rect.hh"

#include <algorithm>

namespace pdf {

Rect::Rect( )
{
	std::fill( Begin(m_corner), End(m_corner), 0 ) ;
}

Rect::Rect( int llx, int lly, int urx, int ury )
{
	m_corner[0] = llx ;
	m_corner[1] = lly ;
	m_corner[2] = urx ;
	m_corner[3] = ury ;
}

bool operator==( const Rect& r1, const Rect& r2 )
{
	return std::equal( Begin(r1.m_corner), End(r1.m_corner),
	                   Begin(r2.m_corner) ) ;
}

bool operator!=( const Rect& r1, const Rect& r2 )
{
	return !operator==( r1, r2 ) ;
}

Rect::const_iterator Rect::begin( ) const
{
	return Begin(m_corner) ;
}

Rect::const_iterator Rect::end( ) const
{
	return End(m_corner) ;
}

} // end of namespace
