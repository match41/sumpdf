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

/**	\file	RealTextTest.hh
    \brief	definition the RealTextTest class
    \date	Jan 23, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_REALTEXTTEST_HH_EADER_INCLUDED__
#define __PDF_REALTEXTTEST_HH_EADER_INCLUDED__

#include "mock/TestBase.hh"

#include <cppunit/extensions/HelperMacros.h>

namespace pdfut {

///	brief description
/**	The RealTextTest class represents
*/
class RealTextTest : public TestBase
{
public :
	RealTextTest( ) ;

	// declare suit function
	CPPUNIT_TEST_SUITE( RealTextTest ) ;
		CPPUNIT_TEST( TestTdCmd ) ;
		CPPUNIT_TEST( TestTJ ) ;
	CPPUNIT_TEST_SUITE_END( ) ;

private :
	void TestTdCmd( ) ;
	void TestTJ( ) ;
} ;

#endif // REALTEXTTEST_HH_

} // end of namespace
