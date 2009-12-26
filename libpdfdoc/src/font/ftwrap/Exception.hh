/*
	libpdf++: a C++ Free library to generate PDF file
	Copyright (C) 2002 Nestal Wan

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/
/*!
	\file	Exception.hh
	\brief	exception for the freetype wrapper classes
	\date	2002-06-16
	\author	Nestal Wan
*/

#ifndef __FREETYPE_ERROR_HEADER_INCLUDED__
#define __FREETYPE_ERROR_HEADER_INCLUDED__

#include "util/Exception.hh"

// standard headers
#include <string>

namespace ft {

class Exception : public pdf::Exception
{
public :
	explicit Exception( const std::string& msg = std::string( ) ) ;
	Exception( int error, const std::string& msg = std::string( ) ) ;

private :
	static std::string Message( int err, const std::string& msg ) ;
} ;

} // end of namespace

#endif
