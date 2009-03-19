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
	\file	TrueTypeTableTest.hh
	\brief	definition the TrueTypeTableTest class
	\date	Sat Mar 7 2009
	\author	Nestal Wan
*/

#ifndef __PDFUT_BYTE_STREAM_TEST_HEADER_INCLUDED__
#define __PDFUT_BYTE_STREAM_TEST_HEADER_INCLUDED__

#include <cppunit/TestFixture.h>

#include <cppunit/extensions/HelperMacros.h>

/*!	\brief	brief description
	
	this class represents
*/
class TrueTypeTableTest : public CppUnit::TestFixture
{
public :
	TrueTypeTableTest( ) ;
	
	// declare suit function
	CPPUNIT_TEST_SUITE( TrueTypeTableTest );
		CPPUNIT_TEST( TestWrite );
		CPPUNIT_TEST( TestRead );
		CPPUNIT_TEST( TestChecksum );
	CPPUNIT_TEST_SUITE_END();

private :
	void TestWrite( ) ;
	void TestRead( ) ;
	void TestChecksum( ) ;
} ;

#endif
