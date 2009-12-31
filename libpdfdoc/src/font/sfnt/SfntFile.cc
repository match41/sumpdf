/***************************************************************************\
 *   Copyright (C) 2009 by Nestal Wan                                      *
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

/**
	\file	SfntFile.cc
	\brief	definition the SfntFile class
	\date	Dec 22, 2009
	\author	Nestal Wan
*/

#include "SfntFile.hh"

extern "C"
{
	#include "sfnt.h"
}

namespace tex {

namespace
{
	inline sfnt* s( void *p )
	{
		return static_cast<sfnt*>( p ) ;
	}
}

SfntFile::SfntFile( FT_Face face )
	: m_sfnt(reinterpret_cast<Impl*>( ::sfnt_open( face, 0xffffffff ) ) )
{
}

SfntFile::~SfntFile( )
{
	::sfnt_close( s(m_sfnt) ) ;
}

} // end of namespace
