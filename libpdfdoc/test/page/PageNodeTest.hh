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
	\file	PageNodeTest.hh
	\brief	definition the PageNodeTest class
	\date	Wed Oct 15 2008
	\author	Nestal Wan
*/

#ifndef __PDFUT_PAGE_NODE_TEST_HEADER_INCLUDED__
#define __PDFUT_PAGE_NODE_TEST_HEADER_INCLUDED__

#include <cppunit/TestFixture.h>

#include <cppunit/extensions/HelperMacros.h>

// freetype headers
#include <ft2build.h>
#include FT_FREETYPE_H

namespace pdf
{
	class PageTree ;
}

/*!	\brief	brief description
	
	this class represents
*/
class PageNodeTest : public CppUnit::TestFixture
{
public :
	PageNodeTest( ) ;

	// declare suit function
	CPPUNIT_TEST_SUITE( PageNodeTest ) ;
		CPPUNIT_TEST( TestCount ) ;
		CPPUNIT_TEST( TestGetLeaf ) ;
		CPPUNIT_TEST( TestOrder ) ;
	CPPUNIT_TEST_SUITE_END( ) ;

public :
	void setUp( ) ;
	void tearDown( ) ;

private :
	void TestCount( ) ;
	void TestGetLeaf( ) ;
	void TestOrder( ) ;

private :
	FT_Library		m_ft_lib ;
	pdf::PageTree	*m_root ;
} ;

#endif
