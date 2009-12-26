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
	\file	FaceBody.hh
	\brief	definition of the freetype face wrapper class
	\date	2002-06-16
	\author	Nestal Wan
	
	this header file is private. applications should not include it.
	it should include Face.hh instead.
*/

#ifndef __FREETYPE_FACE_BODY_HEADER_INCLUDED__
#define __FREETYPE_FACE_BODY_HEADER_INCLUDED__

// class headers
#include "Face.hh"

// freetype headers
#include <ft2build.h>
#include FT_FREETYPE_H

namespace ft {

struct Face::Body
{
	FT_Face m_face ;
} ;

} // end of namespace

#endif
