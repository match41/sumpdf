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
#include "page/RealResources.hh"
#include "util/Matrix.hh"
#include "util/Util.hh"

#include "mock/Assert.hh"

using namespace pdf ;

/**	constructor
*/
RealTextTest::RealTextTest( )
{
}

void RealTextTest::TestTdCmd( )
{
	RealText t ;
	PDF_ASSERT_EQUAL( t.Count(), 1U ) ;
	PDF_ASSERT_EQUAL( t.front().front().Format(), TextState() ) ;
	PDF_ASSERT_EQUAL( t.front().Transform(), Matrix() ) ;
	
	Object	args[]	= { 100, 200 } ;
	Token	cmd( "Td" ) ;
	
	RealResources res( m_ft ) ;
	
	t.OnCommand( cmd, args, Count(args), &res ) ;  
	PDF_ASSERT_EQUAL( t.front().front().Format(), TextState() ) ;
	PDF_ASSERT_EQUAL( t.front().Transform(), Matrix(1,0,0,1,100,200) ) ;
}
