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
	\file	JpegImageTest.hh
	\brief	definition the JpegImageTest class
	\date	Sat Feb 28 2009
	\author	Nestal Wan
*/

#ifndef __IMGUT_JPEG_IMAGE_TEST_HEADER_INCLUDED__
#define __IMGUT_JPEG_IMAGE_TEST_HEADER_INCLUDED__

#include <cppunit/TestFixture.h>

#include <cppunit/extensions/HelperMacros.h>

/*!	\brief	brief description
	
	this class represents
*/
class JpegImageTest : public CppUnit::TestFixture
{
public :
	JpegImageTest( ) ;

	// declare suit function
	CPPUNIT_TEST_SUITE( JpegImageTest ) ;
		CPPUNIT_TEST( TestWidthHeight ) ;
		CPPUNIT_TEST( TestRead );
	CPPUNIT_TEST_SUITE_END( ) ;

private :
	void TestWidthHeight( ) ;
	void TestRead( ) ;
} ;

#endif
