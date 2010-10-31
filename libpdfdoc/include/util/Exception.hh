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
#include <boost/exception/exception.hpp>
#include <boost/exception/info.hpp>

#include <typeinfo>
#include <exception>
#include <string>

namespace pdf {

class Backtrace ;

/**	\defgroup	exception	Exception Classes
*/

///	base class for exception in libpdfdoc
/**	\ingroup exception
	This class is the base class for all exception class in libpdfdoc.
*/
struct Exception :
	virtual public std::exception,
	virtual public boost::exception
{
	Exception( ) ;
} ;

struct FileError	: virtual Exception {} ;

///	Parse error exception.
/**	\ingroup exception
	This exception will be thrown when there is a parse error when reading
	a PDF file.
*/
struct ParseError	: virtual Exception {} ;

///	Invalid type exception.
/**	\ingroup exception
	This exception will be thrown when the Object cannot convert its
	underlying data to a specific type. The what() member function will
	describe the expected and actual type of the data.
*/
struct BadType 		: virtual Exception {} ;

struct Unsupported	: virtual Exception {} ;

// Exception informations
namespace expt
{
	// back-trace information. should be present for all exceptions
	typedef boost::error_info<struct BacktraceTag, Backtrace>	BacktraceInfo ;

	// generic error message
	typedef boost::error_info<struct MsgTag, std::string>		ErrMsg ;

	// formatted error message
	typedef boost::error_info<struct FmtMsgTag, boost::format>	FormattedMsg ;

	// errno as in C
	typedef boost::error_info<struct ErrorNumberTag, int>		ErrorNumber ;

	// the filename of the file that caused the error
	typedef boost::error_info<struct FileNameTag, std::string>	FileName ;

	// the source type for the type conversion that caused the error
	typedef boost::error_info<struct SrcTypeTag, std::string>	SourceType ;
	
	// the destination type for the type conversion that caused the error
	typedef boost::error_info<struct DestTypeTag, std::string>	DestType ;

	// ID of the PDF object that caused the error
	typedef boost::error_info<struct ObjectIDTag, std::size_t>	ObjID ;

	// offset of the file
	typedef boost::error_info<struct OffsetTag, std::size_t>	Offset ;

	// token
	typedef boost::error_info<struct TokenTag, std::string>		Token ;

	// index to an array
	typedef boost::error_info<struct IndexTag, std::size_t>		Index ;

	// name
	typedef boost::error_info<struct NameTag, std::string>		Name ;
}

} // end of namespace

#endif
