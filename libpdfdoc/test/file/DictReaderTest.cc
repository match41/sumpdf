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

/**	\file	DictReaderTest.cc
	\brief	implementation of the DictReaderTest class
	\date	Feb 16, 2010
	\author	Nestal Wan
*/

#include "DictReaderTest.hh"

#include "core/Array.hh"
#include "core/Dictionary.hh"
#include "file/DictReader.hh"

#include "mock/Assert.hh"
#include "mock/MockFile.hh"

#include "util/CArray.hh"

#include <algorithm>

namespace pdfut {

using namespace pdf ;

DictReaderTest::DictReaderTest( )
{
}

void DictReaderTest::setUp( )
{
}

void DictReaderTest::tearDown( )
{
}

void DictReaderTest::TestIntToDouble( )
{
	Dictionary dict ;
	dict.insert( "Test", 100 ) ;
	
	MockFile file ;
	DictReader subject( dict, &file ) ;
	
	double out ;
	CPPUNIT_ASSERT( subject.Detach( "Test", out ) ) ;
	PDFUT_ASSERT_EQUAL( out, 100.0 ) ;
}

void DictReaderTest::TestDetachVec( )
{
	int array[] = { 1, 2, 3, 4, 5 } ;

	Dictionary dict ;
	dict.insert( "SomeArray", Array( Begin(array), End(array) ) ) ;
	
	MockFile file ;
	DictReader subject( dict, &file ) ;
	
	std::vector<double> out ;
	CPPUNIT_ASSERT( subject.Detach( "SomeArray", out ) ) ;
	PDFUT_ASSERT_EQUAL( out.size(), Count(array) ) ;
	CPPUNIT_ASSERT( std::equal( out.begin(), out.end(), Begin(array) ) ) ;
}

void DictReaderTest::TestDetachIndirectVec( )
{
	Object array[] = { "1", "2", "3", "4", Ref( 1, 0 ) } ;
	
	Dictionary dict ;
	dict.insert( "SomeIndirectArray", Array( Begin(array), End(array) ) ) ;

	MockFile file ;
	file.AddObj( Ref(1,0), Object( "indirect" ) ) ;
	
	DictReader subject( dict, &file ) ;
	
	std::vector<std::string> out ;
	CPPUNIT_ASSERT( subject.Detach( "SomeIndirectArray", out ) ) ;
	PDFUT_ASSERT_EQUAL( out.size(), Count(array) ) ;
	PDFUT_ASSERT_EQUAL( out.back(), "indirect" ) ;
}

} // end of namespace
