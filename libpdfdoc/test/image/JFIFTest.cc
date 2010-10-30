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

/**	\file	JFIFTest.cc
	\brief	implementation of the JFIFTest class
	\date	Oct 30, 2010
	\author	Nestal Wan
*/

#include "JFIFTest.hh"

#include "image/JFIF.hh"

#include "mock/Assert.hh"
#include "util/Debug.hh"

namespace pdfut {

using namespace img ;
using namespace pdf ;

JFIFTest::JFIFTest( )
{
}

void JFIFTest::setUp( )
{
//	debug::EnableTrace() ;
}

void JFIFTest::tearDown( )
{
	debug::DisableTrace() ;
}

void JFIFTest::Test( )
{
	std::ifstream src(
		(std::string(TEST_DATA_DIR) + "3x3bgr.jpg").c_str(),
		std::ios::in | std::ios::binary ) ;

	CPPUNIT_ASSERT( src ) ;
	JFIF subject( src.rdbuf() ) ;
	PDFUT_ASSERT_EQUAL( subject.Size(), 360U ) ;
}

} // end of namespace
