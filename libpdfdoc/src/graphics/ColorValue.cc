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

/**	\file	ColorValue.cc
	\brief	implementation of the ColorValue class
	\date	Oct 12, 2010
	\author	Nestal Wan
*/

#include "ColorValue.hh"

#include "core/Name.hh"
#include "graphics/Color.hh"
#include "graphics/ColorMap.hh"
#include "graphics/ColorSpace.hh"
#include "graphics/ColorSpaces.hh"
#include "graphics/ColorSpec.hh"

#include "RealColorSpace.hh"

#include "util/CArray.hh"
#include "util/Debug.hh"

#include <ostream>
#include <algorithm>
#include <iterator>

namespace pdf {

ColorValue::ColorValue( )
	: m_space( gfx::Grayscale() )
{
	m_value[0] = 0 ;
}

ColorValue::ColorValue( const ColorSpace *space )
	: m_space( space )
{
	SetToDefault() ;
}

ColorValue::ColorValue( const ColorSpace *space, double v1 )
	: m_space( space )
{
	PDF_ASSERT( space != 0 ) ;
	PDF_ASSERT( Color::ChannelCount( space->Spec() ) == 1 ) ;
	m_value[0] = v1 ;
}

ColorValue::ColorValue( const ColorSpace *space, double v1, double v2 )
	: m_space( space )
{
	PDF_ASSERT( space != 0 ) ;
	PDF_ASSERT( Color::ChannelCount( space->Spec() ) == 2 ) ;
	m_value[0] = v1 ;
	m_value[1] = v2 ;
}

ColorValue::ColorValue( const ColorSpace *space, double v1, double v2,
	double v3 )
	: m_space( space )
{
	PDF_ASSERT( space != 0 ) ;
	PDF_ASSERT( Color::ChannelCount( space->Spec() ) == 3 ) ;
	m_value[0] = v1 ;
	m_value[1] = v2 ;
	m_value[2] = v3 ;
}

ColorValue::ColorValue( const ColorSpace *space, double v1, double v2, double v3,
	double v4 )
	: m_space( space )
{
	PDF_ASSERT( space != 0 ) ;
	PDF_ASSERT( Color::ChannelCount( space->Spec() ) == 4 ) ;
	m_value[0] = v1 ;
	m_value[1] = v2 ;
	m_value[2] = v3 ;
	m_value[3] = v4 ;
}

ColorValue::ColorValue( const Name& cs )
	: m_space( gfx::ParseColorSpace( cs.Str() ) )
{
	std::fill( Begin(m_value), End(m_value), 0.0 ) ;
}

ColorValue::ColorValue( const Color& color )
	: m_space( gfx::ParseColorSpace( color.Spec() ) )
{
	std::copy( color.begin(), color.end(), m_value ) ;
}

/*
ColorValue& ColorValue::operator=( const ColorValue& c )
{
	ColorValue temp( c ) ;
	Swap( temp ) ;
	return *this ;
}
*/

void ColorValue::Swap( ColorValue& c )
{
	std::swap( c.m_space, m_space ) ;
	std::swap_ranges( Begin(m_value), End(m_value), Begin(c.m_value) ) ;
}

void ColorValue::SetToDefault( )
{
	PDF_ASSERT( m_space != 0 ) ;
	Color c = m_space->DefaultColor() ;
	std::copy( c.begin(), c.end(), m_value ) ;
}

Color ColorValue::Get() const
{
	PDF_ASSERT( m_space != 0 ) ;

	if ( m_space->Map() != 0 )
		return m_space->Map()->LookUp( m_value[0] ) ;
	else
		return Color( m_space->Spec(), m_value ) ;
}

const ColorSpace* ColorValue::Space() const
{
	PDF_ASSERT( m_space != 0 ) ;
	return m_space ;
}

ColorValue::iterator ColorValue::begin( ) const
{
	return m_value ;
}

ColorValue::iterator ColorValue::end( ) const
{
	PDF_ASSERT( m_space != 0 ) ;
	return m_value + Color::ChannelCount( m_space->Spec() ) ;
}

std::ostream& operator<<( std::ostream& os, const ColorValue& v )
{
	for ( ColorValue::iterator i = v.begin() ; i != v.end() ; ++i )
	{
		os << *i ;
		if ( i + 1 != v.end() )
			os << ' ' ;
	}
	
	return os ;
}

bool ColorValue::operator==( const ColorValue& val ) const
{
	PDF_ASSERT( m_space != 0 ) ;
	PDF_ASSERT( val.m_space != 0 ) ;

	// color space equal and color value equal
	return m_space->IsEqual( val.m_space ) && std::equal( begin(), end(),
		val.begin() ) ;
}

bool ColorValue::operator!=( const ColorValue& val ) const
{
	return !operator==( val ) ;
}

} // end of namespace
