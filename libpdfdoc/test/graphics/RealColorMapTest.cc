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

/**	\file	RealColorMapTest.cc
	\brief	implementation of the RealColorMapTest class
	\date	Jan 9, 2011
	\author	Nestal Wan
*/

#include "RealColorMapTest.hh"

#include "graphics/RealColorMap.hh"

#include "graphics/Color.hh"
#include "util/CArray.hh"
#include "mock/Assert.hh"
#include "mock/MockFile.hh"

#include <iostream>

namespace pdfut {

using namespace pdf ;

RealColorMapTest::RealColorMapTest( )
{
}

void RealColorMapTest::setUp( )
{
}

void RealColorMapTest::tearDown( )
{
}

void RealColorMapTest::TestReadWrite( )
{
	Color c[] = { Color(), Color(1.0) } ;
	RealColorMap subject( c, Count(c) ) ;
	
	MockFile file ;
	Ref r = subject.Write( &file ) ;
	
	Object o = file.ReadObj( r ) ;
	
	CPPUNIT_ASSERT( o.Is<pdf::Array>() ) ;
	
	RealColorMap sub2( o.As<pdf::Array>(), &file ) ;
	PDFUT_ASSERT_EQUAL( sub2.Count(), 2U ) ;
	PDFUT_ASSERT_EQUAL( sub2.LookUp(0), Color() ) ;
	PDFUT_ASSERT_EQUAL( sub2.LookUp(1), Color(1.0) ) ;
}

} // end of namespace
