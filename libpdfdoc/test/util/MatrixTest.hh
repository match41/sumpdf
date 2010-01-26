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

/**	\file	MatrixTest.hh
    \brief	definition the MatrixTest class
    \date	Jan 26, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_MATRIXTEST_HH_EADER_INCLUDED__
#define __PDF_MATRIXTEST_HH_EADER_INCLUDED__

#include <cppunit/TestFixture.h>

#include <cppunit/extensions/HelperMacros.h>

///	brief description
/**	The MatrixTest class represents
*/
class MatrixTest : public CppUnit::TestFixture
{
public :
	MatrixTest( ) ;

	// declare suit function
	CPPUNIT_TEST_SUITE( MatrixTest ) ;
		CPPUNIT_TEST( TestMul ) ;
		CPPUNIT_TEST( TestDet ) ;
		CPPUNIT_TEST( TestInv ) ;
	CPPUNIT_TEST_SUITE_END( ) ;

private :
	void TestDet( ) ;
	void TestInv( ) ;
	void TestMul( ) ;
} ;

#endif // MATRIXTEST_HH_
