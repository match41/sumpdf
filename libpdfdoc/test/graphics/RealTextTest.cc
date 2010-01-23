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

#include "core/Object.hh"
#include "graphics/RealText.hh"
#include "page/MockResources.hh"
#include "util/Matrix.hh"
#include "util/Util.hh"

#include "mock/Assert.hh"
#include "mock/MockFont.hh"

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

	RealText t( ts ) ;
	PDF_ASSERT_EQUAL( t.Count(), 1U ) ;
	PDF_ASSERT_EQUAL( t.front().front().Format(), ts ) ;
	PDF_ASSERT_EQUAL( t.front().Transform(), Matrix() ) ;

	// action: translate to 100,200
	Object	args[]	= { 100, 200 } ;
	Token	cmd( "Td" ) ;
	t.OnCommand( cmd, args, Count(args), &res ) ;  
	
	// expected: no new text line is added. current matrix translated
	PDF_ASSERT_EQUAL( t.Count(), 1U ) ;
	PDF_ASSERT_EQUAL( t.front().front().Format(), ts ) ;
	PDF_ASSERT_EQUAL( t.front().Transform(), Matrix(1,0,0,1,100,200) ) ;

	// action: move another 300,900
	args[0] = 300 ;
	args[1] = 900 ;
	t.OnCommand( cmd, args, Count(args), &res ) ;  
	
	// expected: the two translation is combined
	PDF_ASSERT_EQUAL( t.Count(), 1U ) ;
	PDF_ASSERT_EQUAL( t.front().front().Format(), ts ) ;
	PDF_ASSERT_EQUAL( t.front().Transform(), Matrix(1,0,0,1,400,1100) ) ;
	
	RealText exp( ts ) ;
	exp.back() = TextLine( Matrix(1,0,0,1,400,1100), ts ) ;
	PDF_ASSERT_EQUAL( t, exp ) ;
}
