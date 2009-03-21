/***************************************************************************\
 *   Copyright (C) 2009 by Nestal Wan                                      *
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
\***************************************************************************/

/*!
	\file	Exception.cc
	\brief	implementation the Exception class
	\date	Mon Mar 16 2009
	\author	Nestal Wan
*/

#include "Exception.hh"
#include "ErrorTable.hh"

#ifdef _DEBUG
#include "SymbolInfo.hh"
#endif

#include <sstream>

namespace freetype {

Exception::Exception( const std::string& err )
	: pdf::Exception( err )
{
}

Exception::Exception( int err, const std::string& msg )
	: pdf::Exception( Message( err, msg ) )
{
}

std::string Exception::Message( int err, const std::string& msg )
{
	std::ostringstream ss ;
	ss << msg << ": " << LookupError( err ) ;

#ifdef _DEBUG
	ss << std::endl ;
	pdf::SymbolInfo::Backtrace( ss ) ;
#endif

	return ss.str( ) ;
}

} // end of namespace