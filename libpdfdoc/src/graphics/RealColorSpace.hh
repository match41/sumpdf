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
    \date	May 21, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_REAL_COLOR_SPACE_HEADER_INCLUDED__
#define __PDF_REAL_COLOR_SPACE_HEADER_INCLUDED__

#include "graphics/ColorSpace.hh"
#include "util/RefCounter.hh"
#include "util/RefPtr.hh"

#include "graphics/ColorSpec.hh"

#include <cstddef>

namespace pdf {

class Color ;
class File ;
class Object ;

///	brief description
/**	\internal
	The ColorSpace class represents
*/
class RealColorSpace : public ColorSpace, public RefCounter
{
public :
	typedef Object BaseType ;

public :
	explicit RealColorSpace( ColorSpec sp = gfx::rgb ) ;
	RealColorSpace( Object& obj, File *file ) ;
	RealColorSpace( const Color *map, std::size_t size ) ;
	~RealColorSpace( ) ;
	
	ColorSpec	Spec() const ;
	ColorMap*	Map( ) const ; 
	
	bool		IsEqual( const ColorSpace *sp ) const ;

private :
	ColorSpec	m_space ;
	ColorMap	*m_map ;
} ;

} // end of namespace

#endif // COLORSPACE_HH_
