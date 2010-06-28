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
	\file	PageTest.cc
	\brief	implementation the PageTest class
	\date	Thu Mar 20 2008
	\author	Nestal Wan
*/

#include "RealPageTest.hh"

#include "mock/MockFile.hh"
#include "mock/MockFont.hh"
#include "mock/Assert.hh"

#include "core/Ref.hh"
#include "core/Dictionary.hh"
#include "file/DictReader.hh"
#include "font/SimpleFont.hh"
#include "graphics/RealText.hh"
#include "graphics/TextState.hh"
#include "page/RealPage.hh"
#include "page/PageTree.hh"
#include "page/RealResources.hh"
#include "stream/Stream.hh"
#include "util/Rect.hh"
#include "util/Matrix.hh"

#include "mock/MockGraphicsVisitor.hh"

#include <sstream>
#include <iostream>

namespace pdfut {

using namespace pdf ;

RealPageTest::RealPageTest( )
	: m_root( 0 )
{
}

void RealPageTest::setUp( )
{
	TestBase::setUp( ) ;

	// this is the parent page node of the test pages. as it is deleted in
	// tearDown() all its child pages will be delete, there is no need to
	// delete the test pages in each case.
	m_root = new pdf::PageTree( m_font_db ) ;
}

void RealPageTest::tearDown( )
{
	delete m_root ;
	
	TestBase::tearDown( ) ;
}

void RealPageTest::TestNormal( )
{
	std::istringstream ss( "<</Contents 611 0 R /CropBox [0 0 297 419 ] "
	                       "/MediaBox [0 0 297 419 ] /Parent "
	                       "697 0 R /Resources <</ColorSpace"
	                       "<</CS0 708 0 R >> /ExtGState <</GS0 713 0 R"
	                       "/GS1 721 0 R >> /Font <</T1_0 707 0 R "
	                       "/T1_1 627 0 R /T1_2 674 0 R >> /ProcSet"
	                       "[/PDF /Text ]  >> "
	                       "/Rotate 0 /Type /Page >>" ) ;
	pdf::Dictionary d ;
	CPPUNIT_ASSERT( ss >> d ) ;
	
	std::istringstream fss( "<</BaseFont /Helvetica-Bold\n"
	                        "/Encoding /WinAnsiEncoding\n"
	                        "/FirstChar 0\n"
	                        "/LastChar 0\n"
	                        "/Subtype /Type1\n"
	                        "/Type /Font\n"
	                        ">>" ) ;
	pdf::Dictionary fd ;
	CPPUNIT_ASSERT( fss >> fd ) ;

	std::istringstream pss( "<< /Count 1 /Kids [1 0 R]\n"
	                        "/MediaBox [0 0 595.1 842.1]\n"
	                        "/Type /Pages >>" ) ;
	pdf::Dictionary pd ;
	CPPUNIT_ASSERT( pss >> pd ) ;

	MockFile file ;
	file.AddObj( pdf::Ref(1,0), d ) ;
	file.AddObj( pdf::Ref( 611, 0 ), pdf::Stream() ) ;
	file.AddObj( pdf::Ref( 707, 0 ), fd ) ;
	file.AddObj( pdf::Ref( 627, 0 ), fd ) ;
	file.AddObj( pdf::Ref( 674, 0 ), fd ) ;
	file.AddObj( pdf::Ref( 697, 0 ), pd ) ;
	
	pdf::Dictionary dummy ;
	file.AddObj( Ref( 713, 0 ), dummy ) ;
	file.AddObj( Ref( 721, 0 ), dummy ) ;
	
	pdf::Object obj( d ) ;
	pdf::RealPage *p  = new pdf::RealPage( m_root ) ;
	
	DictReader reader( d, &file ) ;
	p->Read( reader ) ;

	PDFUT_ASSERT_EQUAL( p->MediaBox(), pdf::Rect( 0, 0, 297, 419 ) ) ;
}

void RealPageTest::TestWrite( )
{
	RealPage *p  = new pdf::RealPage( m_root ) ;
	SimpleFont *f = new SimpleFont(
		TEST_DATA_DIR + std::string("FreeMonoBoldOblique.ttf"), m_font_db ) ;

	pdf::TextState ts ;
	ts.ChangeFont( 12.0, f ) ;
	p->Resource()->AddFont( f ) ;

	MockFile file ;
	pdf::Ref link = file.AllocLink( ) ;
	p->Write( link, &file, file.AllocLink( ), 0 ) ;
	file.ClearPool( ) ;
	
	pdf::Object out = file.Find( link ) ;
	CPPUNIT_ASSERT( out.Is<pdf::Dictionary>( ) ) ;
	
	pdf::RealPage *p2 = new pdf::RealPage( m_root ) ;
	
	DictReader reader( out.As<pdf::Dictionary>(), &file ) ;
	p->Read( reader ) ;
	
	PDFUT_ASSERT_EQUAL( p->MediaBox(), p2->MediaBox() ) ;
}

void RealPageTest::TestDecode( )
{
	pdf::RealPage *p  = new pdf::RealPage( m_root ) ;
	SimpleFont *f = new SimpleFont(
		TEST_DATA_DIR + std::string("FreeMonoBoldOblique.ttf"), m_font_db ) ;

	pdf::TextState ts ;
	ts.ChangeFont( 12.0, f ) ;
	p->Resource()->AddFont( f ) ;

	// visitor
	struct V : public MockGraphicsVisitor
	{
		std::size_t m_count ;
		V( ) : m_count( 0 ) { }
		
		void VisitText( pdf::Text *t )
		{
			CPPUNIT_ASSERT( t != 0 ) ;
			PDFUT_ASSERT_EQUAL( t->Count(), 2U ) ;
			
			const pdf::Matrix& m = t->At(0)->Transform() ;
			if ( m_count == 0 )
			{
				PDFUT_ASSERT_EQUAL( m.Dx(), 120 ) ;
				PDFUT_ASSERT_EQUAL( m.Dy(), 300 ) ;
				m_count++ ;
			}
			else if ( m_count == 1 )
			{
				PDFUT_ASSERT_EQUAL( m.Dx(), 120 ) ;
				PDFUT_ASSERT_EQUAL( m.Dy(), 400 ) ;
			}
			
		}
	} v ;
	
	p->VisitGraphics( &v ) ;
}

} // end of namespace
