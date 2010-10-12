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

/**	\file	ColorValue.hh
    \brief	definition the ColorValue class
    \date	Oct 12, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_COLORVALUE_HH_EADER_INCLUDED__
#define __PDF_COLORVALUE_HH_EADER_INCLUDED__

#include "util/RefPtr.hh"

#include <iosfwd>

namespace pdf {

class Color ;
class ColorSpace ;
class ColorValue ;
class Name ;

///	brief description
/**	\internal
	The ColorValue class represents
*/
class ColorValue
{
public :
	typedef const double* iterator ;

public :
	ColorValue( ) ;
	ColorValue( const ColorSpace *space ) ;
	ColorValue( const ColorSpace *space, double v1 ) ;
	ColorValue( const ColorSpace *space, double v1, double v2 ) ;
	ColorValue( const ColorSpace *space, double v1, double v2, double v3 ) ;
	ColorValue( const ColorSpace *space, double v1, double v2, double v3,
		double v4 );
	explicit ColorValue( const Name& cs ) ;
	explicit ColorValue( const Color& color ) ;

//	ColorValue& operator=( const ColorValue& c ) ;
	void Swap( ColorValue& c ) ;

	bool operator==( const ColorValue& val ) const ;
	bool operator!=( const ColorValue& val ) const ;

	Color Get() const ;
	const ColorSpace* Space() const ;

	iterator begin( ) const ;
	iterator end( ) const ;
	
	void SetToDefault( ) ;

private :
	const ColorSpace	*m_space ;
	double				m_value[4] ;
} ;

// no need to access members directly
std::ostream& operator<<( std::ostream& os, const ColorValue& v ) ;

} // end of namespace

#endif // COLORVALUE_HH_
