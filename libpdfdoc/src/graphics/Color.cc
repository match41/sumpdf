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

/**	\file	Colour.cc
	\brief	implementation of the Colour class
	\date	Feb 17, 2010
	\author	Nestal Wan
*/

#include "graphics/Color.hh"

#include "util/Debug.hh"
#include "util/Util.hh"

#include <ostream>
#include <iterator>
#include <algorithm>

namespace pdf {

Color::Color( )
	: m_cs( gray )
{
	std::fill( Begin(m_channel), End(m_channel), 0.0 ) ;
}

Color::Color( double gray_val )
{
	AssignGray( gray_val ) ;
}

Color::Color( double r, double g, double b )
{
	AssignRGB( r, g, b ) ;
}

Color::Color( double c, double m, double y, double k )
{
	AssignCMYK( c, m, y, k ) ;
}

Color::Space Color::ColorSpace( ) const
{
	return m_cs ;
}

void Color::AssignGray( double gray_val )
{
	m_cs = gray ;
	m_channel[0] = gray_val ;
}

void Color::AssignRGB( double r, double g, double b )
{
	m_cs = rgb ;
	m_channel[0] = r ;
	m_channel[1] = g ;
	m_channel[2] = b ;
}

void Color::AssignCMYK( double c, double m, double y, double k )
{
	m_cs = cmyk ;
	m_channel[0] = c ;
	m_channel[1] = m ;
	m_channel[2] = y ;
	m_channel[3] = k ;
}

double Color::Red( ) const
{
	PDF_ASSERT( m_cs == rgb ) ;
	return m_channel[0] ;
}

double Color::Green( ) const
{
	PDF_ASSERT( m_cs == rgb ) ;
	return m_channel[1] ;
}

double Color::Blue( ) const
{
	PDF_ASSERT( m_cs == rgb ) ;
	return m_channel[2] ;
}

double Color::Cyan( ) const
{
	PDF_ASSERT( m_cs == cmyk ) ;
	return m_channel[0] ;
}

double Color::Magenta( ) const
{
	PDF_ASSERT( m_cs == cmyk ) ;
	return m_channel[1] ;
}

double Color::Yellow( ) const
{
	PDF_ASSERT( m_cs == cmyk ) ;
	return m_channel[2] ;
}

double Color::Black( ) const
{
	PDF_ASSERT( m_cs != cmyk ) ;
	return m_channel[3] ;
}

double Color::Gray( ) const
{
	PDF_ASSERT( m_cs == gray ) ;
	return m_channel[0] ;
}

Color::iterator Color::begin( ) const
{
	return m_channel ;
}

Color::iterator Color::end( ) const
{
	return m_channel + ChannelCount() ;
}

std::size_t Color::ChannelCount( ) const
{
	static const std::size_t count[] =
	{
		3, 1, 4
	} ;
	
	PDF_ASSERT( m_cs >= rgb && m_cs <= cmyk ) ; 
	return count[m_cs] ;
}

bool Color::operator==( const Color& rhs ) const
{
	return m_cs == rhs.m_cs && std::equal( begin(), end(), rhs.begin() ) ; 
}

bool Color::operator!=( const Color& colour ) const
{
	return !operator==( colour ) ;
}

std::ostream& operator<<( std::ostream& os, const Color& t )
{
	switch ( t.ColorSpace() )
	{
		case Color::rgb:	os << "RGB(" ; break ;
		case Color::gray:	os << "Gray(" ; break ;
		case Color::cmyk:	os << "CMYK(" ; break ;
	}
	
	std::copy( t.begin(), t.end(), std::ostream_iterator<double>( os, " " ) ) ;
	
	return os << ")" ;
}

} // end of namespace
