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
#include "core/TokenSrc.hh"
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
	PDF_ASSERT_EQUAL( obj.ID( ), 1U ) ;
	PDF_ASSERT_EQUAL( obj.Gen( ), 9U ) ;
}

void RefTest::TestError( )
{
	std::istringstream ss( "1 0 a a R" ) ;
	pdf::Ref obj ;
	CPPUNIT_ASSERT( !(ss >> obj) ) ;
	PDF_ASSERT_EQUAL( obj.ID( ), 0U ) ;
	PDF_ASSERT_EQUAL( obj.Gen( ), 0U ) ;
}

void RefTest::TestNonIntError( )
{
	std::istringstream ss( "82/R" ) ;
	pdf::TokenSrc src( ss ) ;
	
	pdf::Ref obj ;
	CPPUNIT_ASSERT( !(src >> obj) ) ;
	PDF_ASSERT_EQUAL( obj.ID( ), 0U ) ;
	PDF_ASSERT_EQUAL( obj.Gen( ), 0U ) ;
	
	// the 3 tokens can still be read
	pdf::Token t[3] ;
	src.ResetState( ) ;
	CPPUNIT_ASSERT( src.Peek( t, pdf::Count(t) ) == pdf::End(t) ) ;
	PDF_ASSERT_EQUAL( t[0].Get(), "82" ) ;
	PDF_ASSERT_EQUAL( t[1].Get(), "/" ) ;
	PDF_ASSERT_EQUAL( t[2].Get(), "R" ) ;

}

void RefTest::TestTooFewToken( )
{
	std::istringstream ss( "a" ) ;
	pdf::TokenSrc src( ss ) ;
	src.PutBack( pdf::Token( "2" ) ) ;
	
	pdf::Ref obj ;
	CPPUNIT_ASSERT( !(src >> obj) ) ;
	PDF_ASSERT_EQUAL( obj.ID( ), 0U ) ;
	PDF_ASSERT_EQUAL( obj.Gen( ), 0U ) ;

	// the 2 tokens can still be read
	pdf::Token t[2] ;
	src.ResetState( ) ;
	PDF_ASSERT_EQUAL( src.Peek( t, pdf::Count(t) ), pdf::End(t) ) ;
	PDF_ASSERT_EQUAL( t[0].Get(), "2" ) ;
	PDF_ASSERT_EQUAL( t[1].Get(), "a" ) ;
}
