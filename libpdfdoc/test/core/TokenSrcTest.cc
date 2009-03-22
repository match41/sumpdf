/***************************************************************************\
 *   Copyright (C) 2009 by Nestal Wan                                      *
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
\***************************************************************************/

/*!
	\file	TokenSrcTest.cc
	\brief	implementation the TokenSrcTest class
	\date	Sun Mar 22 2009
	\author	Nestal Wan
*/

#include "TokenSrcTest.hh"

#include "core/TokenSrc.hh"

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
	CPPUNIT_ASSERT( t.Get() == "Hello" ) ;
	CPPUNIT_ASSERT( !subject.HasCache( ) ) ;
	
	subject.PutBack( t ) ;
	CPPUNIT_ASSERT( subject.HasCache( ) ) ;
	CPPUNIT_ASSERT( subject.ReadToken( t ) ) ;
	CPPUNIT_ASSERT( t.Get() == "Hello" ) ;
	CPPUNIT_ASSERT( !subject.HasCache( ) ) ;

	CPPUNIT_ASSERT( subject.ReadToken( t ) ) ;
	CPPUNIT_ASSERT( t.Get() == "world" ) ;
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
	CPPUNIT_ASSERT( ch == 'h' ) ;

	pdf::Token t ;
	CPPUNIT_ASSERT( subject >> t ) ;
	CPPUNIT_ASSERT( t.Get() == "ello" ) ;
	
	subject.PutBack( t ) ;
	char ello[4] ;
	CPPUNIT_ASSERT( subject.GetChar( ello[0] ) ) ;
	CPPUNIT_ASSERT( subject.GetChar( ello[1] ) ) ;
	CPPUNIT_ASSERT( subject.GetChar( ello[2] ) ) ;
	CPPUNIT_ASSERT( subject.GetChar( ello[3] ) ) ;
	CPPUNIT_ASSERT( ello == std::string("ello") ) ;

	pdf::Token t2 ;
	CPPUNIT_ASSERT( subject >> t2 ) ;
	CPPUNIT_ASSERT( t2.Get() == "world" ) ;
	
	CPPUNIT_ASSERT( !subject.HasCache() ) ;
}
