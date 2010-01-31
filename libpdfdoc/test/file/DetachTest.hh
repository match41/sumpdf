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

/**	\file	DetachTest.hh
    \brief	definition the DetachTest class
    \date	Jan 31, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_DETACHTEST_HH_EADER_INCLUDED__
#define __PDF_DETACHTEST_HH_EADER_INCLUDED__

#include <cppunit/TestFixture.h>

#include <cppunit/extensions/HelperMacros.h>

namespace pdfut {

///	brief description
/**	The DetachTest class represents
*/
class DetachTest : public CppUnit::TestFixture
{
public :
	DetachTest( ) ;
	
	// declare suit function
	CPPUNIT_TEST_SUITE( DetachTest ) ;
		CPPUNIT_TEST( TestDetach ) ;
	CPPUNIT_TEST_SUITE_END();

} ;

} // end of namespace

#endif // DETACHTEST_HH_
