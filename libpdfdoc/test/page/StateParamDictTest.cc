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

#include "StateParamDictTest.hh"

#include "core/Dictionary.hh"
#include "file/DictReader.hh"
#include "graphics/GraphicsState.hh"
#include "page/StateParamDict.hh"

#include "mock/Assert.hh"
#include "mock/MockFile.hh"

namespace pdfut {

using namespace pdf ;

StateParamDictTest::StateParamDictTest( )
{
}

void StateParamDictTest::setUp( )
{
}

void StateParamDictTest::tearDown( )
{
}

void StateParamDictTest::TestRead( )
{
	StateParamDict subject ;
	
	Dictionary dict ;
	dict["LW"] = 100 ;
 	
 	MockFile file ;
 	DictReader reader( dict, &file ) ;
 	
 	subject.Read( reader ) ;
 	GraphicsState gs ;
 	subject.Apply( gs ) ;
 	PDFUT_ASSERT_EQUAL( gs.LineWidth(), 100.0 ) ;
}

} // end of namespace
