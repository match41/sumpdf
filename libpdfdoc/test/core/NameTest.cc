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
	\file	NameTest.cc
	\brief	implementation the NameTest class
	\date	Sun Mar 9 2008
	\author	Nestal Wan
*/

#include "NameTest.hh"

#include "core/Object.hh"

#include "mock/Assert.hh" 

#include <algorithm>
#include <iterator>
#include <sstream>
#include <vector>

namespace pdfut {

using namespace pdf ;

NameTest::NameTest( )
{
}

void NameTest::TestOneName( )
{
	std::istringstream ss( "/Hello" ) ;
	pdf::Name name ;
	CPPUNIT_ASSERT( ss >> name ) ;
	PDFUT_ASSERT_EQUAL( name, pdf::Name( "Hello" ) ) ;
}

void NameTest::TestTwoNames( )
{
	std::istringstream ss( "/Hello/World" ) ;
	pdf::Name name, name2 ;
	CPPUNIT_ASSERT( ss >> name >> name2 ) ;
	PDFUT_ASSERT_EQUAL( name, pdf::Name( "Hello" ) ) ;
	PDFUT_ASSERT_EQUAL( name2, pdf::Name( "World" ) ) ;
}

void NameTest::TestNonName( )
{
	std::istringstream ss( "1234" ) ;
	pdf::Name name ;
	CPPUNIT_ASSERT( !(ss >> name) ) ;
	PDFUT_ASSERT_EQUAL( name, pdf::Name( ) ) ;
}

} // end of namespace
