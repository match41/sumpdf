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
	\file	Rect.cc
	\brief	implementation the Rect class
	\date	Sat Mar 15 2008
	\author	Nestal Wan
*/

#include "util/Rect.hh"
#include "util/Util.hh"

#include <algorithm>

namespace pdf {

Rect::Rect( )
{
	std::fill( Begin(m_corner), End(m_corner), 0 ) ;
}

Rect::Rect( double llx, double lly, double urx, double ury )
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

double Rect::Width( ) const
{
	return m_corner[2] - m_corner[0] ;
}

double Rect::Height( ) const
{
	return m_corner[3] - m_corner[1] ;
}

double Rect::Top( ) const
{
	return m_corner[3] ;
}

double Rect::Bottom( ) const
{
	return m_corner[1] ;
}

double Rect::Left( ) const
{
	return m_corner[0] ;
}

double Rect::Right( ) const
{
	return m_corner[2] ;
}

bool Rect::IsNull( ) const
{
	return
		m_corner[0] == 0.0 && m_corner[1] == 0.0 &&
		m_corner[2] == 0.0 && m_corner[3] == 0.0 ;
}

} // end of namespace
