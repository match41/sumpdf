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
	\file	StringTest.hh
	\brief	definition the StringTest class
	\date	Sun Mar 9 2008
	\author	Nestal Wan
*/

#ifndef __PDF_STRING_TEST_HEADER_INCLUDED__
#define __PDF_STRING_TEST_HEADER_INCLUDED__

#include <cppunit/TestFixture.h>

#include <cppunit/extensions/HelperMacros.h>

namespace pdfut {

class StringTest : public CppUnit::TestFixture
{
public :
	StringTest( ) ;

	// declare suit function
	CPPUNIT_TEST_SUITE( StringTest );
		CPPUNIT_TEST( TestLiteral );
		CPPUNIT_TEST( TestLiteralWithBrackets );
		CPPUNIT_TEST( TestLiteralWithEscape );
		CPPUNIT_TEST( TestErrorLiteral );
		CPPUNIT_TEST( TestHex );
		CPPUNIT_TEST( TestErrorHex );
		CPPUNIT_TEST( TestOctal );
		CPPUNIT_TEST( TestOctal1 );
		CPPUNIT_TEST( TestOctal2 );
		CPPUNIT_TEST( TestSpace );
		CPPUNIT_TEST( TestVTab );
		CPPUNIT_TEST( TestSpacesMix );
		CPPUNIT_TEST( TestBrackets );
	CPPUNIT_TEST_SUITE_END();

private :
	void TestLiteral( ) ;
	void TestLiteralWithBrackets( ) ;
	void TestLiteralWithEscape( ) ;
	void TestErrorLiteral( ) ;
	void TestHex( ) ;
	void TestErrorHex( ) ;
	void TestOctal( ) ;
	void TestOctal1( ) ;
	void TestOctal2( ) ;
	void TestSpace( ) ;
	void TestVTab( ) ;
	void TestSpacesMix( ) ;
	void TestBrackets( ) ;
} ;

} // end of namespace

#endif
