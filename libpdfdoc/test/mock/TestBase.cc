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

/**	\file	TestBase.cc
	\brief	implementation of the TestBase class
	\date	Jan 23, 2010
	\author	Nestal Wan
*/

#include "TestBase.hh"

// freetype headers
#include <ft2build.h>
#include FT_FREETYPE_H

namespace pdfut {

/**	constructor
*/
TestBase::TestBase( )
	: m_ft( 0 )
{
}

void TestBase::setUp( )
{
	::FT_Init_FreeType( &m_ft ) ;
}

void TestBase::tearDown( )
{
	::FT_Done_FreeType( m_ft ) ;
	m_ft = 0 ;
}

} // end of namespace
