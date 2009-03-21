/***************************************************************************
 *   Copyright (C) 2006 by Nestal Wan                                      *
 *   me@nestal.net                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
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

#include <algorithm>
#include <iterator>
#include <sstream>
#include <vector>

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
	CPPUNIT_ASSERT( sub[0] == 10 ) ;
	CPPUNIT_ASSERT( sub[1].As<pdf::Name>() == pdf::Name("Hello") ) ;
	CPPUNIT_ASSERT( sub[2] == 123.0 ) ;
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
