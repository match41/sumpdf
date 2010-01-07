/***************************************************************************\
 *   Copyright (C) 2009 by Nestal Wan                                      *
 *   me@nestal.net                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License.        *
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

/**
	\file	RefObjMapTest.cc
	\brief	definition the RefObjMapTest class
	\date	Dec 26, 2009
	\author	Nestal Wan
 */

#include "RefObjMapTest.hh"

#include "file/RefObjMap.hh"
#include "util/RefCounter.hh"

#include "mock/Assert.hh"

namespace
{
	// RefObjMap should work with reference counted type
	struct Dummy : public pdf::RefCounter
	{
	} ;
}

RefObjMapTest::RefObjMapTest( )
{
}

void RefObjMapTest::TestSimple( )
{
	pdf::RefObjMap<Dummy> map ;
	Dummy *d = new Dummy ;
	map.Add( pdf::Ref( 100, 0 ), d ) ;
	Dummy *out = map.Find( pdf::Ref( 100, 0 ) ) ;
	PDF_ASSERT_EQUAL( out, d ) ;
	PDF_ASSERT_EQUAL( out->UseCount(), 2U ) ;
	PDF_ASSERT_EQUAL( map.Find( out ), pdf::Ref( 100, 0 ) ) ; 
}
