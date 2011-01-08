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

/**	\file	ColorSpaceTest.hh
    \brief	definition the ColorSpaceTest class
    \date	May 22, 2010
    \author	Nestal Wan
*/

#ifndef __PDFUT_COLORSPACETEST_HEADER_INCLUDED__
#define __PDFUT_COLORSPACETEST_HEADER_INCLUDED__

#include <cppunit/TestFixture.h>

#include <cppunit/extensions/HelperMacros.h>

namespace pdfut {

class RealColorSpaceTest : public CppUnit::TestFixture
{
public :
	RealColorSpaceTest( ) ;

	// declare suit function
	CPPUNIT_TEST_SUITE( RealColorSpaceTest ) ;
		CPPUNIT_TEST( Test ) ;
		CPPUNIT_TEST( TestICCAlternate ) ;
		CPPUNIT_TEST( TestRecursive ) ;
		CPPUNIT_TEST( TestWrite ) ;
	CPPUNIT_TEST_SUITE_END();

public :
	void setUp( ) ;
	void tearDown( ) ;

private :
	void Test( ) ;
	void TestRecursive( ) ;
	void TestICCAlternate( ) ;
	void TestWrite( ) ;
} ;

} // end of namespace

#endif // COLORSPACETEST_HH_
