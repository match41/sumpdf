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
	\file	StringTest.cc
	\brief	implementation the StringTest class
	\date	Sun Mar 9 2008
	\author	Nestal Wan
*/

#include "StringTest.hh"

#include "core/String.hh"

#include "mock/Assert.hh"

#include <sstream>
#include <vector>
#include <stdexcept>

StringTest::StringTest( )
{
}

void StringTest::TestLiteral( )
{
	std::istringstream ss( "(123456)" ) ;
	pdf::String str ;
	CPPUNIT_ASSERT( ss >> str ) ;
	PDF_ASSERT_EQUAL( str.Get(), "123456" ) ;
}

void StringTest::TestLiteralWithBrackets( )
{
	std::istringstream ss( "(123()4(56))" ) ;
	pdf::String str ;
	CPPUNIT_ASSERT( ss >> str ) ;
	PDF_ASSERT_EQUAL( str.Get(), "123()4(56)" ) ;
}

void StringTest::TestLiteralWithEscape( )
{
	std::istringstream ss( "(123(\\b\\f)4(56))" ) ;
	pdf::String str ;
	CPPUNIT_ASSERT( ss >> str ) ;
	PDF_ASSERT_EQUAL( str.Get(), "123(\b\f)4(56)" ) ;
}

void StringTest::TestErrorLiteral( )
{
	std::istringstream ss( "(12[}}CVDe,.3\\kkk\\f4(56))" ) ;
	ss.exceptions( ss.failbit ) ;
	
	pdf::String str ;
	CPPUNIT_ASSERT( ss >> str ) ;
	PDF_ASSERT_EQUAL( str.Get(), "12[}}CVDe,.3kk\f4(56)" ) ;
}

void StringTest::TestHex( )
{
	std::istringstream ss( "<6465666768696a6b>" ) ;
	pdf::String str ;
	CPPUNIT_ASSERT( ss >> str ) ;
	PDF_ASSERT_EQUAL( str.Get(), "defghijk" ) ;
}

void StringTest::TestErrorHex( )
{
	std::istringstream ss( "<type>" ) ;
	ss.exceptions( ss.failbit ) ;

	pdf::String str ;
	CPPUNIT_ASSERT_THROW( ss >> str, std::ios_base::failure ) ;
}
