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

/**
	\file	RealDocTest.hh
	\brief	definition the RealDocTest class
	\date	Dec 27, 2009
	\author	Nestal Wan
*/

#ifndef __PDF_REALDOCTEST_HH_EADER_INCLUDED__
#define __PDF_REALDOCTEST_HH_EADER_INCLUDED__

#include <cppunit/TestFixture.h>

#include <cppunit/extensions/HelperMacros.h>

namespace pdfut {

///	brief description
/**	this class represents
*/
class RealDocTest : public CppUnit::TestFixture
{
public:
	RealDocTest( ) ;

	// declare suit function
	CPPUNIT_TEST_SUITE( RealDocTest ) ;
		CPPUNIT_TEST( TestNew ) ;
		CPPUNIT_TEST( TestRead ) ;
		CPPUNIT_TEST( TestReadColour ) ;
	CPPUNIT_TEST_SUITE_END( ) ;

private :
	void TestNew( ) ;
	void TestRead( ) ;
	void TestReadColour( ) ;
} ;

} // end of namespace

#endif // REALDOCTEST_HH_
