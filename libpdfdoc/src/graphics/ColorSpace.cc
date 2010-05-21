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

/**	\file	ColorSpace.cc
	\brief	implementation of the ColorSpace class
	\date	May 21, 2010
	\author	Nestal Wan
*/

#include "graphics/ColorSpace.hh"

#include "core/Object.hh"
#include "file/ArrayReader.hh"
#include "stream/Stream.hh"
#include "util/Exception.hh"
#include "util/Debug.hh"

#include <cstring>
#include <vector>

namespace pdf {

struct ColorSpace::Map
{
	std::vector<unsigned char>	components ;
} ;

/**	constructor
	
*/
ColorSpace::ColorSpace( Color::Space sp )
	: m_space( sp )
{
}

ColorSpace::ColorSpace( Object& obj, File *file )
{
	if ( obj.Is<Name>() )
		m_space = NameToSpace( obj.As<Name>().Str() ) ;
	else if ( obj.Is<Array>() )
	{
		ArrayReader ar( obj.As<Array>(), file ) ;
		
		Name name, base ;
		int hival ;
		Object lookup ;
		
		if (ar.Detach( 0, name )	&& name.Str() == "Indexed"	&&
			ar.Detach( 1, base )	&&
			ar.Detach( 2, hival )	&& hival <= 255	&& hival >= 0 &&
			ar.Detach( 3, lookup )	&&
			(lookup.Is<std::string>() || lookup.Is<Stream>() ) )
		{
			m_map.reset( new Map ) ;
			m_map->components.resize( hival + 1 ) ;
			m_space = NameToSpace( base.Str() ) ;
			
			if ( lookup.Is<std::string>() )
			{
				const std::string& s = lookup.As<std::string>() ;
				std::size_t size = std::min(m_map->components.size(), s.size());
				std::memcpy( &m_map->components[0], &s[0], size ) ;
			}
			else
			{
				Stream& s = lookup.As<Stream>() ;
				s.CopyData( &m_map->components[0], m_map->components.size() ) ;
			}
		}
	}
}

ColorSpace::ColorSpace( const Color *map, std::size_t size )
{
	for ( std::size_t i = 0 ; i < size ; ++i )
	{
		
	}
}

bool ColorSpace::IsIndex( ) const
{
	return m_map.get() != 0 ;
}

Color ColorSpace::Lookup( unsigned val ) const
{
	PDF_ASSERT( m_map.get() != 0 ) ;
	return Color() ;
}

Color::Space ColorSpace::Get() const
{
	return m_space ;
}

Color::Space ColorSpace::NameToSpace( const std::string& name )
{
	if ( name == "DeviceGray" )
		return Color::gray ;
	else if ( name == "DeviceRGB" )
		return Color::rgb ;
	else if ( name == "DeviceCMYK" )
		return Color::cmyk ;
	else
		throw Exception( "unknown color space name" ) ;
}

} // end of namespace
