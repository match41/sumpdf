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

/**	\file	FontDbTest.cc
	\brief	implementation of the FCFontDbTest class
	\date	Feb 14, 2010
	\author	Nestal Wan
*/

#include "FontDbTest.hh"

#include "font/FontDb.hh"

#include "mock/Assert.hh"

// freetype headers
#include <ft2build.h>
#include FT_FREETYPE_H

namespace pdfut {

using namespace pdf ;

FontDbTest::FontDbTest( )
{
}

void FontDbTest::setUp( )
{
}

void FontDbTest::tearDown( )
{
}

void FontDbTest::Test( )
{
	std::auto_ptr<FontDb> subject = CreateFontDb() ;
	std::vector<unsigned char> data = subject->FindFont( "Arial", "" ) ;
	CPPUNIT_ASSERT( !data.empty() ) ;
	
	FT_Face face = subject->LoadFont( &data[0], data.size() ) ;
	CPPUNIT_ASSERT( face != 0 ) ;
	PDFUT_ASSERT_EQUAL( face->family_name, std::string("Arial") ) ;
}

} // end of namespace
