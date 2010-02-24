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

/**	\file	RealTextTest.cc
	\brief	implementation of the RealTextTest class
	\date	Jan 23, 2010
	\author	Nestal Wan
*/

#include "RealTextTest.hh"

#include "core/Array.hh"
#include "core/Object.hh"
#include "graphics/RealText.hh"
#include "page/MockResources.hh"
#include "util/Matrix.hh"
#include "util/Util.hh"

#include "mock/Assert.hh"
#include "mock/MockFont.hh"

namespace pdfut {

using namespace pdf ;

/**	constructor
*/
RealTextTest::RealTextTest( )
{
}

void RealTextTest::TestTdCmd( )
{
	MockResources res ;
	MockFont font ;
	res.AddFont( &font ) ;
	
	TextState ts ;
	ts.SetFont( 12.0, &font ) ;
	GraphicsState gs( ts ) ;

	RealText t( gs ) ;
	PDFUT_ASSERT_EQUAL( t.Count(), 1U ) ;
	PDFUT_ASSERT_EQUAL( t.front().Format(), gs ) ;
	PDFUT_ASSERT_EQUAL( t.front().Transform(), Matrix() ) ;

	// action: translate to 100,200
	Object	args[]	= { 100, 200 } ;
	Token	cmd( "Td" ) ;
	t.OnCommand( cmd, args, Count(args), &res ) ;  
	
	// expected: no new text line is added. current matrix translated
	PDFUT_ASSERT_EQUAL( t.Count(), 1U ) ;
	PDFUT_ASSERT_EQUAL( t.front().Format(), gs ) ;
	PDFUT_ASSERT_EQUAL( t.front().Transform(), Matrix() ) ;
	PDFUT_ASSERT_EQUAL( t.front().XPos(), 100.0 ) ;
	PDFUT_ASSERT_EQUAL( t.front().YPos(), 200.0 ) ;

	// action: move another 300,900
	args[0] = 300 ;
	args[1] = 900 ;
	t.OnCommand( cmd, args, Count(args), &res ) ;  
	
	// expected: the two translation is combined
	PDFUT_ASSERT_EQUAL( t.Count(), 1U ) ;
	PDFUT_ASSERT_EQUAL( t.front().Format(), gs ) ;
	PDFUT_ASSERT_EQUAL( t.front().Transform(), Matrix() ) ;
	PDFUT_ASSERT_EQUAL( t.front().XPos(), 400.0 ) ;
	PDFUT_ASSERT_EQUAL( t.front().YPos(), 1100.0 ) ;
	
	RealText exp( gs ) ;
	exp.back() = TextLine( 400, 1100, gs, Matrix() ) ;
	PDFUT_ASSERT_EQUAL( t, exp ) ;
}

void RealTextTest::TestTj( )
{
	MockResources res ;
	MockFont font ;
	Name fname = res.AddFont( &font ) ;
	
	TextState ts( 12.0, &font ) ;
	GraphicsState gs( ts ) ;

	RealText t( gs ) ;
	
	// action: display a string "abc"
	Object	args[]	= { "abc" } ;
	Token	cmd( "Tj" ) ;
	t.OnCommand( cmd, args, Count(args), &res ) ;
	
	PDFUT_ASSERT_EQUAL( t.Count(), 1U ) ;
	PDFUT_ASSERT_EQUAL( t.front().Format(), gs ) ;
	PDFUT_ASSERT_EQUAL( t.front().Transform(), Matrix() ) ;
	CPPUNIT_ASSERT( t.front().Text() == L"abc" ) ;
	PDFUT_ASSERT_EQUAL( t.front().Width(), 18 ) ;
	PDFUT_ASSERT_EQUAL( t.front().Format().GetFont(), &font ) ;
	
	// action: display another string "abc"
	Object	args2[]	= { "abc" } ;
	Token	cmd2( "Tj" ) ;
	t.OnCommand( cmd2, args2, Count(args2), &res ) ;
	
	PDFUT_ASSERT_EQUAL( t.Count(), 1U ) ;
	PDFUT_ASSERT_EQUAL( t.back().Format(), gs ) ;
		
	CPPUNIT_ASSERT( t.back().Text() == L"abcabc" ) ;
	PDFUT_ASSERT_EQUAL( t.back().Width(), 36 ) ;
	
	// double font size
	TextState ts2x( 24.0, &font ) ;
	GraphicsState gs2x( ts2x ) ;
	Object	args3[]	= { fname, 24.0 } ;
	Token	cmd3( "Tf" ) ;
	t.OnCommand( cmd3, args3, Count(args3), &res ) ;
	
	// add text again
	Object	args4[]	= { "cdef" } ;
	Token	cmd4( "Tj" ) ;
	t.OnCommand( cmd4, args4, Count(args4), &res ) ;

	PDFUT_ASSERT_EQUAL( t.Count(), 2U ) ;
	PDFUT_ASSERT_EQUAL( t.back().Format(), gs2x ) ;
	PDFUT_ASSERT_EQUAL( t.back().Transform(), Matrix() ) ;
	PDFUT_ASSERT_EQUAL( t.back().XPos(), 36.0 ) ;
	PDFUT_ASSERT_EQUAL( t.back().YPos(), 0.0 ) ;
}

void RealTextTest::TestTJ( )
{
	MockResources res ;
	MockFont font ;
	Name fname = res.AddFont( &font ) ;
	
	TextState ts( 12.0, &font ) ;
	GraphicsState gs( ts ) ;

	RealText t( gs ) ;
	
	// action: display a string "abc"
	Object	tj_ops[] = { "abc", -2000.0, "cde" } ;
	Token	cmd( "TJ" ) ;
	Array	tj( Begin(tj_ops), End(tj_ops) ) ;
	Object	args[] = { tj } ;
	t.OnCommand( cmd, args, Count(args), &res ) ;

	PDFUT_ASSERT_EQUAL( t.Count(), 1U ) ;
	PDFUT_ASSERT_EQUAL( t.front().Format(), gs ) ;
	
	const TextLine& line = t.front() ;
	PDFUT_ASSERT_EQUAL( line.Width(), 60.0 ) ;

	// double font size and a new TextLine object should be generated
	Object	args3[]	= { fname, 24.0 } ;
	Token	cmd3( "Tf" ) ;
	t.OnCommand( cmd3, args3, Count(args3), &res ) ;
	
	Object	args4[]	= { "cdef" } ;
	Token	cmd4( "Tj" ) ;
	t.OnCommand( cmd4, args4, Count(args4), &res ) ;

	// 2 text lines in total
	PDFUT_ASSERT_EQUAL( t.Count(), 2U ) ;

	// next object starts at 60.0 units further
	PDFUT_ASSERT_EQUAL( t.back().Transform(), Matrix() ) ;
	PDFUT_ASSERT_EQUAL( t.back().XPos(), 60.0 ) ;
	PDFUT_ASSERT_EQUAL( t.back().YPos(), 0.0 ) ;
}

void RealTextTest::TestTjx2( )
{
	MockResources res ;
	MockFont font ;
	Name fname = res.AddFont( &font ) ;
	
	TextState ts( 12.0, &font ) ;
	GraphicsState gs( ts ) ;
	RealText t( gs ) ;

	// action: display a string "111"
	Object	args[]	= { "111" } ;
	Token	cmd( "Tj" ) ;
	t.OnCommand( cmd, args, Count(args), &res ) ;

	// action: display a string "222"
	Object	args2[]	= { "222" } ;
	Token	cmd2( "Tj" ) ;
	t.OnCommand( cmd2, args2, Count(args2), &res ) ;

	PDFUT_ASSERT_EQUAL( t.Count(), 1U ) ;
	PDFUT_ASSERT_EQUAL( t.front().Format(), gs ) ;
	PDFUT_ASSERT_EQUAL( t.front().Transform(), Matrix() ) ;
	CPPUNIT_ASSERT( t.front().Text() == L"111222" ) ;
}

void RealTextTest::TestName( )
{
	MockResources res ;
	MockFont font ;
	Name fname = res.AddFont( &font ) ;
	
	TextState ts( 12.0, &font ) ;
	GraphicsState gs( ts ) ;
	RealText subject( gs ) ;
	
	Object tj[] = { "Name:", -10072, "M", 80, "atch", -250, "man,", -250, "Jr"};
	Object args[] = { Array(Begin(tj), End(tj)) } ;
	Token cmd( "TJ" ) ;
	subject.OnCommand( cmd, args, Count(args), &res ) ;

	PDFUT_ASSERT_EQUAL( subject.Count(), 1U ) ;
	
	TextLine line = subject.front() ;
	PDFUT_ASSERT_EQUAL( line.Format(), gs ) ;
	PDFUT_ASSERT_EQUAL( line.Transform(), Matrix() ) ;
	
	double x = 0.0, y = 0.0 ;
	Matrix mat ;
	std::stringstream ss ;
	line.Print( ss, mat, x, y, gs, &res ) ;
std::cout << "out = " << ss.str() << std::endl ;
}

} // end of nameapce
