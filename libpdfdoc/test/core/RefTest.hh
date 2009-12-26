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
	\file	RefTest.hh
	\brief	definition the RefTest class
	\date	Sun Mar 9 2008
	\author	Nestal Wan
*/

#ifndef __PDFUT_REF_TEST_HEADER_INCLUDED__
#define __PDFUT_REF_TEST_HEADER_INCLUDED__

#include <cppunit/TestFixture.h>

#include <cppunit/extensions/HelperMacros.h>

namespace CppUnit
{
	class Test ;
}

class RefTest : public CppUnit::TestFixture
{
public :
	RefTest( ) ;

	// declare suit function
	CPPUNIT_TEST_SUITE( RefTest );
		CPPUNIT_TEST( TestNormal );
		CPPUNIT_TEST( TestError );
		CPPUNIT_TEST( TestNonIntError );
		CPPUNIT_TEST( TestTooFewToken );
	CPPUNIT_TEST_SUITE_END();

private :
	void TestNormal( ) ;
	void TestError( ) ;
	void TestNonIntError( ) ;
	void TestTooFewToken( ) ;
} ;

#endif
