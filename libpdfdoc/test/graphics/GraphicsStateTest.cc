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
#include "page/MockResources.hh"
#include "graphics/GraphicsState.hh"

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
	
	Object args[] = { font_name, 12.0 } ;
	CPPUNIT_ASSERT( subject.OnCommand( Token("Tf"), args, Count(args), &res ) );
	PDFUT_ASSERT_EQUAL( subject.GetTextState().FontSize(), 12.0 ) ;
	PDFUT_ASSERT_EQUAL( subject.GetTextState().GetFont(), &font ) ;
}

} // end of namespace
