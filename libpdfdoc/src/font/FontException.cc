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

/**	\file	FontException.cc
	\brief	implementation of the FontException class
	\date	Jan 23, 2010
	\author	Nestal Wan
*/

#include "FontException.hh"

#include "util/CArray.hh"

// freetype headers
#include <ft2build.h>
#include FT_FREETYPE_H

#include <boost/format.hpp>

#include <map>

// setup freetype error table
#undef __FTERRORS_H__
#define FT_ERRORDEF( e, v, s )  std::pair<const int, const char*>( e, s ),
#define FT_ERROR_START_LIST     {
#define FT_ERROR_END_LIST       };

namespace pdf {

namespace
{
	const std::pair<const int, const char*> ft_errors[] =
	#include FT_ERRORS_H

	const std::map<int, const char*> ft_error_map(
		Begin(ft_errors), End(ft_errors) ) ;
}

namespace expt
{
	std::string LookUpFtError( int fterror )
	{
		std::map<int, const char*>::const_iterator i =
			ft_error_map.find( fterror );
		
		return i != ft_error_map.end()
			? std::string(i->second != 0 ? i->second : "" )
			: std::string() ;
	}
}

} // end of namespace
