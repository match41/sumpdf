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
	\file	ObjectTest.cc
	\brief	implementation the ObjectTest class
	\date	Sun Mar 9 2008
	\author	Nestal Wan
*/

#include "ObjectTest.hh"

#include "core/Object.hh"
#include "core/TraverseObject.hh"
#include "core/Array.hh"
#include "core/Token.hh"

#include "stream/Stream.hh"

#include "util/CArray.hh"

#include "mock/Assert.hh"

#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

#include <algorithm>
#include <iterator>
#include <sstream>
#include <vector>
#include <iostream>

namespace pdfut {

using namespace pdf ;

ObjectTest::ObjectTest( )
{
}

void ObjectTest::TestName( )
{
	std::istringstream ss( "/Name" ) ;
	pdf::Object name ;
	CPPUNIT_ASSERT( ss >> name ) ;
	CPPUNIT_ASSERT( name.Type() == pdf::Object::name ) ;
	CPPUNIT_ASSERT( name.Is<pdf::Name>() ) ;
	CPPUNIT_ASSERT( name.TypeID() == typeid(pdf::Name) ) ;
	PDFUT_ASSERT_EQUAL( name.As<pdf::Name>( ), pdf::Name( "Name" ) ) ;
}

void ObjectTest::TestBoolTrue( )
{
	std::istringstream ss( "true" ) ;
	pdf::Object b ;
	CPPUNIT_ASSERT( ss >> b ) ;
	CPPUNIT_ASSERT( b.Type() == pdf::Object::boolean ) ;
	CPPUNIT_ASSERT( b.Is<bool>() ) ;
	CPPUNIT_ASSERT( b.TypeID() == typeid(bool) ) ;
	CPPUNIT_ASSERT( b.As<bool>( ) ) ;
}

void ObjectTest::TestBoolFalse( )
{
	std::istringstream ss( "false" ) ;
	pdf::Object b ;
	CPPUNIT_ASSERT( ss >> b ) ;
	CPPUNIT_ASSERT( !b.Is<void>( ) ) ;
	PDFUT_ASSERT_EQUAL( b.Type(), pdf::Object::boolean ) ;
	CPPUNIT_ASSERT( b.TypeID() == typeid(bool) ) ;
	CPPUNIT_ASSERT( !b.As<bool>( ) ) ;
}

void ObjectTest::TestString( )
{
	std::istringstream ss( "(123 () 456)" ) ;
	pdf::Object b ;
	CPPUNIT_ASSERT( ss >> b ) ;
	CPPUNIT_ASSERT( !b.Is<void>( ) ) ;
	PDFUT_ASSERT_EQUAL( b.Type(), pdf::Object::string ) ;
	CPPUNIT_ASSERT( b.TypeID() == typeid(std::string) ) ;
	PDFUT_ASSERT_EQUAL( b.As<std::string>( ), "123 () 456" ) ;
}

void ObjectTest::TestInt( )
{
	std::istringstream ss( "1000" ) ;
	pdf::Object b ;
	CPPUNIT_ASSERT( ss >> b ) ;
	CPPUNIT_ASSERT( !b.Is<void>( ) ) ;
	PDFUT_ASSERT_EQUAL( b.Type(), pdf::Object::integer ) ;
	CPPUNIT_ASSERT( b.TypeID() == typeid(int) ) ;
	CPPUNIT_ASSERT( b.Is<int>() ) ;
	PDFUT_ASSERT_EQUAL( b.As<int>( ), 1000 ) ;
}

void ObjectTest::TestDouble( )
{
	std::istringstream ss( "10.00" ) ;
	pdf::Object b ;
	CPPUNIT_ASSERT( ss >> b ) ;
	CPPUNIT_ASSERT( !b.Is<void>( ) ) ;
	CPPUNIT_ASSERT( b.Type() == pdf::Object::floating ) ;
	CPPUNIT_ASSERT( b.TypeID() == typeid(double) ) ;
	PDFUT_ASSERT_EQUAL( b.As<double>( ), 10.0 ) ;
}

void ObjectTest::TestDoubleRoundTrip( )
{
	std::stringstream ss ;
	pdf::Object b( 10.0 ) ;
	CPPUNIT_ASSERT( ss << b ) ;
	pdf::Object c ;
	CPPUNIT_ASSERT( ss >> c ) ;
	CPPUNIT_ASSERT( c.Is<double>() ) ;
	PDFUT_ASSERT_EQUAL( c, b ) ;
}

void ObjectTest::TestIndirect( )
{
}

void ObjectTest::TestArray( )
{
	pdf::Ref refs[] =
	{
		pdf::Ref( 1, 0 ),
		pdf::Ref( 2, 0 ),
		pdf::Ref( 3, 0 ),
		pdf::Ref( 4, 0 ),
	} ;
	pdf::Object b( pdf::Array( pdf::Begin(refs), pdf::End(refs) ) ), b2 ;
	CPPUNIT_ASSERT( !b.Is<void>( ) ) ;
	CPPUNIT_ASSERT( b.TypeID() == typeid(pdf::Array) ) ;
	CPPUNIT_ASSERT( b2.Is<void>() ) ;

	std::stringstream ss ;
	CPPUNIT_ASSERT( ss << b ) ;
	CPPUNIT_ASSERT( ss >> b2 ) ;
	
	CPPUNIT_ASSERT( !b2.Is<void>( ) ) ;
	PDFUT_ASSERT_EQUAL( b, b2 ) ;
}

namespace
{
	struct Visitor : boost::static_visitor<>
	{
		int counter ;
		Visitor( ) : counter( 0 )
		{
		}
		
