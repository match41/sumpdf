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

#include "core/Array.hh"
#include "core/Object.hh"
#include "file/ArrayReader.hh"
#include "graphics/Color.hh"
#include "stream/Stream.hh"
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
	debug::Tracer tracer;

	if ( obj.Is<Name>() )
		m_space = ParseSpec( obj.As<Name>().Str() ) ;
	
	else if ( obj.Is<Array>() )
	{
		Array& a = obj.As<Array>() ;
		if ( a.empty() )
			throw Exception() << expt::ErrMsg( "empty array in color space" ) ;
		
		Name name = a[0].As<Name>() ;
		if ( name == "Indexed" )
			m_map.reset( new RealColorMap( obj.As<Array>(), file ) ) ;
		
		else if ( name == "ICCBased" && a.size() >= 2 )
		{
			ArrayReader ar( a, file ) ;
			Stream profile ;
			if ( ar.Detach( 1, profile ) )
			{
				debug::Trace() << "profile = " << profile.Self() << std::endl ;
				const Dictionary& pdict = profile.Self() ;
				Name alt = pdict["Alternate"].As<Name>() ;
				debug::Trace() << "alt = " << alt << std::endl ;
				
				m_space = ParseSpec( alt.Str() ) ;
				debug::Trace() << "space = " << m_space << std::endl ;
			}
		}
		
		else
			debug::Trace() << "unknown cs name: " << name << std::endl ;
		
		if ( m_space == gfx::none && m_map.get() == 0 )
			throw Exception() << expt::ErrMsg( "unsupported color space" ) ;
	}
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

std::size_t	RealColorSpace::ChannelCount() const
{
	return m_map.get() != 0 ? 1 : Color::ChannelCount( m_space ) ;
}

} // end of namespace
