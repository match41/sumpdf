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

/**	\file	TextLineTest.cc
	\brief	implementation of the TextLineTest class
	\date	Feb 2, 2010
	\author	Nestal Wan
*/

#include "TextLineTest.hh"

#include "core/Array.hh"
#include "core/Name.hh"
#include "graphics/TextLine.hh"
#include "graphics/TextState.hh"
#include "util/Matrix.hh"

#include "page/MockResources.hh"
#include "mock/MockFont.hh"

#include <iostream>

namespace pdfut {

using namespace pdf ;

/**	constructor
	
*/
TextLineTest::TextLineTest( )
{
}

void TextLineTest::TestPrint( )
{
	MockResources res ;
	MockFont font ;
	Name fname = res.AddFont( &font ) ;
	
	TextState ts( 12.0, &font ) ;
	TextLine subject( 0, 0, (GraphicsState(ts)), (Matrix()) ) ;
	subject.AppendText( L"hello---" ) ;
	subject.AppendSpace( 100 ) ;
	subject.AppendText( L"world???" ) ;
	
//	double x = 0.0, y = 0.0 ;
	Matrix m ;
	GraphicsState gs( ts ) ;
//	subject.Print( std::cout, m, x, y, gs, &res ) ;
}

void TextLineTest::TestTJ( )
{
	
	
	
}

} // end of namespace
