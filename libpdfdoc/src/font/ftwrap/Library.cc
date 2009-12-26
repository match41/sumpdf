/***************************************************************************\
 *   Copyright (C) 2002 by Nestal Wan                                      *
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
/*!
	\file	Library.cc
	\brief	implementation of the Library class
	\date	2002-06-16
	\author	Nestal Wan
*/

// class header
#include "Library.hh"
#include "LibraryBody.hh"
#include "Exception.hh"

// standard headers
#include <cassert>

namespace ft {

Library::Library( ) : m_pimpl( new Body )
{
}

Library::~Library( )
{
}

Library::Body::Body( )
{
	FT_Error error = ::FT_Init_FreeType( &m_lib ) ;
	if ( error )
		throw Exception( ) ;
}

Library::Body::~Body( )
{
	if ( m_lib != 0 )
		::FT_Done_FreeType( m_lib ) ;
}

} // end of namespace
