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

/**	\file	RealColorSpaceTest.cc
	\brief	implementation of the ColorSpaceTest class
	\date	May 22, 2010
	\author	Nestal Wan
*/

#include "RealColorSpaceTest.hh"

#include "core/Array.hh"
#include "core/Name.hh"
#include "graphics/RealColorSpace.hh"
#include "graphics/Color.hh"
#include "graphics/ColorMap.hh"

#include "util/CArray.hh"

#include "mock/Assert.hh"
#include "mock/MockFile.hh"

namespace pdfut {

using namespace pdf ;

RealColorSpaceTest::RealColorSpaceTest( )
{
}

void RealColorSpaceTest::setUp( )
{
}

void RealColorSpaceTest::tearDown( )
{
}

void RealColorSpaceTest::Test( )
{
	const char s[] = "\0\xff\0\xff\0\0" ;
	std::string ss(s, Count(s) ) ;

	Object a[] = { Name("Indexed"), Name("DeviceRGB"), 1, ss } ;
	MockFile file ;
	Object src( (Array(a)) ) ;

	RealColorSpace subject( src, &file ) ;
	CPPUNIT_ASSERT( subject.Map() != 0 ) ;
	PDFUT_ASSERT_EQUAL( subject.Map()->Base(), gfx::rgb ) ;
	PDFUT_ASSERT_EQUAL( subject.Map()->LookUp(0), Color( 0,   1.0, 0 ) ) ;
	PDFUT_ASSERT_EQUAL( subject.Map()->LookUp(1), Color( 1.0, 0,   0 ) ) ;
	PDFUT_ASSERT_EQUAL( subject.Map()->LookUp(2), Color() ) ;
}

void RealColorSpaceTest::TestRecursive( )
{
}

} // end of namespace
