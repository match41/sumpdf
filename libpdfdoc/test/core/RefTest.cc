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
	\file	RefTest.cc
	\brief	implementation the RefTest class
	\date	Sun Mar 9 2008
	\author	Nestal Wan
*/

#include "RefTest.hh"

#include "core/Ref.hh"

#include <sstream>

RefTest::RefTest( )
{
}

void RefTest::TestNormal( )
{
	std::istringstream ss( "1 9 R" ) ;
	pdf::Ref obj ;
	CPPUNIT_ASSERT( ss >> obj ) ;
	CPPUNIT_ASSERT( obj.ID( ) == 1 ) ;
	CPPUNIT_ASSERT( obj.Generation( ) == 9 ) ;
}

void RefTest::TestError( )
{
	std::istringstream ss( "1 0 a a R" ) ;
	pdf::Ref obj ;
	CPPUNIT_ASSERT( !(ss >> obj) ) ;
	CPPUNIT_ASSERT( obj.ID( ) == 0 ) ;
	CPPUNIT_ASSERT( obj.Generation( ) == 0 ) ;
}

void RefTest::TestNonIntError( )
{
	std::istringstream ss( "82/R" ) ;
	pdf::Ref obj ;
	CPPUNIT_ASSERT( !(ss >> obj) ) ;
	CPPUNIT_ASSERT( obj.ID( ) == 0 ) ;
	CPPUNIT_ASSERT( obj.Generation( ) == 0 ) ;
}