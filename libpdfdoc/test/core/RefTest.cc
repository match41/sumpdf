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
	\file	RefTest.cc
	\brief	implementation the RefTest class
	\date	Sun Mar 9 2008
	\author	Nestal Wan
*/

#include "RefTest.hh"

#include "core/Ref.hh"
#include "util/Util.hh"

#include "mock/Assert.hh"

#include <sstream>

RefTest::RefTest( )
{
}

void RefTest::TestNormal( )
{
	std::istringstream ss( "1 9 R" ) ;
	pdf::Ref obj ;
	CPPUNIT_ASSERT( ss >> obj ) ;
	PDFUT_ASSERT_EQUAL( obj.ID( ), 1U ) ;
	PDFUT_ASSERT_EQUAL( obj.Gen( ), 9U ) ;
}

void RefTest::TestError( )
{
	std::istringstream ss( "1 0 a a R" ) ;
	pdf::Ref obj ;
	CPPUNIT_ASSERT( !(ss >> obj) ) ;
	PDFUT_ASSERT_EQUAL( obj.ID( ), 0U ) ;
	PDFUT_ASSERT_EQUAL( obj.Gen( ), 0U ) ;
}

void RefTest::TestNonIntError( )
{
	std::istringstream src( "82/R" ) ;
	
	pdf::Ref obj ;
	CPPUNIT_ASSERT( !(src >> obj) ) ;
	PDFUT_ASSERT_EQUAL( obj.ID( ), 0U ) ;
	PDFUT_ASSERT_EQUAL( obj.Gen( ), 0U ) ;
}

void RefTest::TestTooFewToken( )
{
	std::istringstream src( "a" ) ;
	
	pdf::Ref obj ;
	CPPUNIT_ASSERT( !(src >> obj) ) ;
	PDFUT_ASSERT_EQUAL( obj.ID( ), 0U ) ;
	PDFUT_ASSERT_EQUAL( obj.Gen( ), 0U ) ;
}
