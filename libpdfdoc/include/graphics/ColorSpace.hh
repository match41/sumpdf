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

#ifndef __PDF_COLORSPACE_HH_EADER_INCLUDED__
#define __PDF_COLORSPACE_HH_EADER_INCLUDED__

#include "Color.hh"

namespace pdf {

///	brief description
/**	\internal
	The ColorSpace class represents
*/
class ColorSpace
{
public :
	virtual ~ColorSpace( ) ;
	
	virtual bool IsIndex( ) const = 0 ;
	virtual Color Lookup( unsigned val ) const = 0 ;
	virtual std::size_t ColorCount( ) const = 0 ;
	
	virtual Color::Space Get() const = 0 ;
} ;

} // end of namespace

#endif // COLORSPACE_HH_
