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

/**	\file	CodeMapTest.cc
	\brief	implementation of the CodeMapTest class
	\date	Mar 21, 2010
	\author	Nestal Wan
*/

#include "CodeMapTest.hh"

#include "font/CodeMap.hh"

#include "mock/Assert.hh"

namespace pdfut {

using namespace pdf ;

CodeMapTest::CodeMapTest( )
{
}

void CodeMapTest::setUp( )
{
}

void CodeMapTest::tearDown( )
{
}

void CodeMapTest::Test( )
{
	wchar_t ch ;
	const char *name ;
	
	CPPUNIT_ASSERT( NameToUnicode( "bullet", ch ) ) ;
	PDFUT_ASSERT_EQUAL( ch, 8226 ) ;
	
	CPPUNIT_ASSERT( NameToUnicode( "three", ch ) ) ;
	PDFUT_ASSERT_EQUAL( ch, 0x33 ) ;
	
	CPPUNIT_ASSERT( UnicodeToName( 8226, name ) ) ;
	PDFUT_ASSERT_EQUAL( name, std::string("bullet") ) ;
}

} // end of namespace
