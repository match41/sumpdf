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
	\file	Exception.cc
	\brief	implementation the Exception class
	\date	Wed Mar 4 2009
	\author	Nestal Wan
*/

#include "Exception.hh"

#include "Util.hh"
#include "SymbolInfo.hh"

#include <boost/format.hpp>

#include <cstdlib>
#include <sstream>

namespace pdf {

Exception::Exception( const std::string& err )
	: runtime_error( err )
{
}

Exception::Exception( boost::format fmt )
	: runtime_error( fmt.str() )
{
}

BadType::BadType( const std::type_info& from, const std::type_info& to,
				  const std::exception& e )
	: Exception( boost::format( "Cannot convert \"%1%\" object to \"%2%\". "
	                            "exception: \"%3%\""
#ifndef NDEBUG
	                            "\n%4%"
#endif
	                            )
	             % Demangle( from.name() )
	             % Demangle( to.name() )
	             % e.what() )
{
}

std::string BadType::ErrorMsg( const std::type_info& from,
	                           const std::type_info& to,
	                           const std::exception& e )
{
	std::ostringstream ss ;
	
	ss << "Cannot convert \"" << Demangle( from.name() )
	   << "\" object to \""   << Demangle( to.name() )
	   << "\". exception: \"" << e.what() << "\"" ;

#ifndef NDEBUG
	ss << "\n" ;
	SymbolInfo::Instance()->Backtrace( ss ) ;
#endif
	
	return ss.str() ;
}

ParseError::ParseError( const std::string& err )
	: Exception( err )
{
}

} // end of namespace
