/***************************************************************************
 *   Copyright (C) 2006 by Nestal Wan                                      *
 *   me@nestal.net                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
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
 ***************************************************************************/

/*!
	\file	Util.cc
	\brief	implementation the Util class
	\date	Wed Mar 4 2009
	\author	Nestal Wan
*/

#include "Util.hh"

#include <cassert>

#ifndef _WIN32
#include <cxxabi.h>
#endif

#include <cstdlib>

namespace pdf {

std::string Demangle( const char *name )
{
#ifndef _WIN32
	assert( name != 0 ) ;

	char *cname = abi::__cxa_demangle( name, 0, 0, 0 ) ;
	std::string result( name ) ;
	if ( cname != 0 )
	{
		result = cname ;
		std::free( cname ) ;
	}
	return result ;
#else
    return std::string( ) ;
#endif
}

} // end of namespace
