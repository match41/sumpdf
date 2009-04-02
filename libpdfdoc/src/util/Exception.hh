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
	\file	Exception.hh
	\brief	definition the Exception class
	\date	Wed Mar 4 2009
	\author	Nestal Wan
*/

#ifndef __PDF_EXCEPTION_HEADER_INCLUDED__
#define __PDF_EXCEPTION_HEADER_INCLUDED__

#include <boost/format/format_fwd.hpp>

#include <iosfwd>
#include <stdexcept>
#include <typeinfo>

namespace pdf {

/*!	\brief	base class for exception in libpdfdoc
	
	This class is the base class for all exception class in libpdfdoc.
*/
class Exception : public std::runtime_error
{
public :
	explicit Exception( const std::string& err = std::string( ),
	                    bool backtrace = true ) ;
	explicit Exception( boost::format fmt, bool backtrace = true ) ;
} ;

/*!	\brief	Invalid type exception.
	
	This exception will be thrown when the Object cannot convert its
	underlying data to a specific type. The what() member function will
	describe the expected and actual type of the data.
*/
class BadType : public Exception
{
public :
	BadType( const std::type_info& from, const std::type_info& to,
	         const std::exception& e ) ;
} ;

class ParseError : public Exception
{
public :
	explicit ParseError( const std::string& err ) ;
	explicit ParseError( boost::format fmt ) ;
} ;

} // end of namespace

#endif