		template <typename T>
		void operator()( const T& t )
		{
		}
		
		void operator()( const pdf::Ref& r )
		{
			counter++ ;
		}
	} ;
}

void ObjectTest::TestObj5( )
{
	const char file[] = "<</StemV 79/FontName/PAPHHN+Minion"
	                    "Pro-Regular/FontFile3 4 0 R/Flags 4/Descent -258"
	                    "/FontBBox[-290 -360 1684 989]/Ascent 717/CapHeig"
	                    "ht 679/XHeight 440/Type/FontDescriptor/ItalicAng"
	                    "le 0/StemH 32/CharSet(/space/copyright/one/nine/"
	                    "eight/five/endash/two/zero/six/A/d/o/b/e/S/y/s/t"
	                    "/m/I/n/c/r/p/a/period/l/i/g/h/v/P/D/F/R/f/comma/"
	                    "x/colon/u/seven/N/O/T/C/E/w/hyphen/j/k/z/q/G/M/X"
	                    "/U/slash/W/B/V/semicolon/three/K/H/L/Z/ampersand"
	                    "/quotedblleft/quotedblright/underscor\\\ne)>>" ;
	std::istringstream is( file ) ;
	pdf::Object sub ;
	CPPUNIT_ASSERT( is >> sub ) ;
	CPPUNIT_ASSERT( !sub.Is<void>( ) ) ;
	
	const pdf::Dictionary& dict = sub.As<pdf::Dictionary>() ;
	const std::string& a = dict[pdf::Name("CharSet")] ;

	PDFUT_ASSERT_EQUAL( a[a.size()-2], '\n' ) ;
	PDFUT_ASSERT_EQUAL( a[a.size()-1], 'e' ) ;
}

void ObjectTest::TestObj9020( )
{
	const std::string filename = std::string(TEST_DATA_DIR) + "obj9020" ;
	std::ifstream testdata( filename.c_str( ), std::ios::in |
	                                           std::ios::binary ) ;
	CPPUNIT_ASSERT( testdata ) ;
	
	pdf::Object obj ;
	CPPUNIT_ASSERT( testdata >> obj ) ;
	CPPUNIT_ASSERT( !obj.Is<void>( ) ) ;
}

void ObjectTest::TestConvertToLong( )
{
	pdf::Object obj( 100 ) ;
	long l = obj ;
	CPPUNIT_ASSERT( !obj.Is<void>( ) ) ;
	PDFUT_ASSERT_EQUAL( l, 100 ) ;
}

void ObjectTest::TestConvertToFloat( )
{
	pdf::Object obj( 10.01f ) ;
	float f = obj ;
	CPPUNIT_ASSERT( !obj.Is<void>( ) ) ;
	PDFUT_ASSERT_EQUAL( f, 10.01f ) ;
}

void ObjectTest::TestIsVoid( )
{
	pdf::Object obj ;
	CPPUNIT_ASSERT( obj.Is<void>() ) ;
}

void ObjectTest::TestConvert( )
{
	pdf::Object obj( 100 ) ;
	unsigned ui = obj ;
	PDFUT_ASSERT_EQUAL( ui, 100U ) ;
	
	long li = obj ;
	PDFUT_ASSERT_EQUAL( li, 100 ) ;
}

void ObjectTest::TestVTab( )
{
	std::istringstream src( "(\x09)" ) ;

	pdf::Object str ;
	CPPUNIT_ASSERT( src >> str ) ;
	PDFUT_ASSERT_EQUAL( str.As<std::string>(), "\x09" ) ;
}

void ObjectTest::TestF( )
{
	std::istringstream src( "f\n" ) ;
	
	pdf::Object str ;
	CPPUNIT_ASSERT( src >> str ) ;
	CPPUNIT_ASSERT( str.Is<Token>() ) ;
	PDFUT_ASSERT_EQUAL( str.As<Token>(), Token("f") ) ;
}

} // end of namespace
