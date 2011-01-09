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

/**	\file	RealColorMap.cc
	\brief	implementation of the RealColorMap class
	\date	Oct 12, 2010
	\author	Nestal Wan
*/

#include "RealColorMap.hh"

#include "core/Object.hh"

#include "graphics/Color.hh"
#include "graphics/RealColorSpace.hh"
#include "file/ArrayReader.hh"
#include "stream/Stream.hh"
#include "util/Debug.hh"
#include "util/Exception.hh"
#include "util/Functional.hh"

#include <boost/bind.hpp>

#include <cstring>

namespace pdf {

/**	constructor
	
*/
RealColorMap::RealColorMap( )
{
}


RealColorMap::RealColorMap( const Color *map, std::size_t size )
	: m_base( new RealColorSpace(
		size > 0 && map != 0 ? map[0].Spec() : gfx::none ) )
{
	// both equal to 0 is OK, otherwise both must not equal to 0.
	PDF_ASSERT( map  != 0 || size == 0 ) ;
	PDF_ASSERT( size != 0 || map  == 0 ) ;

	if ( size > 0 )
	{
		m_comp.resize( size * map[0].ChannelCount() ) ;
	
		for ( std::size_t i = 0 ; i < size ; ++i )
		{
			PDF_ASSERT( map[i].Spec() == map[0].Spec() ) ;
			map[i].GetAsRawByte( &m_comp[i*map[i].ChannelCount()] ) ;
		}
	}
}

RealColorMap::RealColorMap( Array& obj, File *file )
{
	ArrayReader ar( obj, file ) ;
	
	using namespace boost ;
	
	Name name ;
	int hival ;
	Object lookup ;

	if (ar.Detach( 0, name )	&& name.Str() == "Indexed"	&&
		ar.Detach( 2, hival )	&& hival <= 255	&& hival >= 0 &&
		ar.Detach( 3, lookup )	&&
		(lookup.Is<std::string>() || lookup.Is<Stream>() ) )
	{
		m_base = ar.Create( 1, bind( NewPtr<RealColorSpace>(), _1, file ) ) ;
		PDF_ASSERT( m_base != 0 ) ;
		
		m_comp.resize( (hival + 1) * m_base->ChannelCount() ) ;
		
		if ( lookup.Is<std::string>() )
		{
			const std::string& s = lookup.As<std::string>() ;
			std::size_t size = std::min(m_comp.size(), s.size());
			std::memcpy( &m_comp[0], &s[0], size ) ;
		}
		else
		{
			Stream& s = lookup.As<Stream>() ;
			s.CopyData( &m_comp[0], m_comp.size() ) ;
		}
	}
	else
	{
		throw Exception() << expt::ErrMsg( "invalid color map" ) ;
	}
}

Color RealColorMap::LookUp( unsigned char val ) const
{
	PDF_ASSERT( m_base != 0 ) ;

	std::size_t count = Color::ChannelCount( m_base->Spec() ) ;
	if ( (val+1) * count <= m_comp.size() )
		return Color( m_base->Spec(), &m_comp[val*count] ) ;
	else
		return Color() ;
}

std::size_t RealColorMap::Count( ) const
{
	PDF_ASSERT( m_base != 0 ) ;
	return m_comp.size() / Color::ChannelCount( m_base->Spec() ) ;
}

ColorSpace* RealColorMap::Base( ) const
{
	return m_base.get() ;
}

} // end of namespace
