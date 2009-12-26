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
	\file	Types.hh
	\brief	convert freetype types to C types
	\date	2002-06-16
	\author	Nestal Wan
*/

// freetype headers
#include <ft2build.h>
#include FT_TYPES_H

namespace ft {

inline double FtFixed2Double( FT_Fixed fixed )
{
	return fixed / 65536.0f ;
}

inline double FtPos2Double( FT_Pos pos )
{
	return pos / 64.0f ;
}

} // end of namespace
