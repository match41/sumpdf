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
#include "core/Ref.hh"
#include "file/ArrayReader.hh"
#include "file/DictReader.hh"
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
	FromObject( obj, file ) ;

	if ( !IsValid() )
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

Ref RealColorSpace::Write( File *file ) const
{
	return Ref() ;
}

void RealColorSpace::FromObject( Object& obj, File *file )
{
	if ( obj.Is<Name>() )
		m_space = ParseSpec( obj.As<Name>().Str() ) ;
	
	else if ( obj.Is<Array>() )
		FromArray( obj.As<Array>(), file ) ;
}

void RealColorSpace::FromArray( Array& array, File *file )
{
	if ( array.empty() )
		throw Exception() << expt::ErrMsg( "empty array in color space" ) ;
	
	Name name = array[0].As<Name>() ;
	if ( name == "Indexed" )
		m_map.reset( new RealColorMap( array, file ) ) ;
	
	else if ( name == "ICCBased" && array.size() >= 2 )
	{
		ArrayReader ar( array, file ) ;
		Stream profile ;
		if ( ar.Detach( 1, profile ) )
			CreateICCProfile( profile.Self(), file ) ;
	}
}

void RealColorSpace::CreateICCProfile( Dictionary& dict, File *file )
{
	DictReader dr( dict, file ) ;
	
	std::size_t channel_count = 0 ;
	if ( !dr.Detach( "N", channel_count ) )			
		throw Exception() << expt::ErrMsg( "number of channel is "
			"required for ICCBased color space" ) ;
	
	// ICC profile is not supported yet
	// treat the color space as specified in "Alternate"
	Object alt ;
	if ( dr.Detach( "Alternate", alt ) )
	{
		FromObject( alt, file ) ;
		
		if ( Color::ChannelCount(m_space) != channel_count )
			throw Exception() << expt::ErrMsg( "number of channel in "
				"ICCBased color space not match with alternate" ) ;
	}
	// alternate color space is missing, use RGB, CMKY or gray depends
	// on number of channel
	else
	{
		switch ( channel_count )
		{
		case 1: m_space = gfx::gray; 	break ;
		case 3: m_space = gfx::rgb; 	break ;
		case 4: m_space = gfx::cmyk; 	break ;
		default:
			throw Exception() << expt::ErrMsg( "invalid number of "
				"channel in ICCBased color space" ) ;
		}
	}
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

bool RealColorSpace::IsValid() const
{
	return m_space != gfx::none || m_map.get() != 0 ;
}

} // end of namespace
