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

/**	\file	ColorSpaces.cc
	\brief	implementation of the ColorSpaces class
	\date	Oct 13, 2010
	\author	Nestal Wan
*/

#include "graphics/ColorSpaces.hh"

#include "RealColorSpace.hh"

#include "util/Exception.hh"

namespace pdf {
namespace gfx {

const ColorSpace* RGB( )
{
	static const RealColorSpace cs( rgb ) ;
	return &cs ;
}

const ColorSpace* Grayscale( )
{
	static const RealColorSpace cs( gray ) ;
	return &cs ;
}

const ColorSpace* CMYK( )
{
	static const RealColorSpace cs( cmyk ) ;
	return &cs ;
}

const ColorSpace* ParseColorSpace( const std::string& name )
{
	return ParseColorSpace( ParseSpec( name ) ) ;
}
const ColorSpace* ParseColorSpace( ColorSpec spec )
{
	switch ( spec )
	{
	case rgb:	return RGB() ;
	case gray:	return Grayscale() ;
	case cmyk:	return CMYK() ;
	default:	throw Exception() << expt::ErrMsg( "invalid color spec" ) ;
	}
}

} } // end of namespace
