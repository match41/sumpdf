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

/**	\file	FontEncodingTest.cc
	\brief	implementation of the FontEncodingTest class
	\date	Mar 21, 2010
	\author	Nestal Wan
*/

#include "FontEncodingTest.hh"

#include "file/DictReader.hh"

#include "font/FontEncoding.hh"

#include "mock/Assert.hh"
#include "mock/MockFile.hh"

#include <sstream>

namespace pdfut {

using namespace pdf ;

FontEncodingTest::FontEncodingTest( )
{
}

void FontEncodingTest::setUp( )
{
}

void FontEncodingTest::tearDown( )
{
}

void FontEncodingTest::Test( )
{
	std::istringstream ss(
		"<</Type /Encoding/Differences [ 0 /.notdef 15/bullet 16/.notdef]>>" ) ;
	
	Dictionary self ;
	CPPUNIT_ASSERT( ss >> self ) ;
	
	MockFile file ;
	DictReader dr( self, &file ) ;

	FontEncoding subject( dr ) ;
	PDFUT_ASSERT_EQUAL( subject.LookUp( 15 ), 8226 ) ;
}

} // end of namespace
