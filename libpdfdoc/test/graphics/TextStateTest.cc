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

/**	\file	TextStateTest.cc
	\brief	implementation of the TextStateTest class
	\date	Jan 23, 2010
	\author	Nestal Wan
*/

#include "TextStateTest.hh"

#include "graphics/TextState.hh"
#include "page/Resources.hh"

#include "mock/Assert.hh"
#include "mock/MockFont.hh"

#include <sstream>
#include <iostream>

using namespace pdf ;

/**	constructor
*/
TextStateTest::TextStateTest( )
{
}

void TextStateTest::TestPrint( )
{
	TextState ts ;
	ts.SetCharSpace( 100 ) ;
	
	MockFont *font = new MockFont ;
	ts.SetFont( 12.0, font ) ;
	
	Resources r( m_ft ) ;
	Name n = r.AddFont( font ) ;
	
	std::ostringstream ss ;
	ts.Print( ss, &r ) ;

std::cout << ss.str() << std::endl ;
}

