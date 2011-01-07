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

/**	\file	RealColorSpace.hh
    \brief	definition the RealColorSpace class
    \date	May 21, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_REAL_COLOR_SPACE_HEADER_INCLUDED__
#define __PDF_REAL_COLOR_SPACE_HEADER_INCLUDED__

#include "util/RefCounter.hh"
#include "graphics/ColorSpace.hh"

#include "graphics/ColorSpec.hh"
#include "util/RefPtr.hh"

#include <cstddef>
#include <memory>

namespace pdf {

class Array ;
class Color ;
class Dictionary ;
class File ;
class Object ;
class Ref ;

///	brief Color space implementation
/**	\internal
	The RealColorSpace class is a concrete implementation of a color space.
*/
class RealColorSpace : public RefCounter, public ColorSpace 
{
public :
	typedef Object BaseType ;

public :
	explicit RealColorSpace( ColorSpec sp = gfx::rgb ) ;
	RealColorSpace( Object& obj, File *file ) ;
	RealColorSpace( const Color *map, std::size_t size ) ;
	virtual ~RealColorSpace( ) ;
	
	Ref Write( File *file ) const ;
	
	ColorSpec	Spec() const ;
	ColorMap*	Map( ) const ; 
	Color		DefaultColor() const ;
	std::size_t	ChannelCount() const ;
	
	bool		IsEqual( const ColorSpace *sp ) const ;

private :
	bool IsValid() const ;
	void FromObject( Object& obj, File *file ) ;
	void FromArray( Array& array, File *file ) ;
	void CreateICCProfile( Dictionary& dict, File *file ) ;

private :
	ColorSpec				m_space ;
	std::auto_ptr<ColorMap>	m_map ;
} ;

} // end of namespace

#endif // COLORSPACE_HH_
