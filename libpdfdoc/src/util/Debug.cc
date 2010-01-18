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

/**	\file	Debug.cc
	\brief	implementation of the Debug class
	\date	Jan 17, 2010
	\author	Nestal Wan
*/

#include "Debug.hh"

#include "Backtrace.hh"

#include <iostream>
#include <cstdlib>

namespace pdf {

void AssertFail( const char *text, const char *file, unsigned line )
{
	std::cerr
		<< file << ":" << line << ": Assertion \"" << text << "\" failed.\n"
		<< "Call stack:\n"
		<< Backtrace(3) << std::endl ;
	std::abort( ) ;
}

} // end of namespace
