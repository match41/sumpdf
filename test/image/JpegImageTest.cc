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
	\file	JpegImageTest.cc
	\brief	implementation the JpegImageTest class
	\date	Sat Feb 28 2009
	\author	Nestal Wan
*/

#include "JpegImageTest.hh"

#include "image/JpegImage.hh"

#include <iostream>
#include <algorithm>
#include <iterator>

JpegImageTest::JpegImageTest( )
{
}

void JpegImageTest::TestWidthHeight( )
{
	img::JpegImage subject( std::string(TEST_DATA_DIR) + "3x3bgr.jpg" ) ;
	CPPUNIT_ASSERT( subject.Width( )  == 3 ) ;
	CPPUNIT_ASSERT( subject.Height( ) == 3 ) ;
}

void JpegImageTest::TestRead( )
{
/*	img::PngImage subject( std::string(TEST_DATA_DIR) + "3x3bgr.png" ) ;
	
	std::vector<unsigned char> v( subject.RowSize( ) ) ;
	
	// 1st row is blue
	subject.ReadRow( &v[0] ) ;
	unsigned char blue[] = { 0, 0, 255, 0, 0, 255, 0, 0, 255, } ;
	CPPUNIT_ASSERT( std::equal( v.begin( ), v.end( ), blue ) ) ;

	// 1st row is green
	subject.ReadRow( &v[0] ) ;
	unsigned char green[] = { 0, 255, 0, 0, 255, 0, 0, 255, 0, } ;
	CPPUNIT_ASSERT( std::equal( v.begin( ), v.end( ), green ) ) ;

	// 1st row is red
	subject.ReadRow( &v[0] ) ;
	unsigned char red[] = { 240, 3, 3, 240, 3, 3, 240, 3, 3, } ;
	CPPUNIT_ASSERT( std::equal( v.begin( ), v.end( ), red ) ) ;
*/
}

