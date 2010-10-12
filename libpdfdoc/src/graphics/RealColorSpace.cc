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

/**	\file	RealColorSpace.cc
	\brief	implementation of the ColorSpace class
	\date	May 21, 2010
	\author	Nestal Wan
*/

#include "RealColorSpace.hh"

#include "RealColorMap.hh"

#include "core/Object.hh"
#include "graphics/Color.hh"
#include "util/Exception.hh"
#include "util/Debug.hh"

#include <iostream>

namespace pdf {

/**	constructor
	
*/
RealColorSpace::RealColorSpace( ColorSpec sp )
	: m_space( sp )
{
}

RealColorSpace::RealColorSpace( Object& obj, File *file )
	: m_space( gfx::none )
{
	if ( obj.Is<Name>() )
		m_space = ParseSpec( obj.As<Name>().Str() ) ;
	
	else if ( obj.Is<Array>() )
		m_map.reset( new RealColorMap( obj.As<Array>(), file ) ) ;
	else
		throw Exception() << expt::ErrMsg( "invalid color space" ) ;
}

RealColorSpace::RealColorSpace( const Color *map, std::size_t size )
	: m_space( gfx::none )
	, m_map( new RealColorMap( map, size ) )
{
}

RealColorSpace::~RealColorSpace( )
{
}

ColorSpec RealColorSpace::Spec() const
{
	return m_space ;
}

ColorMap* RealColorSpace::Map( ) const
{
	return m_space == gfx::none ? m_map.get() : 0 ;
}

bool RealColorSpace::IsEqual( const ColorSpace *sp ) const
{
	PDF_ASSERT( sp != 0 ) ;

	if ( m_space == sp->Spec() )
	{
		// color map is the same if present
		if ( m_map.get() != 0 && m_map.get() != sp->Map() )
			return false ;
	
		return true ;
	}
	else
		return false ;
}

Color RealColorSpace::DefaultColor() const
{
	static const double channels[] = { 0, 0, 0, 1 } ;
	switch ( m_space )
	{
	case gfx::rgb:
	case gfx::gray:
	case gfx::cmyk:		return Color( m_space, channels ) ;
	case gfx::none:
		PDF_ASSERT( m_map.get() != 0 ) ;
		PDF_ASSERT( m_map->Count() > 0 ) ;
		return m_map->LookUp(0) ;
	}
}

} // end of namespace
