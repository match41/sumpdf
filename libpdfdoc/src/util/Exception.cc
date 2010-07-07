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
#include "util/Util.hh"

#include "Backtrace.hh"

#include <boost/format.hpp>

#include <cstdlib>
#include <iterator>
#include <sstream>

namespace pdf {

Exception::Exception( boost::format fmt )
{
	*this << BacktraceInfo(Backtrace()) << ErrorMsg( fmt.str() ) ;
}

Exception::Exception( const std::string& str )
{
	*this << BacktraceInfo(Backtrace()) << ErrorMsg( str ) ;
}

void Exception::Add( const std::string& err )
{
	*this << ErrorMsg( err ) ;
}

void Exception::Add( boost::format fmt )
{
	*this << ErrorMsg( fmt.str() ) ;
}

std::string Exception::ErrorMessage( ) const
{
	if( const std::string *err = boost::get_error_info<ErrorMsg>(*this) )
		return *err ;
	else
		return std::string() ;
}

std::string Exception::GetBacktrace( ) const
{
	if( const Backtrace *err = boost::get_error_info<BacktraceInfo>(*this) )
		return err->ToString() ;
	else
		return std::string() ;
}

BadType::BadType(
	const std::type_info& 	from,
	const std::type_info& 	to,
	const std::string& 		err,
	const std::string&		obj )
	: Exception( boost::format( "Cannot convert \"%1%\" object to \"%2%\". "
	                            "exception: \"%3%\"\n"
	                            "object: %4%" )
	             % Demangle( from.name() )
	             % Demangle( to.name() )
	             % err
	             % obj )
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
