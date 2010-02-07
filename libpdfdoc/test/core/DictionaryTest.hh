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
	\file	DictionaryTest.hh
	\brief	definition the DictionaryTest class
	\date	Tue Mar 11 2008
	\author	Nestal Wan
*/

#ifndef __PDFUT_DICTIONARY_TEST_HEADER_INCLUDED__
#define __PDFUT_DICTIONARY_TEST_HEADER_INCLUDED__

#include <cppunit/TestFixture.h>

#include <cppunit/extensions/HelperMacros.h>

class DictionaryTest : public CppUnit::TestFixture
{
public :
	DictionaryTest( ) ;

	// declare suit function
	CPPUNIT_TEST_SUITE( DictionaryTest ) ;
		CPPUNIT_TEST( TestNormal ) ;
		CPPUNIT_TEST( TestRead ) ;
		CPPUNIT_TEST( TestTrailer ) ;
		CPPUNIT_TEST( TestDoubleEnd ) ;
		CPPUNIT_TEST( TestImage ) ;
		CPPUNIT_TEST( TestSwap ) ;
		CPPUNIT_TEST( TestNull ) ;
		CPPUNIT_TEST( TestErase ) ;
		CPPUNIT_TEST( TestEraseByAddNull ) ;
	CPPUNIT_TEST_SUITE_END( ) ;

private :
	void TestNormal( ) ;
	void TestRead( ) ;
	void TestTrailer( ) ;
	void TestDoubleEnd( ) ;
	void TestImage( ) ;
	void TestSwap( ) ;
	void TestNull( ) ;
	void TestErase( ) ;
	void TestEraseByAddNull( ) ;
} ;

#endif
