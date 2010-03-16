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

#include "libpdfdoc.hh"

namespace pdf {

/*!	\brief	brief description
	
	this class represents
*/
class LIBPDFDOC_API Rect
{
private :
	double m_corner[4] ;

public :
	Rect( ) ;
	Rect( double llx, double lly, double urx, double ury ) ;

	template <typename InputIt>
	Rect( InputIt first, InputIt last )
	{
		Assign( first, last ) ;
	}
	
	template <typename InputIt>
	void Assign( InputIt first, InputIt last )
	{
		for ( double *dest = m_corner;
		      dest != m_corner + 4 && first != last ; ++dest, ++first )
			*dest = *first ;
	}
	
	friend bool operator==( const Rect& r1, const Rect& r2 ) ;
	friend bool operator!=( const Rect& r1, const Rect& r2 ) ;
	
	typedef const double* const_iterator ;
	const_iterator begin( ) const ;
	const_iterator end( ) const ;
	
	double Width( ) const ;
	double Height( ) const ;
	
	double Top( ) const ;
	double Bottom( ) const ;
	double Left( ) const ;
	double Right( ) const ;
	
	bool IsNull( ) const ;
} ;

} // end of namespace

#endif
