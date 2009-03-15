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
	\file	TokenTest.hh
	\brief	definition the TokenTest class
	\date	Sun Mar 9 2008
	\author	Nestal Wan
*/

#ifndef __PDFUT_TOKEN_TEST_HEADER_INCLUDED__
#define __PDFUT_TOKEN_TEST_HEADER_INCLUDED__

#include <cppunit/TestFixture.h>

#include <cppunit/extensions/HelperMacros.h>

namespace CppUnit
{
	class Test ;
}

class TokenTest : public CppUnit::TestFixture
{
public :
	TokenTest( ) ;

	// declare suit function
	CPPUNIT_TEST_SUITE( TokenTest );
		CPPUNIT_TEST( TestAllNumbers );
		CPPUNIT_TEST( TestAllDelimitor );
		CPPUNIT_TEST( TestMixChar );
	CPPUNIT_TEST_SUITE_END();

private :
	void TestAllNumbers( ) ;
	void TestAllDelimitor( ) ;
	void TestMixChar( ) ;
} ;

#endif
