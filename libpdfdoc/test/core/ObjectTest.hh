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
	\file	ObjectTest.hh
	\brief	definition the ObjectTest class
	\date	Sun Mar 9 2008
	\author	Nestal Wan
*/

#ifndef __PDFUT_OBJECT_TEST_HEADER_INCLUDED__
#define __PDFUT_OBJECT_TEST_HEADER_INCLUDED__

#include <cppunit/TestFixture.h>

#include <cppunit/extensions/HelperMacros.h>

namespace CppUnit
{
	class Test ;
}

class ObjectTest : public CppUnit::TestFixture
{
public :
	ObjectTest( ) ;

	// declare suit function
	CPPUNIT_TEST_SUITE( ObjectTest ) ;
		CPPUNIT_TEST( TestName ) ;
		CPPUNIT_TEST( TestBoolTrue ) ;
		CPPUNIT_TEST( TestBoolFalse ) ;
		CPPUNIT_TEST( TestString ) ;
		CPPUNIT_TEST( TestInt ) ;
		CPPUNIT_TEST( TestDouble ) ;
		CPPUNIT_TEST( TestIndirect ) ;
		CPPUNIT_TEST( TestArray ) ;
		CPPUNIT_TEST( TestVisit ) ;
		CPPUNIT_TEST( TestObj5 ) ;
		CPPUNIT_TEST( TestObj9020 ) ;
		CPPUNIT_TEST( TestConvertToLong ) ;
		CPPUNIT_TEST( TestConvertToFloat ) ;
		CPPUNIT_TEST( TestIsNull ) ;
	CPPUNIT_TEST_SUITE_END( ) ;

private :
	void TestName( ) ;
	void TestString( ) ;
	void TestBoolTrue( ) ;
	void TestBoolFalse( ) ;
	void TestInt( ) ;
	void TestDouble( ) ;
	void TestIndirect( ) ;
	void TestArray( ) ;
	void TestVisit( ) ;
	void TestObj5( ) ;
	void TestObj9020( ) ;
	void TestConvertToLong( ) ;
	void TestConvertToFloat( ) ;
	void TestIsNull( ) ;
} ;

#endif
