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
	{
		m_map = new RealColorMap( obj.As<Array>(), file ) ;
	}
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

ColorMap*	RealColorSpace::Map( ) const
{
	return m_map ;
}

} // end of namespace
