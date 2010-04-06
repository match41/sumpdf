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

/**	\file	RealPathTest.cc
	\brief	implementation of the RealPathTest class
	\date	Apr 6, 2010
	\author	Nestal Wan
*/

#include "RealPathTest.hh"

#include "RealDoc.hh"

#include "page/Page.hh"
#include "graphics/Color.hh"

#include "graphics/Path.hh"
#include "graphics/PathSegment.hh"

#include "mock/Assert.hh"
#include "mock/MockGraphicsVisitor.hh"

namespace pdfut {

using namespace pdf ;

RealPathTest::RealPathTest( )
{
}

void RealPathTest::setUp( )
{
}

void RealPathTest::tearDown( )
{
}

void RealPathTest::Test( )
{
	RealDoc doc ;
	doc.Read( std::string(TEST_DATA_DIR) + "Star.pdf" ) ;
	
	PDFUT_ASSERT_EQUAL( doc.PageCount(), 1U ) ;
	
	Page *page1 = doc.GetPage(0) ;
	CPPUNIT_ASSERT( page1 != 0 ) ;
	
	class Visitor : public MockGraphicsVisitor
	{
	public :
		Visitor( ) : m_index( ) {}
		
		void VisitPath( Path *path )
		{
			CPPUNIT_ASSERT( path != 0 ) ;
			CPPUNIT_ASSERT( path->Count() > 0 ) ;

			if ( m_index == 0 )
			{
				PDFUT_ASSERT_EQUAL( path->Count(), 13U ) ;
				
				PathSegment p = path->Segment(0) ;
				PDFUT_ASSERT_EQUAL( p.GetOp(), PathSegment::move ) ;
				PDFUT_ASSERT_EQUAL( p.Count(), 2U ) ;
				
				double exp[] = { 96, 131.031 } ;
				PDFUT_ASSERT_RANGE_EQUAL( p.begin(), p.end(), exp ) ;
			}
			
			++m_index ;
		}
	
	private :
		int m_index ;
	} v ;
	
	page1->VisitGraphics( &v ) ;
}

} // end of namespace
