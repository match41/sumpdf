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

/**	\file	DictReaderTest.hh
    \brief	definition the DictReaderTest class
    \date	Feb 16, 2010
    \author	Nestal Wan
*/

#ifndef __PDFUT_DICTREADERTEST_HH_EADER_INCLUDED__
#define __PDFUT_DICTREADERTEST_HH_EADER_INCLUDED__

#include <cppunit/TestFixture.h>

#include <cppunit/extensions/HelperMacros.h>

namespace pdfut {

class DictReaderTest : public CppUnit::TestFixture
{
public :
	DictReaderTest( ) ;

	// declare suit function
	CPPUNIT_TEST_SUITE( DictReaderTest ) ;
		CPPUNIT_TEST( TestIntToDouble ) ;
	CPPUNIT_TEST_SUITE_END();

public :
	void setUp( ) ;
	void tearDown( ) ;

private :
	void TestIntToDouble( ) ;
} ;

} // end of namespace

#endif // DICTREADERTEST_HH_
