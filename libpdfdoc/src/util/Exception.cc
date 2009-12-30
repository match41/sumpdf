/***************************************************************************
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
 ***************************************************************************/

/*!
	\file	Exception.cc
	\brief	implementation the Exception class
	\date	Wed Mar 4 2009
	\author	Nestal Wan
*/

#include "util/Exception.hh"

#include "Util.hh"
#include "Backtrace.hh"

#include <boost/format.hpp>

#include <cstdlib>
#include <sstream>
#include <iostream>

namespace pdf {

Exception::Exception( boost::format fmt, bool backtrace )
	: runtime_error( backtrace ? fmt.str() + "\nBacktrace:\n" +
                                 Backtrace().ToString() : fmt.str() )
{
}

Exception::Exception( const std::string& str, bool backtrace )
	: runtime_error( backtrace ? str + "\nBacktrace:\n"
	                                 + Backtrace().ToString( ) : str )
{
}

BadType::BadType( const std::type_info& from, const std::type_info& to,
				  const std::exception& e )
	: Exception( boost::format( "Cannot convert \"%1%\" object to \"%2%\". "
	                            "exception: \"%3%\"" )
	             % Demangle( from.name() )
	             % Demangle( to.name() )
	             % e.what() )
{
}

ParseError::ParseError( const std::string& err )
	: Exception( err )
{
}

ParseError::ParseError( boost::format fmt )
	: Exception( fmt )
{
}

} // end of namespace
