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
	\file	libpdfdoc.cc
	\brief	implementation the export functions for the libpdfdoc DLL
	\date	Fri Apr 11 2008
	\author	Nestal Wan
*/

#include "libpdfdoc.hh"

#include "RealDoc.hh"
#include "graphics/RealPath.hh"
#include "graphics/RealText.hh"
#include "graphics/RealTextLine.hh"

namespace pdf {

///	Exported function to create a document object
/**	This function is exported from the libpdfdoc DLL to create a document
	object. A pointer to the Doc interface is returned. Clients must use
	interact with the PDF document by using the Doc interface pointer. After
	using the Doc object, clients must delete it.
	\return	a pointer to the Doc interface of the PDF document object. Clients
			must delete it after use.
*/
LIBPDFDOC_API Doc*  CreateDoc( )
{
	return new RealDoc ;
}

/// Exported function to get the version string of libpdfdoc.
LIBPDFDOC_API const char* Version( )
{
	static const char version[] = VERSION ;
	return version ;
}

LIBPDFDOC_API Text* CreateText( const GraphicsState& ts )
{
	return new RealText( ts ) ;
}

LIBPDFDOC_API Path* CreatePath( const GraphicsState& gs )
{
	return new RealPath( gs ) ;
}

LIBPDFDOC_API TextLine* CreateTextLine(
	const GraphicsState& 	state,		
	const Matrix&			transform,
	const std::wstring&		text )
{
	return new RealTextLine( state, transform, text ) ;
}


} // end of namespace
