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

/**	\file	ContentStreamTest.cc
	\brief	implementation of the ContentStreamTest class
	\date	Mar 19, 2010
	\author	Nestal Wan
*/

#include "ContentStreamTest.hh"

#include "graphics/GraphicsVisitor.hh"
#include "graphics/Path.hh"
#include "graphics/Text.hh"
#include "graphics/RealTextLine.hh"

#include "page/ContentStream.hh"
#include "page/MockResources.hh"

#include "stream/Stream.hh"

#include "mock/Assert.hh"
#include "mock/MockFont.hh"
#include "mock/MockGraphicsVisitor.hh"

#include "util/Matrix.hh"

#include <sstream>

namespace pdfut {

using namespace pdf ;

ContentStreamTest::ContentStreamTest( )
{
}

void ContentStreamTest::setUp( )
{
}

void ContentStreamTest::tearDown( )
{
}

void ContentStreamTest::TestCID( )
{
	Stream str[] =
	{
		Stream(
			" q 1 0 0 1 72 769.89 cm BT /F1 14.346 Tf 61.77 -63.35 Td"
			"[(1)-999(Section)-250(T)54(itle)]TJ /F2 9.963 Tf 0 -22.7 Td"
			"[(This)-249(is)-250(a)-250(test)-250(document)-250(cr)17(eated)"
			"-249(by)-250(L)]TJ 156.14 -2.49 Td[(Y)]TJ 5.4 2.49 Td"
			"[(X.)-310(Seems)-250(like)-250(L)]TJ 65.54 -2.49 Td[(Y)]TJ"
			" 5.4 2.49 Td[(X)-250(always)-250(uses)-250(CID)-250(fonts.)]TJ "
			"-63.12 -544.59 Td[(1)]TJ ET Q" ),
	} ;

	MockFont font ;
	MockResources res ;
	res.Add( Name("F1"), &font ) ;

	class Visitor : public MockGraphicsVisitor
	{
	public :
		void VisitText( Text *text )
		{
			PDFUT_ASSERT_EQUAL( text->Transform(),
				Matrix::Translation( 72.0,  769.89) ) ;
		
			PDFUT_ASSERT_EQUAL( text->Count(), 7U ) ;
			
			TextLine *i = text->At(0) ;
			PDFUT_ASSERT_EQUAL( i->Transform(),
				Matrix::Translation( 61.77, -63.35) ) ;
			PDFUT_ASSERT_EQUAL( i->Text(), std::wstring(L"1SectionTitle") ) ;
		}
	} v ;

	ContentStream subject( str, str+1, &res, &v ) ;
	subject.Decode() ;
}

void ContentStreamTest::TestF( )
{
	Stream str[] =
	{
		Stream( "q\n0.03137 0.2902 0.3451 rg\n36 783 523 23 re\nf\nQ\n" )
	} ;

	class Visitor : public MockGraphicsVisitor
	{
	public :
		Visitor()
			: m_path_count( 0 )
		{
		}
		
		void VisitPath( Path *path )
		{
			m_path_count++ ;
			CPPUNIT_ASSERT( path->IsFill() ) ;
		}
	
		int m_path_count ;
	} v ;

	MockResources res ;
	ContentStream subject( str, str+1, &res, &v ) ;
	subject.Decode() ;
	
	PDFUT_ASSERT_EQUAL( v.m_path_count, 1 ) ;
}

} // end of namespace
