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
#include <iterator>
#include <sstream>
#include <sstream>

namespace pdf {

struct Exception::Impl
{
	std::string		what ;
	bool			what_dirty ;
	
	std::vector<std::string>	msgs ;
	Backtrace					backtrace ;
} ;

Exception::Exception( boost::format fmt )
	: m_impl( new Impl )
{
	Add( fmt.str() ) ;
}

Exception::Exception( const std::string& str )
	: m_impl( new Impl )
{
	Add( str ) ;
}

Exception::~Exception() throw ()
{
}

void Exception::Add( const std::string& err )
{
	m_impl->msgs.push_back( err ) ;
	m_impl->what_dirty = true ;
}

void Exception::Add( boost::format fmt )
{
	Add( fmt.str() ) ;
}

const char* Exception::what( ) const throw()
{
	if ( m_impl->what_dirty )
	{
		std::ostringstream oss ;
		std::copy( m_impl->msgs.begin(), m_impl->msgs.end(),
			std::ostream_iterator<std::string>( oss, "\n" ) ) ;
		oss << "Backtrace:\n" << m_impl->backtrace.ToString() ;
		m_impl->what = oss.str() ;
		m_impl->what_dirty = false ;
	}
	return m_impl->what.c_str() ;
}

std::string Exception::ErrorMessage( ) const
{
	std::ostringstream oss ;
	std::copy( m_impl->msgs.begin(), m_impl->msgs.end(),
		std::ostream_iterator<std::string>( oss, "\n" ) ) ;
	return oss.str() ;
}

std::string Exception::GetBacktrace( ) const
{
	return m_impl->backtrace.ToString() ;
}

BadType::BadType(
	const std::type_info& 	from,
	const std::type_info& 	to,
	const std::string& 		err )
	: Exception( boost::format( "Cannot convert \"%1%\" object to \"%2%\". "
	                            "exception: \"%3%\"" )
	             % Demangle( from.name() )
	             % Demangle( to.name() )
	             % err )
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
