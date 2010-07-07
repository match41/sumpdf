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
	\file	Exception.hh
	\brief	definition the Exception class
	\date	Wed Mar 4 2009
	\author	Nestal Wan
*/

#ifndef __PDF_EXCEPTION_HEADER_INCLUDED__
#define __PDF_EXCEPTION_HEADER_INCLUDED__

#include "util/Export.hh"

#include <boost/format/format_fwd.hpp>
#include <boost/exception.hpp>

#include <iosfwd>
#include <typeinfo>
#include <exception>
#include <string>

//#include <boost/shared_ptr.hpp>

namespace pdf {

/**	\defgroup	exception	Exception Classes
*/

///	base class for exception in libpdfdoc
/**	\ingroup exception
	This class is the base class for all exception class in libpdfdoc.
*/
class Exception :
	virtual public std::exception,
	virtual public boost::exception
{
public :
	explicit Exception( const std::string& err ) ;
	explicit Exception( boost::format fmt ) ;

	void Add( const std::string& err ) ;
	void Add( boost::format fmt ) ;

	std::string ErrorMessage( ) const ;
	std::string GetBacktrace( ) const ;
} ;

class Backtrace ;
typedef boost::error_info<struct BacktraceTag, Backtrace>	BacktraceInfo ;
typedef boost::error_info<struct ErrMsgTag, std::string>	ErrorMsg ;

///	Invalid type exception.
/**	\ingroup exception
	This exception will be thrown when the Object cannot convert its
	underlying data to a specific type. The what() member function will
	describe the expected and actual type of the data.
*/
class BadType : public Exception
{
public :
	BadType(
		const std::type_info&	from,
		const std::type_info&	to,
		const std::string&		err,
		const std::string&		obj = std::string() ) ;
} ;

///	Parse error exception.
/**	\ingroup exception
	This exception will be thrown when there is a parse error when reading
	a PDF file.
*/
class ParseError : public Exception
{
public :
	explicit ParseError( const std::string& err ) ;
	explicit ParseError( boost::format fmt ) ;
} ;

} // end of namespace

#endif
