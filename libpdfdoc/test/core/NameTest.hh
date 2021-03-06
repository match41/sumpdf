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
	\file	NameTest.hh
	\brief	definition the NameTest class
	\date	Sun Mar 9 2008
	\author	Nestal Wan
*/

#ifndef __PDFUT_NAME_TEST_HEADER_INCLUDED__
#define __PDFUT_NAME_TEST_HEADER_INCLUDED__

#include <cppunit/TestFixture.h>

#include <cppunit/extensions/HelperMacros.h>

namespace pdfut {

class NameTest : public CppUnit::TestFixture
{
public :
	NameTest( ) ;

	// declare suit function
	CPPUNIT_TEST_SUITE( NameTest );
		CPPUNIT_TEST( TestOneName );
		CPPUNIT_TEST( TestTwoNames );
		CPPUNIT_TEST( TestNonName );
	CPPUNIT_TEST_SUITE_END();

private :
	void TestOneName( ) ;
	void TestTwoNames( ) ;
	void TestNonName( ) ;
} ;

} // end of namespace

#endif
