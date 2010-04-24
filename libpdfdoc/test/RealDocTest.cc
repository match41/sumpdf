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
	\file	RealDocTest.cc
	\brief	definition the RealDocTest class
	\date	Dec 27, 2009
	\author	Nestal Wan
*/

#include "RealDocTest.hh"

#include "RealDoc.hh"
#include "DocInfo.hh"

#include "page/Page.hh"
#include "graphics/Color.hh"
#include "graphics/GraphicsState.hh"
#include "graphics/Text.hh"
#include "graphics/TextLine.hh"

#include "mock/Assert.hh"
#include "mock/MockGraphicsVisitor.hh"

#include <iostream>

namespace pdfut {

using namespace pdf ;

RealDocTest::RealDocTest( )
{
}

void RealDocTest::TestRead( )
{
	pdf::RealDoc doc ;
	doc.Read( std::string(TEST_DATA_DIR) + "FileTestSimple.pdf" ) ;
	
	pdf::DocInfo *info = doc.Info( ) ;
	CPPUNIT_ASSERT( info != 0 ) ;
	PDFUT_ASSERT_EQUAL( info->Producer(),	"nestal" ) ;
	PDFUT_ASSERT_EQUAL( info->Creator(),	"D:20080410074227" ) ;
	
	CPPUNIT_ASSERT( doc.PageCount() > 0 ) ;
	
	Page *page1 = doc.GetPage(0) ;
	CPPUNIT_ASSERT( page1 != 0 ) ;
}

void RealDocTest::TestNew( )
{
	RealDoc doc ;
	PDFUT_ASSERT_EQUAL( doc.PageCount(), 1u ) ;
}

void RealDocTest::TestReadColour( )
{
	RealDoc doc ;
	doc.Read( std::string(TEST_DATA_DIR) + "Color.pdf" ) ;
	
	PDFUT_ASSERT_EQUAL( doc.PageCount(), 1U ) ;
	
	Page *page1 = doc.GetPage(0) ;
	CPPUNIT_ASSERT( page1 != 0 ) ;
	
	class Visitor : public MockGraphicsVisitor
	{
	public :
		void VisitText( Text *text )
		{
			CPPUNIT_ASSERT( text != 0 ) ;
			CPPUNIT_ASSERT( text->Count() > 0 ) ;

			for ( Text::iterator i = text->begin() ; i != text->end() ; ++i )
				m_lines.push_back( *i ) ;
		}
		std::vector<TextLine> m_lines ;
	} v ;
	
	page1->VisitGraphics( &v ) ;
	
	CPPUNIT_ASSERT( !v.m_lines.empty() ) ;
	PDFUT_ASSERT_EQUAL( v.m_lines.front().Format().FillColor(),
		Color( 1,0,0 ) ) ;
}

} // end of namespace
