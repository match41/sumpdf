/***************************************************************************\
 *   Copyright (C) 2009 by Nestal Wan                                      *
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

/*!
	\file	TokenSrcTest.cc
	\brief	implementation the TokenSrcTest class
	\date	Sun Mar 22 2009
	\author	Nestal Wan
*/

#include "TokenSrcTest.hh"

#include "core/TokenSrc.hh"
#include "util/Util.hh"

#include "mock/Assert.hh"

#include <sstream>
#include <iostream>

TokenSrcTest::TokenSrcTest( )
{
}

void TokenSrcTest::TestReadToken( )
{
	std::istringstream ss( "Hello world" ) ;
	pdf::TokenSrc subject( ss ) ;
	
	pdf::Token t ;
	CPPUNIT_ASSERT( subject.ReadToken( t ) ) ;
	PDFUT_ASSERT_EQUAL( t.Get(), "Hello" ) ;
	CPPUNIT_ASSERT( !subject.HasCache( ) ) ;
	
	subject.PutBack( t ) ;
	CPPUNIT_ASSERT( subject.HasCache( ) ) ;
	CPPUNIT_ASSERT( subject.ReadToken( t ) ) ;
	PDFUT_ASSERT_EQUAL( t.Get(), "Hello" ) ;
	CPPUNIT_ASSERT( !subject.HasCache( ) ) ;

	CPPUNIT_ASSERT( subject.ReadToken( t ) ) ;
	PDFUT_ASSERT_EQUAL( t.Get(), "world" ) ;
	CPPUNIT_ASSERT( !subject.HasCache( ) ) ;
}

void TokenSrcTest::TestGetChar( )
{
	std::istringstream ss ;
	pdf::TokenSrc subject( ss ) ;
	subject.PutBack( pdf::Token("world") ) ;
	subject.PutBack( pdf::Token("hello") ) ;
	
	char ch ;
	CPPUNIT_ASSERT( subject.GetChar( ch ) ) ;
	PDFUT_ASSERT_EQUAL( ch, 'h' ) ;

	pdf::Token t ;
	CPPUNIT_ASSERT( subject >> t ) ;
	PDFUT_ASSERT_EQUAL( t.Get(), "ello" ) ;
	
	subject.PutBack( t ) ;
	char ello[4] ;
	CPPUNIT_ASSERT( subject.GetChar( ello[0] ) ) ;
	CPPUNIT_ASSERT( subject.GetChar( ello[1] ) ) ;
	CPPUNIT_ASSERT( subject.GetChar( ello[2] ) ) ;
	CPPUNIT_ASSERT( subject.GetChar( ello[3] ) ) ;
	PDFUT_ASSERT_EQUAL( std::string( ello, pdf::Count(ello) ), "ello" ) ;

	pdf::Token t2 ;
	CPPUNIT_ASSERT( subject >> t2 ) ;
	PDFUT_ASSERT_EQUAL( t2.Get(), "world" ) ;
	
	CPPUNIT_ASSERT( !subject.HasCache() ) ;
}

void TokenSrcTest::TestPeekAllCache( )
{
	std::istringstream ss ;
	pdf::TokenSrc subject( ss ) ;

	subject.PutBack( pdf::Token("world") ) ;
	subject.PutBack( pdf::Token("hello") ) ;
	
	pdf::Token t[2] ;
	CPPUNIT_ASSERT( subject.Peek( t, pdf::Count(t) ) == pdf::End(t) ) ;
	PDFUT_ASSERT_EQUAL( t[0].Get(), "hello" ) ;
	PDFUT_ASSERT_EQUAL( t[1].Get(), "world" ) ;
	
	pdf::Token a[2] ;
	CPPUNIT_ASSERT( subject >> a[0] >> a[1] ) ;
	PDFUT_ASSERT_EQUAL( a[0].Get(), "hello" ) ;
	PDFUT_ASSERT_EQUAL( a[1].Get(), "world" ) ;
}

void TokenSrcTest::TestPeekWithRead( )
{
	std::istringstream ss( "hello world foo bar" ) ;
	pdf::TokenSrc subject( ss ) ;

	pdf::Token hello, space, world ;
	CPPUNIT_ASSERT( ss >> hello >> space >> world ) ;
	CPPUNIT_ASSERT( subject ) ;
	PDFUT_ASSERT_EQUAL( hello.Get(), "hello" ) ;
	PDFUT_ASSERT_EQUAL( world.Get(), "world" ) ;

	subject.PutBack( world ) ;
	subject.PutBack( hello ) ;
	
	pdf::Token t[4] ;
	CPPUNIT_ASSERT( subject.Peek( t, pdf::Count(t) ) == pdf::End(t) ) ;
	CPPUNIT_ASSERT( subject ) ;
	PDFUT_ASSERT_EQUAL( t[0].Get(), "hello" ) ;
	PDFUT_ASSERT_EQUAL( t[1].Get(), "world" ) ;
	PDFUT_ASSERT_EQUAL( t[2].Get(), "foo" ) ;
	PDFUT_ASSERT_EQUAL( t[3].Get(), "bar" ) ;

	pdf::Token a[4] ;
	CPPUNIT_ASSERT( subject >> a[0] >> a[1] >> a[2] >> a[3] ) ;
	PDFUT_ASSERT_EQUAL( a[0].Get(), "hello" ) ;
	PDFUT_ASSERT_EQUAL( a[1].Get(), "world" ) ;
	PDFUT_ASSERT_EQUAL( a[2].Get(), "foo" ) ;
	PDFUT_ASSERT_EQUAL( a[3].Get(), "bar" ) ;
}

void TokenSrcTest::TestIgnoreCache( )
{
	std::istringstream ss ;
	pdf::TokenSrc subject( ss ) ;

	subject.PutBack( pdf::Token("flower") ) ;
	subject.PutBack( pdf::Token("a") ) ;
	subject.PutBack( pdf::Token("am") ) ;
	subject.PutBack( pdf::Token("I") ) ;

	subject.Ignore( 2 ) ;
	
	pdf::Token t[2] ;
	CPPUNIT_ASSERT( subject.Peek( t, pdf::Count(t) ) == pdf::End(t) ) ;
	CPPUNIT_ASSERT( t[0].Get() == "a" ) ;
	CPPUNIT_ASSERT( t[1].Get() == "flower" ) ;
}

void TokenSrcTest::TestIgnoreRead( )
{
	std::istringstream ss( "is a pig" ) ;
	pdf::TokenSrc subject( ss ) ;
	subject.PutBack( pdf::Token( "there" ) ) ;

	subject.Ignore( 3 ) ;
	pdf::Token t ;
	CPPUNIT_ASSERT( subject >> t ) ;	
	CPPUNIT_ASSERT( t.Get() == "pig" ) ;
}

void TokenSrcTest::TestSpaceInString( )
{
	std::istringstream ss( "(\t)" ) ;
	pdf::TokenSrc subject( ss ) ;

	pdf::Token t ;
	CPPUNIT_ASSERT( ss >> t ) ;
	PDFUT_ASSERT_EQUAL( t.Get(), "(\t)" ) ;
}
