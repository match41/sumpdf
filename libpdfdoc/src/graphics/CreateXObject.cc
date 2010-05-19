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

/**	\file	CreateXObject.cc
	\brief	implementation of the XObject class
	\date	May 19, 2010
	\author	Nestal Wan
*/

#include "XObject.hh"

#include "RealImage.hh"

#include "core/Dictionary.hh"
#include "stream/Stream.hh"

#include <iostream>

namespace pdf {

XObject* CreateXObject( Stream& s )
{
	const Dictionary& dict = s.Self() ;
	if ( dict["Subtype"].As<Name>() == Name("Image") )
		return new RealImage( s ) ;
	else
	{
		std::cout << "XObject type: " << dict["Subtype"] <<
			" is not supported yet." << std::endl ;
		return new RealImage( s ) ;
	}
}

} // end of namespace
