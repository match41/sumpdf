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
	\file	StreamTest.cc
	\brief	implementation the StreamTest class
	\date	Tue Apr 1 2008
	\author	Nestal Wan
*/

#include "StreamTest.hh"

#include "core/Dictionary.hh"
#include "core/Stream.hh"
#include "core/Token.hh"
#include "core/TokenSrc.hh"

#include <sstream>
#include <iostream>

StreamTest::StreamTest( )
{
}

void StreamTest::TestRead( )
{
	std::istringstream oss( "<<\n/Length 45\n>>\nstream\nBT\n"
	                          "/F0 12 Tf 100 100 Td (Hello world!) Tj\n"
	                          "ET\nendstream" ) ;
	pdf::TokenSrc src( oss ) ;
	pdf::Dictionary d ;
	CPPUNIT_ASSERT( src >> d ) ;
	
	pdf::Token objstr ;
	CPPUNIT_ASSERT( src >> objstr ) ;
	CPPUNIT_ASSERT( objstr.Get() == "stream" ) ;
}

void StreamTest::TestRead2( )
{
	pdf::Dictionary d ;
	d["Length"] = 5 ;
	std::istringstream iss( "hello" ) ;
	pdf::Stream subject( iss.rdbuf(), 0, d ) ;
	pdf::Token t ;
	CPPUNIT_ASSERT( subject.InStream() >> t ) ;
	CPPUNIT_ASSERT( t.Get() == "hello" ) ;
}

void StreamTest::TestWrite( )
{
	std::string str = "0 12 TD (string string) Tj" ;
/*	pdf::Stream stream( str ) ;
	
	std::stringstream ss ; 
	ss << stream ;

	pdf::Dictionary dict ;
	ss >> dict ;
	CPPUNIT_ASSERT( dict["Length"] == str.size( ) ) ;*/
}
