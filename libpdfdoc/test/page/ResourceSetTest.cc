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

/**	\file	ResourceSetTest.cc
	\brief	implementation of the ResourceSetTest class
	\date	May 21, 2010
	\author	Nestal Wan
*/

#include "ResourceSetTest.hh"

#include "file/DictReader.hh"
#include "page/ResourceSet.hh"

#include "mock/Assert.hh"
#include "mock/MockFile.hh"
#include "mock/MockFont.hh"

#include "util/Functional.hh"

namespace pdfut {

using namespace pdf ;

ResourceSetTest::ResourceSetTest( )
{
}

void ResourceSetTest::setUp( )
{
}

void ResourceSetTest::tearDown( )
{
}

void ResourceSetTest::Test( )
{
	MockFont font ;
	MockFile file ;
	file.Pool()->Add( Ref(1,0), &font ) ;
	
	Dictionary dict ;
	dict.insert( "F1", Ref(1,0) ) ;
	
	DictReader dr( dict, &file ) ;
	
	ResourceSet<BaseFont> subject( "F" ) ;
	subject.MassProduce( dr, NewPtr<MockFont>( ) ) ;
	
	BaseFont* f = subject.Find( "F1" ) ;
	PDFUT_ASSERT_EQUAL( f, &font ) ;
	
	Name n = subject.Find( f ) ;
	PDFUT_ASSERT_EQUAL( n, "F1" ) ;
}

} // end of namespace
