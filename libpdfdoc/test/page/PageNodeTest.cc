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
	\file	PageNodeTest.cc
	\brief	implementation the PageNodeTest class
	\date	Wed Oct 15 2008
	\author	Nestal Wan
*/

#include "PageNodeTest.hh"

#include "page/PageTree.hh"
#include "page/RealPage.hh"

PageNodeTest::PageNodeTest( )
{
}

void PageNodeTest::TestCount( )
{
	pdf::PageTree root, level1( &root ), level2( &level1 ) ;
	pdf::RealPage page( &level2 ) ;
	
	CPPUNIT_ASSERT( root.Count( ) == 1 ) ;

	pdf::RealPage page2( &level1 ) ;
	CPPUNIT_ASSERT( root.Count( ) == 2 ) ;
}

void PageNodeTest::TestGetLeaf( )
{
	pdf::PageTree root, level1( &root ), level2( &level1 ) ;
	pdf::RealPage page( &level2 ) ;

	CPPUNIT_ASSERT( root.GetLeaf( 0 ) == &page ) ;
	CPPUNIT_ASSERT( root.GetLeaf( root.Count() ) == 0 ) ;
	CPPUNIT_ASSERT( root.GetLeaf( 100 ) == 0 ) ;

	pdf::RealPage page2( &level1 ) ;
	CPPUNIT_ASSERT( root.GetLeaf( 0 ) == &page ) ;
	CPPUNIT_ASSERT( root.GetLeaf( 1 ) == &page2 ) ;
	CPPUNIT_ASSERT( root.GetLeaf( root.Count() ) == 0 ) ;
	CPPUNIT_ASSERT( root.GetLeaf( 100 ) == 0 ) ;
}

void PageNodeTest::TestOrder( )
{
	// root
	// |- level0
	//    |- level1
	//    |  |- page0
	//    |  \- page1
	//    |- page2
	//    \- page3
	pdf::PageTree root, level0( &root ), level1( &level0 ) ;
	pdf::RealPage page0( &level1 ), page2( &level0 ), page3( &level0 ),
	              page1( &level1 ) ;
	
	CPPUNIT_ASSERT( root.Count( ) == 4 ) ;
	CPPUNIT_ASSERT( root.GetLeaf( 0 ) == &page0 ) ;
	CPPUNIT_ASSERT( root.GetLeaf( 1 ) == &page1 ) ;
	CPPUNIT_ASSERT( root.GetLeaf( 2 ) == &page2 ) ;
	CPPUNIT_ASSERT( root.GetLeaf( 3 ) == &page3 ) ;
}
