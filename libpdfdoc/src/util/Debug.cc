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

#include "util/Debug.hh"

#include "Backtrace.hh"

#include <iostream>
#include <fstream>
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

namespace debug {

bool has_trace = false ;

// output stream to put debug messages
std::ostream& Trace()
{
	if ( has_trace )
		return std::cout ;
	else
	{
		static std::ofstream os ;
		return os ;
	}
}

void EnableTrace()
{
	has_trace = true ;
}

void DisableTrace()
{
	has_trace = false ;
}

} // end of namespace debug

} // end of namespace pdf
