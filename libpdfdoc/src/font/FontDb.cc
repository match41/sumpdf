/***************************************************************************\
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
\***************************************************************************/

/**	\file	FontDb.cc
	\brief	implementation of the FontDb class
	\date	Feb 11, 2010
	\author	Nestal Wan
*/

#include "font/FontDb.hh"

namespace pdf {

/**	constructor
	
*/
FontDb::~FontDb( )
{
}

} // end of namespace

///////////////////////////////////////////////////////////////////////////
// probably we should create another new C++ source file 

#ifdef HAVE_FONTCONFIG
	#include "FCFontDb.hh"
#elif defined WIN32
	#include "Win32FontDb.hh"
#else
	#error No suitable FontDb implementation
#endif

namespace pdf {

std::auto_ptr<FontDb> CreateFontDb( )
{
#ifdef HAVE_FONTCONFIG
	return std::auto_ptr<FontDb>( new FCFontDb ) ;

#elif defined WIN32
	return std::auto_ptr<FontDb>( new Win32FontDb ) ;

#else
	// should never runs
	return std::auto_ptr<FontDb>() ;
#endif
}

} // end of namespace
