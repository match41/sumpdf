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

/**	\file	RealContentTest.cc
	\brief	implementation of the RealContentTest class
	\date	Jan 25, 2010
	\author	Nestal Wan
*/

#include "RealContentTest.hh"

#include "page/RealContent.hh"
#include "page/MockResources.hh"
#include "graphics/Text.hh"
#include "graphics/TextLine.hh"
#include "graphics/TextState.hh"

#include "mock/Assert.hh"
#include "mock/MockFont.hh"

#include "stream/Stream.hh"

#include "util/Util.hh"

#include <sstream>

namespace pdfut {

using namespace pdf ;

/**	constructor
*/
RealContentTest::RealContentTest( )
{
}

void RealContentTest::TestWrite( )
{
	RealContent subject ;

	MockFont font ;
	TextState ts ;
	ts.SetFont( 12.0, &font ) ;
	MockResources res ;
	Name fname = res.AddFont( &font ) ; 
	
	Text *t = subject.AddText( ts ) ;
	Matrix tm( 1,0,0,1, 100, 200 ) ;
	RealTextLine line( 0, 0, (GraphicsState(ts)), tm ) ;
	line.AppendText( L"Hello World!\n" ) ;
	line.AppendText( L"This is line 2\n" ) ;
	t->AddLine( line ) ;

	Matrix tm2( 1,0,0,1, 100, 400 ) ;
	RealTextLine line2( 0, 0, (GraphicsState(ts)), tm2 ) ;
	line2.AppendText( L"Hello World2!\n" ) ;
	line2.AppendText( L"This is line 3\n" ) ;
	t->AddLine( line2 ) ;

	Stream str ;
	subject.Write( str, &res ) ;
}

void RealContentTest::TestRead( )
{
	MockFont font ;
	MockResources res ;
	Name fname = res.AddFont( &font ) ; 

	std::ostringstream oss ;
	oss << fname << " 14.0 Tf BT (Hello World!) Tj ET" ;

	Stream str[1] = { Stream( oss.str() ) } ;

	RealContent subject ;
	subject.Load( Begin(str), End(str), &res ) ;
	
	PDFUT_ASSERT_EQUAL( subject.Count(), 1U ) ;
	
	TextState ts ;
	ts.SetFont( 14.0, &font ) ;
	Matrix tm( 1,0,0,1, 0, 0 ) ;
	RealTextLine tl( 0, 0, (GraphicsState(ts)), tm ) ;
	tl.AppendText( L"Hello World!" ) ;
	
	const Text *text = dynamic_cast<const Text*>( subject.Item(0) ) ;
	CPPUNIT_ASSERT( text != 0 ) ;
	PDFUT_ASSERT_EQUAL( text->Count(), 1U ) ;
	Text::const_iterator tit = text->begin() ;
	CPPUNIT_ASSERT( tit != text->end() ) ;
	PDFUT_ASSERT_EQUAL( *tit, tl ) ;
}

} // end of namespace
