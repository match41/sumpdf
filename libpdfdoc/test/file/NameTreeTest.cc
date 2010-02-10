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

/**	\file	NameTreeTest.cc
	\brief	implementation of the NameTreeTest class
	\date	Feb 4, 2010
	\author	Nestal Wan
*/

#include "NameTreeTest.hh"

#include "file/NameTree.hh"
#include "file/DictReader.hh"

#include "core/Array.hh"
#include "core/Object.hh"

#include "util/Util.hh"

#include "mock/MockFile.hh"
#include "mock/Assert.hh"

#include <iostream>

namespace pdfut {

using namespace pdf ;

/**	constructor
	
*/
NameTreeTest::NameTreeTest( )
{
}

void NameTreeTest::TestSimple( )
{
	MockFile file ;
	Object name[] = { "v1", 100, "v2", 101.0 } ;

	DictReader self ;
	self["Names"] = Array( Begin(name), End(name) ) ;

	NameTree subject;
	subject.Read( self, &file ) ;
	PDFUT_ASSERT_EQUAL( subject.LookUp( "v1" ), Object(100) ) ;
	PDFUT_ASSERT_EQUAL( subject.LookUp( "v2" ), Object(101.0) ) ; 
	PDFUT_ASSERT_EQUAL( subject.LookUp( "v3" ), Object::NullObj() ) ;
}

void NameTreeTest::TestWrite( )
{
	MockFile file ;
	NameTree subject ;
	subject.Add( "nestal", 100.1 ) ;
	subject.Add( "ada", 110.1 ) ;
	
	Object limit[] = { "ada", "nestal" } ;
	Object names[] = { "ada", 110.1, "nestal", 100.1 } ;
	
	Ref r = subject.Write( &file ) ; 
	Dictionary dict = file.Find( r ).As<Dictionary>() ;
	PDFUT_ASSERT_EQUAL( dict["Limits"], Array(Begin(limit), End(limit) ) ) ; 
	PDFUT_ASSERT_EQUAL( dict["Names"],  Array(Begin(names), End(names) ) ) ; 
}

} // end of namespace
