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

/**	\file	ColorMap.hh
    \brief	definition the ColorMap class
    \date	Oct 12, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_COLORMAP_HH_EADER_INCLUDED__
#define __PDF_COLORMAP_HH_EADER_INCLUDED__

#include "ColorSpec.hh"

#include <cstddef>

namespace pdf {

class Color ;

///	brief description
/**	\internal
	The ColorMap class represents
*/
class ColorMap
{
public :
	virtual ~ColorMap( ) ;
	
	virtual Color LookUp( unsigned char idx ) const = 0 ;
	virtual std::size_t Count( ) const = 0 ;
	virtual ColorSpec Base( ) const = 0 ;
} ;

} // end of namespace

#endif // COLORMAP_HH_
