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

/**	\file	ElementPoolTest.cc
	\brief	implementation of the ElementPoolTest class
	\date	Feb 6, 2010
	\author	Nestal Wan
*/

#include "ElementPoolTest.hh"

#include "file/ElementPool.hh"
#include "util/RefCounter.hh"

#include "mock/Assert.hh"

namespace pdfut {

using namespace pdf ;

namespace
{
	template <typename Tag>
	struct TestElement : public RefCounter
	{
	} ;
	
	struct Tag1 {} ;
	struct Tag2 {} ;
	
	typedef TestElement<Tag1> Telement1 ;
	typedef TestElement<Tag2> Telement2 ;
}

ElementPoolTest::ElementPoolTest( )
{
}

void ElementPoolTest::setUp( )
{
}

void ElementPoolTest::tearDown( )
{
}

void ElementPoolTest::TestAddAcquire( )
{
	ElementPool subject ;
	
	Telement1 *rc1 = new Telement1 ;
	subject.Add( Ref(100,0), rc1 ) ;
	
	PDFUT_ASSERT_EQUAL( subject.Find<Telement1>( Ref(100,0) ), rc1 ) ;
	PDFUT_ASSERT_EQUAL( rc1->UseCount(), 1U ) ;
	PDFUT_ASSERT_EQUAL( subject.Find( rc1 ), Ref(100,0) ) ;
	CPPUNIT_ASSERT( subject.Has( Ref(100,0) ) ) ;
	CPPUNIT_ASSERT( !subject.Has( Ref(101,0) ) ) ;
	
	Telement1 *rc2 = new Telement1 ;
	CPPUNIT_ASSERT( subject.Acquire( Ref(100,0), rc2 ) ) ;
	PDFUT_ASSERT_EQUAL( rc2, rc1 ) ;
	PDFUT_ASSERT_EQUAL( rc1->UseCount(), 2U ) ;
	
	subject.Clear( ) ;
	CPPUNIT_ASSERT( !subject.Has( Ref(100,0) ) ) ;
	PDFUT_ASSERT_EQUAL( rc1->UseCount(), 2U ) ;
	
	rc1->Release( ) ;
	rc1->Release( ) ;
}

void ElementPoolTest::TestAcquireFail( )
{
	ElementPool subject ;
	
	Telement1 *rc1 = new Telement1 ;
	subject.Add( Ref(100,0), rc1 ) ;
	
	Telement2 *rc2 = 0 ;
	CPPUNIT_ASSERT_THROW( subject.Acquire( Ref(100,0), rc2 ), std::bad_cast ) ;
	CPPUNIT_ASSERT_THROW( subject.Find<Telement2>( Ref(100,0) ), std::bad_cast ) ;
}

} // end of namespace
