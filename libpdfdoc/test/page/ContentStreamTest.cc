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
#include "graphics/Text.hh"
#include "graphics/TextLine.hh"

#include "page/ContentStream.hh"
#include "page/MockResources.hh"

#include "stream/Stream.hh"

#include "mock/Assert.hh"
#include "mock/MockFont.hh"

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

void ContentStreamTest::TestTestCID( )
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

	class Visitor : public GraphicsVisitor
	{
	public :
		void VisitText( Text *text )
		{
			PDFUT_ASSERT_EQUAL( text->Transform(),
				Matrix::Translation( 72.0,  769.89) ) ;
		
			PDFUT_ASSERT_EQUAL( text->Count(), 7U ) ;
			Text::iterator i = text->begin() ;
			CPPUNIT_ASSERT( i != text->end() ) ;
			PDFUT_ASSERT_EQUAL( i->Transform(),
				Matrix::Translation( 61.77, -63.35) ) ;
			PDFUT_ASSERT_EQUAL( i->Text(), std::wstring(L"1SectionTitle") ) ;
			
			++i ;
			CPPUNIT_ASSERT( i != text->end() ) ;
		}
		
		void VisitGraphics( Graphics *text )
		{
		}
		
		void VisitPath( Path *path )
		{
		}
	} v ;

	ContentStream subject( str, str+1, &res, &v ) ;
	subject.Decode() ;
}

} // end of namespace
