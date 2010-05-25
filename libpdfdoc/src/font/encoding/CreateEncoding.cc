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

/**	\file	CreateEncoding.cc
	\brief	implementation of the CreateEncoding class
	\date	May 26, 2010
	\author	Nestal Wan
*/

#include "BaseEncoding.hh"

#include "BuildInEncoding.hh"
#include "SimpleEncoding.hh"

#include "file/DictReader.hh"

namespace pdf {

/**	constructor
	
*/
BaseEncoding* CreateEncoding( Object& obj, File *file )
{
	if ( obj.Is<Name>() )
		return new BuildInEncoding( obj.As<Name>() ) ;
	
	else if ( obj.Is<Dictionary>() )
	{
		DictReader dr( obj.As<Dictionary>(), file ) ;
		return new SimpleEncoding( dr );
	}
	else
		return 0 ;
}

} // end of namespace
