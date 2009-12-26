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
	\file	Library.hh
	\brief	definition of the freetype library wrapper
	\date	2002-06-16
	\author	Nestal Wan
*/

#ifndef __FREETYPE_LIB_HEADER_INCLUDED__
#define __FREETYPE_LIB_HEADER_INCLUDED__

// for auto_ptr
#include <memory>

namespace ft {

/*!	\class	Library
	\brief	the freetype library object wrapper
	
	this class wraps up the freetype library object. the library object is used
	to create other freetype objects, such as font faces.
*/
class Library
{
public :
	class Body ;
	
private :
	//! pointer to implementation. hides freetype data structure behind this
	//! pointer.
	const std::auto_ptr<Body> m_pimpl ;

public :
	Library( ) ;
	~Library( ) ;
	
	Body* Pimpl( ) { return m_pimpl.get( ) ; }
} ;

} // end of namespace

#endif
