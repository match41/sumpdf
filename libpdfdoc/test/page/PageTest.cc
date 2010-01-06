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

#include "PageTest.hh"

#include "mock/MockFile.hh"

#include "core/Ref.hh"
#include "core/Dictionary.hh"
#include "font/StandardFont.hh"
#include "page/RealPage.hh"
#include "page/PageTree.hh"
#include "page/PaintOp.hh"
#include "stream/Stream.hh"
#include "util/Rect.hh"

#include <sstream>
#include <iostream>

PageTest::PageTest( )
	: m_root( 0 )
{
}

void PageTest::setUp( )
{
	// this is the parent page node of the test pages. as it is deleted in
	// tearDown() all its child pages will be delete, there is no need to
	// delete the test pages in each case.
	m_root = new pdf::PageTree ;
}

void PageTest::tearDown( )
{
	delete m_root ;
}

void PageTest::TestNormal( )
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
	
	pdf::Object obj( d ) ;
	pdf::RealPage *p  = new pdf::RealPage( m_root ) ;
	p->Read( d, &file ) ;

	CPPUNIT_ASSERT( p->MediaBox() == pdf::Rect( 0, 0, 297, 419 ) ) ;
}

void PageTest::TestWrite( )
{
	pdf::RealPage *p  = new pdf::RealPage( m_root ) ;
	pdf::StandardFont *f = new pdf::StandardFont( "TimesNewRoman" ) ;
	p->DrawText( 120, 300, f, "This is a line" ) ;
	p->DrawText( 120, 400, f, "This is another line" ) ;
	p->Finish( ) ;
	
	MockFile file ;
	pdf::Ref link = file.AllocLink( ) ;
	p->Write( link, &file, file.AllocLink( ) ) ;
	
	pdf::Object out = file.Find( link ) ;
	CPPUNIT_ASSERT( out.Is<pdf::Dictionary>( ) ) ;
	
	pdf::RealPage *p2 = new pdf::RealPage( m_root ) ;
	p2->Read( out.As<pdf::Dictionary>(), &file ) ;
	CPPUNIT_ASSERT( p->MediaBox() == p2->MediaBox() ) ;
}

void PageTest::TestDecode( )
{
	pdf::RealPage *p  = new pdf::RealPage( m_root ) ;
	pdf::StandardFont *f = new pdf::StandardFont( "TimesNewRoman" ) ;
	p->DrawText( 120, 300, f, "This is a line" ) ;
	p->DrawText( 120, 400, f, "This is another line" ) ;
	p->Finish( ) ;

	std::vector<pdf::PaintOp> ops ;
	p->Decode( ops ) ;
	
	CPPUNIT_ASSERT( ops.size() == 10 ) ;
	CPPUNIT_ASSERT( ops[0].Code() == pdf::PaintOp::begin_text ) ;
	CPPUNIT_ASSERT( ops[1].As<pdf::TextFont>().font == f ) ;
	CPPUNIT_ASSERT( ops[2].As<pdf::TextPosition>().offx == 120 ) ;
	CPPUNIT_ASSERT( ops[2].As<pdf::TextPosition>().offy == 300 ) ;
}
