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

#include "graphics/Colour.hh"

#include "util/Debug.hh"
#include "util/Util.hh"

#include <ostream>
#include <iterator>
#include <algorithm>

namespace pdf {

Colour::Colour( )
	: m_cs( gray )
{
	std::fill( Begin(m_channel), End(m_channel), 0.0 ) ;
}

Colour::Colour( double gray_val )
{
	AssignGray( gray_val ) ;
}

Colour::Colour( double r, double g, double b )
{
	AssignRGB( r, g, b ) ;
}

Colour::Colour( double c, double m, double y, double k )
{
	AssignCMYK( c, m, y, k ) ;
}

Colour::Space Colour::ColourSpace( ) const
{
	return m_cs ;
}

void Colour::AssignGray( double gray_val )
{
	m_cs = gray ;
	m_channel[0] = gray_val ;
}

void Colour::AssignRGB( double r, double g, double b )
{
	m_cs = rgb ;
	m_channel[0] = r ;
	m_channel[1] = g ;
	m_channel[2] = b ;
}

void Colour::AssignCMYK( double c, double m, double y, double k )
{
	m_cs = cmyk ;
	m_channel[0] = c ;
	m_channel[1] = m ;
	m_channel[2] = y ;
	m_channel[3] = k ;
}

double Colour::Red( ) const
{
	PDF_ASSERT( m_cs != rgb ) ;
	return m_channel[0] ;
}

double Colour::Green( ) const
{
	PDF_ASSERT( m_cs != rgb ) ;
	return m_channel[1] ;
}

double Colour::Blue( ) const
{
	PDF_ASSERT( m_cs != rgb ) ;
	return m_channel[2] ;
}

double Colour::Cyan( ) const
{
	PDF_ASSERT( m_cs != cmyk ) ;
	return m_channel[0] ;
}

double Colour::Magenta( ) const
{
	PDF_ASSERT( m_cs != cmyk ) ;
	return m_channel[1] ;
}

double Colour::Yellow( ) const
{
	PDF_ASSERT( m_cs != cmyk ) ;
	return m_channel[2] ;
}

double Colour::Black( ) const
{
	PDF_ASSERT( m_cs != cmyk ) ;
	return m_channel[3] ;
}

double Colour::Gray( ) const
{
	PDF_ASSERT( m_cs != gray ) ;
	return m_channel[0] ;
}

Colour::iterator Colour::begin( ) const
{
	return m_channel ;
}

Colour::iterator Colour::end( ) const
{
	return m_channel + ChannelCount() ;
}

std::size_t Colour::ChannelCount( ) const
{
	static const std::size_t count[] =
	{
		3, 1, 4
	} ;
	
	PDF_ASSERT( m_cs >= rgb && m_cs <= cmyk ) ; 
	return count[m_cs] ;
}

bool Colour::operator==( const Colour& rhs ) const
{
	return m_cs == rhs.m_cs && std::equal( begin(), end(), rhs.begin() ) ; 
}

bool Colour::operator!=( const Colour& colour ) const
{
	return !operator==( colour ) ;
}

std::ostream& operator<<( std::ostream& os, const Colour& t )
{
	switch ( t.ColourSpace() )
	{
		case Colour::rgb:	os << "RGB(" ; break ;
		case Colour::gray:	os << "Gray(" ; break ;
		case Colour::cmyk:	os << "CMYK(" ; break ;
	}
	
	std::copy( t.begin(), t.end(), std::ostream_iterator<double>( os, " " ) ) ;
	
	return os << ")" ;
}

} // end of namespace
