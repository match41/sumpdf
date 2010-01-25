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
#include "mock/MockFont.hh"
#include "stream/Stream.hh"

#include <sstream>

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
	TextLine line( tm, ts ) ;
	line.AppendText( L"Hello World!\n" ) ;
	line.AppendText( L"This is line 2\n" ) ;
	t->AddLine( line ) ;
	
	Stream str ;
	subject.Write( str, &res ) ;
std::cout << str << std::endl ;
}
