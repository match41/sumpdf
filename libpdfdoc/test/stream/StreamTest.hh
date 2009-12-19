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
	\file	StreamTest.hh
	\brief	definition the StreamTest class
	\date	Tue Apr 1 2008
	\author	Nestal Wan
*/

#ifndef __PDFUT_STREAM_TEST_HEADER_INCLUDED__
#define __PDFUT_STREAM_TEST_HEADER_INCLUDED__

#include <cppunit/TestFixture.h>

#include <cppunit/extensions/HelperMacros.h>

#include <vector>

namespace CppUnit
{
	class Test ;
}

class StreamTest : public CppUnit::TestFixture
{
public :
	StreamTest( ) ;

	// declare suit function
	CPPUNIT_TEST_SUITE( StreamTest ) ;
		CPPUNIT_TEST( TestRead ) ;
		CPPUNIT_TEST( TestRead2 ) ;
		CPPUNIT_TEST( TestReadDeflate ) ;
		CPPUNIT_TEST( TestWrite ) ;
		CPPUNIT_TEST( TestReset ) ;
		CPPUNIT_TEST( TestWriteOstream ) ;
		CPPUNIT_TEST( TestName ) ;
		CPPUNIT_TEST( TestClone ) ;
	CPPUNIT_TEST_SUITE_END( ) ;

public :
	void setUp( ) ;
	void tearDown( ) ;

private :
	void TestRead( ) ;
	void TestRead2( ) ;
	void TestWrite( ) ;
	void TestReset( ) ;
	void TestReadDeflate( ) ;
	void TestWriteOstream( ) ;
	void TestName( ) ;
	void TestClone( ) ;

private :
	std::vector<unsigned char> m_original, m_compressed ;
} ;

#endif
