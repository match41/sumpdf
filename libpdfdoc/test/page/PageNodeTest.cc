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
	\file	PageNodeTest.cc
	\brief	implementation the PageNodeTest class
	\date	Wed Oct 15 2008
	\author	Nestal Wan
*/

#include "PageNodeTest.hh"

#include "page/PageTree.hh"
#include "page/RealPage.hh"

#include "mock/Assert.hh"

PageNodeTest::PageNodeTest( )
	: m_root( 0 )
{
}

void PageNodeTest::setUp( )
{
	::FT_Init_FreeType( &m_ft_lib ) ;

	m_root = new pdf::PageTree( m_ft_lib ) ;
}

void PageNodeTest::tearDown( )
{
	delete m_root ;
	::FT_Done_FreeType( m_ft_lib ) ;
}

void PageNodeTest::TestCount( )
{
	pdf::PageTree *level1	= new pdf::PageTree( m_root ),
	              *level2	= new pdf::PageTree( level1 ) ;
	new pdf::RealPage( level2 ) ;
	
	PDF_ASSERT_EQUAL( m_root->Count( ), 1u ) ;

	new pdf::RealPage( level1 ) ;
	PDF_ASSERT_EQUAL( m_root->Count( ), 2u ) ;
}

void PageNodeTest::TestGetLeaf( )
{
	pdf::PageTree *level1	= new pdf::PageTree( m_root ),
	              *level2	= new pdf::PageTree( level1 ) ;
	pdf::RealPage *page		= new pdf::RealPage( level2 ) ;

	PDF_ASSERT_EQUAL( m_root->GetLeaf( 0 ), page ) ;
	PDF_ASSERT_NULL( m_root->GetLeaf( m_root->Count() ) ) ;
	PDF_ASSERT_NULL( m_root->GetLeaf( 100 ) ) ;

	pdf::RealPage *page2	= new pdf::RealPage( level1 ) ;
	PDF_ASSERT_EQUAL( m_root->GetLeaf( 0 ), page ) ;
	PDF_ASSERT_EQUAL( m_root->GetLeaf( 1 ), page2 ) ;
	PDF_ASSERT_NULL( m_root->GetLeaf( m_root->Count() ) ) ;
	PDF_ASSERT_NULL( m_root->GetLeaf( 100 ) ) ;
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
	pdf::PageTree *level0	= new pdf::PageTree( m_root ),
	              *level1	= new pdf::PageTree( level0 ) ;
	pdf::RealPage *page0	= new pdf::RealPage( level1 ),
	              *page1	= new pdf::RealPage( level1 ),
                  *page2	= new pdf::RealPage( level0 ),
                  *page3	= new pdf::RealPage( level0 ) ;
	
	CPPUNIT_ASSERT( m_root->Count( ) == 4 ) ;
	CPPUNIT_ASSERT( m_root->GetLeaf( 0 ) == page0 ) ;
	CPPUNIT_ASSERT( m_root->GetLeaf( 1 ) == page1 ) ;
	CPPUNIT_ASSERT( m_root->GetLeaf( 2 ) == page2 ) ;
	CPPUNIT_ASSERT( m_root->GetLeaf( 3 ) == page3 ) ;
}
