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

/**	\file	GraphicsStateTest.cc
	\brief	implementation of the GraphicsStateTest class
	\date	Feb 18, 2010
	\author	Nestal Wan
*/

#include "GraphicsStateTest.hh"

#include "core/Token.hh"
#include "core/Object.hh"
#include "page/ContentOp.hh"
#include "page/MockResources.hh"
#include "graphics/Colour.hh"
#include "graphics/GraphicsState.hh"
#include "graphics/TextState.hh"

#include "mock/Assert.hh"
#include "mock/MockFont.hh"

#include "util/Util.hh"

namespace pdfut {

using namespace pdf ;

GraphicsStateTest::GraphicsStateTest( )
{
}

void GraphicsStateTest::setUp( )
{
}

void GraphicsStateTest::tearDown( )
{
}

void GraphicsStateTest::TestTextStateCommand( )
{
	MockFont font ;
	MockResources res ;
	Name font_name = res.AddFont( &font ) ;
	
	GraphicsState subject ;
	CPPUNIT_ASSERT( GraphicsState::IsGSCommand( Token("Tf") ) ) ;
	
	Object args[] = { font_name, 12.0 } ;
	ContentOp op( Token("Tf"), Begin(args), End(args) ) ;
	subject.OnCommand( op, &res ) ;  
	
	PDFUT_ASSERT_EQUAL( subject.GetTextState().FontSize(), 12.0 ) ;
	PDFUT_ASSERT_EQUAL( subject.GetTextState().GetFont(), &font ) ;
}

void GraphicsStateTest::TestColourCommand( )
{
	MockResources res ;
	GraphicsState subject ;
	Object args[] = { Name( "DeviceRGB" ) } ;
	ContentOp op( Token("CS"), Begin(args), End(args) ) ;
	subject.OnCommand( op, &res ) ;
	
	PDFUT_ASSERT_EQUAL( subject.StrokeColour().ColourSpace(), Colour::rgb ) ;
	PDFUT_ASSERT_EQUAL( subject.StrokeColour(), Colour(0.0, 0.0, 0.0) ) ;

	Object args2[] = { 1.0, 0.0, 0.0 } ;
	ContentOp op2( Token("RG"), Begin(args2), End(args2) ) ;
	subject.OnCommand( op2, &res ) ;

	PDFUT_ASSERT_EQUAL( subject.StrokeColour().ColourSpace(), Colour::rgb ) ;
	PDFUT_ASSERT_EQUAL( subject.StrokeColour(), Colour(1.0, 0.0, 0.0) ) ;
	
	GraphicsState prev ;
	std::ostringstream oss ;
	subject.Print( oss, &res, prev ) ;
	PDFUT_ASSERT_EQUAL( oss.str(), "1 0 0 RG\n" ) ;

	Object args3[] = { 0.0, 1.0, 0.0, 0.5 } ;
	ContentOp op3( Token("k"), Begin(args3), End(args3) ) ;
	subject.OnCommand( op3, &res ) ;

	std::ostringstream oss2 ;
	subject.Print( oss2, &res, prev ) ;
	PDFUT_ASSERT_EQUAL( oss2.str(), "1 0 0 RG\n0 1 0 0.5 k\n" ) ;
}

} // end of namespace
