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
	\file	Rect.hh
	\brief	definition the Rect class
	\date	Wed Mar 19 2008
	\author	Nestal Wan
*/

#ifndef __PDF_RECT_HEADER_INCLUDED__
#define __PDF_RECT_HEADER_INCLUDED__

#include "Util.hh"

namespace pdf {

/*!	\brief	brief description
	
	this class represents
*/
class Rect
{
private :
	int m_corner[4] ;

public :
	Rect( ) ;
	Rect( int llx, int lly, int urx, int ury ) ;

	template <typename InputIt>
	Rect( InputIt first, InputIt last )
	{
		for ( int *dest = Begin(m_corner) ;
		      dest != End(m_corner) && first != last ; ++dest, ++first )
			*dest = *first ;
	}
	
	friend bool operator==( const Rect& r1, const Rect& r2 ) ;
	friend bool operator!=( const Rect& r1, const Rect& r2 ) ;
	
	typedef const int* const_iterator ;
	const_iterator begin( ) const ;
	const_iterator end( ) const ;
} ;

} // end of namespace

#endif
