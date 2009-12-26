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
	\file	RawFilterTest.cc
	\brief	implementation the RawFilterTest class
	\date	Mon Mar 9 2009
	\author	Nestal Wan
*/

#include "RawFilterTest.hh"

#include "core/Object.hh"
#include "stream/RawFilter.hh"

#include <sstream>

RawFilterTest::RawFilterTest( )
{
}

void RawFilterTest::TestRead( )
{
	std::istringstream ss( "hello world" ) ;
	pdf::RawFilter t( ss.rdbuf(), 0, 5 ) ;
	
	unsigned char buf[10] ;
	CPPUNIT_ASSERT( t.Read( buf, sizeof(buf) ) == 5 ) ;
	CPPUNIT_ASSERT( std::string(buf, buf+5) == "hello" ) ;
}

void RawFilterTest::TestWrite( )
{
	std::ostringstream ss ;
	pdf::RawFilter t( ss.rdbuf(), 0 ) ;
	
	unsigned char buf[] = "hello" ;
	CPPUNIT_ASSERT( t.Write( buf, sizeof(buf)-1 ) == 5 ) ;
	CPPUNIT_ASSERT( ss.str() == "hello" ) ;
}

void RawFilterTest::TestName( )
{
	std::ostringstream ss ;
	pdf::RawFilter t( ss.rdbuf(), 0 ) ;

	CPPUNIT_ASSERT( t.GetFilterName().IsNull() ) ;
}
