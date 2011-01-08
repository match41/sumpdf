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

/**	\file	ColorSpec.cc
	\brief	implementation of the ColorSpec class
	\date	Oct 12, 2010
	\author	Nestal Wan
*/

#include "graphics/ColorSpec.hh"

#include "util/Exception.hh"

#include <iostream>

namespace pdf {

ColorSpec ParseSpec( const std::string& name )
{
	if ( name == "DeviceGray" )
		return gfx::gray ;
	else if ( name == "DeviceRGB" )
		return gfx::rgb ;
	else if ( name == "DeviceCMYK" )
		return gfx::cmyk ;
	else
		return gfx::none ;
}

const std::string& SpecName( ColorSpec spec )
{
	static const std::string map[] = {"DeviceRGB", "DeviceGray", "DeviceCMYK"} ;
	static const std::string empty ;

std::cout << "spec is " << spec << std::endl ;
	return spec >= gfx::rgb && spec < gfx::none ? map[spec] : empty ;
}

} // end of namespace
