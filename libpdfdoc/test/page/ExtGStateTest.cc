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

/**	\file	StateParamDictTest.cc
	\brief	implementation of the StateParamDictTest class
	\date	Mar 1, 2010
	\author	Nestal Wan
*/

#include "ExtGStateTest.hh"

#include "core/Dictionary.hh"
#include "file/DictReader.hh"
#include "graphics/GraphicsState.hh"
#include "page/ExtGState.hh"

#include "mock/Assert.hh"
#include "mock/MockFile.hh"

namespace pdfut {

using namespace pdf ;

ExtGStateTest::ExtGStateTest( )
{
}

void ExtGStateTest::setUp( )
{
}

void ExtGStateTest::tearDown( )
{
}

void ExtGStateTest::TestRead( )
{
	ExtGState subject ;
	
	Dictionary dict ;
	dict.insert( "LW",	100 ) ;
 	
 	MockFile file ;
 	DictReader reader( dict, &file ) ;
 	
 	subject.Read( reader ) ;
 	
 	GraphicsState gs ;
 	gs.LineWidth( 301.0 ) ;
 	
 	subject.Apply( gs ) ;
 	PDFUT_ASSERT_EQUAL( gs.LineWidth(), 100.0 ) ;
}

} // end of namespace
