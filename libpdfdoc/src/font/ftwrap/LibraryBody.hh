/***************************************************************************\
 *   Copyright (C) 2002 by Nestal Wan                                      *
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
\***************************************************************************/
/*!
	\file	LibraryBody.hh
	\brief	definition of the CLibrary::CBody
	\date	2002-06-16
	\author	Nestal Wan
*/

#ifndef __FREETYPE_LIBRARY_BODY_HEADER_INCLUDED__
#define __FREETYPE_LIBRARY_BODY_HEADER_INCLUDED__

#ifndef __FREETYPE_LIBRARY_HEADER_INCLUDED__
#include "Library.hh"
#endif

// freetype headers
#include <ft2build.h>
#include FT_FREETYPE_H

namespace ft {

class Library::Body
{
private :
	FT_Library m_lib ;

public :
	Body( ) ;
	~Body( ) ;
	
	FT_Library GetBody( ) { return m_lib ; }
} ;

} // end of namespace

#endif
