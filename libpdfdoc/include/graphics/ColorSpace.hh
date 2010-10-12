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

/**	\file	ColorSpace.hh
    \brief	definition the ColorSpace class
    \date	May 24, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_COLORSPACE_HEADER_INCLUDED__
#define __PDF_COLORSPACE_HEADER_INCLUDED__

#include "ColorSpec.hh"

namespace pdf {

class Color ;
class ColorMap ;

///	brief description
/**	\internal
	The ColorSpace class represents a color space in the PDF document. It is
	part of the graphics states. It is also used by images.
*/
class ColorSpace
{
public :
	virtual ~ColorSpace( ) ;
	
	virtual ColorSpec	Spec() const = 0 ;
	virtual ColorMap*	Map( ) const = 0 ;
	virtual bool		IsEqual( const ColorSpace *sp ) const = 0 ;
	virtual Color		DefaultColor() const = 0 ;
} ;

} // end of namespace

#endif // COLORSPACE_HH_
