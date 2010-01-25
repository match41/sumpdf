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

/**	\file	RealContentTest.hh
    \brief	definition the RealContentTest class
    \date	Jan 25, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_REALCONTENTTEST_HH_EADER_INCLUDED__
#define __PDF_REALCONTENTTEST_HH_EADER_INCLUDED__

#include "mock/TestBase.hh"

#include <cppunit/extensions/HelperMacros.h>

///	brief description
/**	The RealContentTest class represents
*/
class RealContentTest : public TestBase
{
public :
	RealContentTest( ) ;
	
	// declare suit function
	CPPUNIT_TEST_SUITE( RealContentTest ) ;
		CPPUNIT_TEST( TestWrite ) ;
	CPPUNIT_TEST_SUITE_END( ) ;

private :
	void TestWrite( ) ;
} ;

#endif // REALCONTENTTEST_HH_
