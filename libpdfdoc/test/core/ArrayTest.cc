/***************************************************************************
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
 ***************************************************************************/

/*!
	\file	ArrayTest.cc
	\brief	implementation the ArrayTest class
	\date	Wed Mar 19 2008
	\author	Nestal Wan
*/

#include "ArrayTest.hh"

#include "core/Array.hh"
#include "core/Dictionary.hh"
#include "util/Util.hh"

#include "core/Token.hh"
#include "core/TokenSrc.hh"

#include "mock/Assert.hh"

#include <algorithm>
#include <iterator>
#include <sstream>
#include <vector>

#include <iostream>

namespace pdfut {

using namespace pdf ;

ArrayTest::ArrayTest( )
{
}

void ArrayTest::TestNormal( )
{
	int a[] = { 1, 2, 3, 4, 5 } ;

	pdf::Array pa( pdf::Begin(a), pdf::End(a) ) ;
	
	std::vector<int> v( pa.begin( ), pa.end( ) ) ;
	CPPUNIT_ASSERT( v.size() == pdf::Count( a ) ) ;
	CPPUNIT_ASSERT( std::equal( v.begin(), v.end(), pa.begin() ) ) ;
}

void ArrayTest::TestDifferent( )
{
	pdf::Array subject ;
	subject.push_back( pdf::Object( 10 ) ) ;
	subject.push_back( pdf::Object( "string" ) ) ;
	subject.push_back( pdf::Object( 101.01f ) ) ;

	CPPUNIT_ASSERT( subject.size() == 3 ) ;
	CPPUNIT_ASSERT( subject[2].As<double>() == 101.01f ) ;
}

void ArrayTest::TestRead( )
{
	std::istringstream ss( "[10 /Hello 123.0 (am a string)]");
	pdf::Array sub ;
	CPPUNIT_ASSERT( ss >> sub ) ;
	PDFUT_ASSERT_EQUAL( sub[0], 10 ) ;
	PDFUT_ASSERT_EQUAL( sub[1].As<pdf::Name>(), pdf::Name("Hello") ) ;
	PDFUT_ASSERT_EQUAL( sub[2], 123.0 ) ;
/*	CPPUNIT_ASSERT( sub[3] == std::string("I am a string") ) ;
	pdf::Dictionary d ;
	d["Type"] = pdf::Name("Dict") ;
	CPPUNIT_ASSERT( sub[4] == d ) ;*/
}

void ArrayTest::TestString( )
{
	std::istringstream ss( "[ 549 3.14 false (Ralph) /SomeName ]" ) ;
	pdf::Array sub ;
	CPPUNIT_ASSERT( ss >> sub ) ;
}

void ArrayTest::TestTJ( )
{
	std::istringstream ss( "[<01111111111111111> 59 <02> 3 <02> 3 <02> -6 <02> "
	"3 <02> 3 <02> -6 <02> 3 <02> 3 <02> -6 <02> 3 <02> 3 <02> -6 <02>]" ) ;
	
	pdf::Array sub ;
	CPPUNIT_ASSERT( ss >> sub ) ;
	
	char obj1[] = { 0x01, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x10, '\0' } ;
	PDFUT_ASSERT_EQUAL( sub.size(), 27U ) ;
	PDFUT_ASSERT_EQUAL( sub[0].As<std::string>(), obj1 ) ;
}

void ArrayTest::TestGsTJ( )
{
	std::istringstream ss(
		"[(\x9)95.985(\x9)95.0775(\x3)94.9402(\x4)97.6199(\x5)99.0967"
		"(\x6)93.9489(\x5)99.0967(\x7)99.2789(\x8)91.3616(\x9)95.0775"
		"(\xa)98.1911(\x7)93.4046(\x7)99.2789(\x2)95.0782(\x3)94.9406"
		"(\x0b)100.133(\x9)95.0775(\xc)100.421(\x5)99.0953]" ) ;
	
	TokenSrc src( ss ) ;
	Array sub ;
	CPPUNIT_ASSERT( src >> sub ) ;
	
	PDFUT_ASSERT_EQUAL( sub.size(), 38U ) ;
	PDFUT_ASSERT_EQUAL( sub[2], "\x9" ) ;
	PDFUT_ASSERT_EQUAL( sub[3], 95.0775 ) ;
}

void ArrayTest::TestBracketTJ( )
{
	std::istringstream ss(
		"[(the line siz)10.8(e)-0.1( is 128 by)12.5(tes "
		"\(sub-)19.4(div)12.5(i)-3(ded or )]TJ" ) ;
	TokenSrc src( ss ) ;
	Array sub ;
	CPPUNIT_ASSERT( src >> sub ) ;
	
	std::cout << sub ;
}


} // end of namespace
