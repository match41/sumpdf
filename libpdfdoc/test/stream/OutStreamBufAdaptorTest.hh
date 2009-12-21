/***************************************************************************\
 *   Copyright (C) 2009 by Nestal Wan                                      *
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
 \***************************************************************************/

/**
 \file	OutStreamBufAdaptorTest.hh
 \brief	definition the OutStreamBufAdaptorTest class
 \date	Dec 22, 2009
 \author	nestal
 */

#ifndef __PDF_OUTSTREAMBUFADAPTORTEST_HEADER_INCLUDED__
#define __PDF_OUTSTREAMBUFADAPTORTEST_HEADER_INCLUDED__

#include <cppunit/TestFixture.h>

#include <cppunit/extensions/HelperMacros.h>

class OutStreamBufAdaptorTest : public CppUnit::TestFixture
{
public:
	OutStreamBufAdaptorTest( ) ;

	// declare suit function
	CPPUNIT_TEST_SUITE( OutStreamBufAdaptorTest );
		CPPUNIT_TEST( TestWrite ) ;
	CPPUNIT_TEST_SUITE_END( ) ;

private :
	void TestWrite( ) ;
} ;

#endif // OUTSTREAMBUFADAPTORTEST_HH_
