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

/**	\file	RealColorMap.hh
    \brief	definition the RealColorMap class
    \date	Oct 12, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_REALCOLORMAP_HH_EADER_INCLUDED__
#define __PDF_REALCOLORMAP_HH_EADER_INCLUDED__

#include "graphics/ColorMap.hh"
#include "util/RefCounter.hh"

#include "RealColorSpace.hh"
#include "util/RefPtr.hh"

#include <vector>

namespace pdf {

class Array ;
class ColorSpace ;
class File ;
class RealColorSpace ;
class Ref ;

///	brief description
/**	\internal
	The RealColorMap class represents
*/
class RealColorMap : public ColorMap, public RefCounter
{
public :
	RealColorMap( ) ;
	RealColorMap( Array& obj, File *file ) ;
	RealColorMap( const Color *map, std::size_t size ) ;

	Color LookUp( unsigned char idx ) const ;
	std::size_t Count( ) const ;
	ColorSpace* Base( ) const ;
	
	Ref Write( File *file ) const ;

private :
	boost::intrusive_ptr<RealColorSpace>	m_base ;
	std::vector<unsigned char>				m_comp ;
} ;

} // end of namespace

#endif // REALCOLORMAP_HH_
