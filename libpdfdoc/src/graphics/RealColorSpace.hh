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

#include <memory>
#include <string>

namespace pdf {

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
	explicit RealColorSpace( Color::Space sp = Color::rgb ) ;
	RealColorSpace( Object& obj, File *file ) ;
	RealColorSpace( const Color *map, std::size_t size ) ;
	~RealColorSpace( ) ;
	
	bool IsIndex( ) const ;
	Color Lookup( unsigned val ) const ;
	std::size_t ColorCount( ) const ;
	
	Color::Space Get() const ;
	
	friend std::ostream& operator<<( std::ostream& os, const RealColorSpace& cs ) ;

private :
	static Color::Space	NameToSpace( const std::string& name ) ;

private :
	// color space enum. if space is index, this is the color space of the
	// color map
	Color::Space		m_space ;

	// color map. only valid when space is index
	struct	Map ;
	std::auto_ptr<Map>	m_map ;
} ;

} // end of namespace

#endif // COLORSPACE_HH_
