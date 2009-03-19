/***************************************************************************
 *   Copyright (C) 2006 by Nestal Wan                                      *
 *   me@nestal.net                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
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
 ***************************************************************************/

/*!
	\file	ElementTrackerTest.cc
	\brief	implementation the ElementTrackerTest class
	\date	Sat Apr 19 2008
	\author	Nestal Wan
*/

#include "ElementTrackerTest.hh"

#include "file/ElementTracker.hh"
#include "file/ElementList.hh"
#include "file/IElement.hh"

#include <boost/lambda/lambda.hpp>

#include <algorithm>

namespace
{
	class MockElement : public pdf::IElement
	{
	public :
		MockElement( )
		{
		}
	
		template <typename InputIt>
		MockElement( InputIt first, InputIt last )
			: m_child( first, last )
		{
		}
	
		void Read( const pdf::Ref& link, pdf::IElementSrc *repo )
		{
		}
		
		void Write( const pdf::Ref& link, pdf::IElementDest *repo ) const
		{
		}
		
		pdf::ElementList GetChildren( ) const
		{
			return m_child ;
		}
	
	private :
		pdf::ElementList m_child ;
	} ;
}


ElementTrackerTest::ElementTrackerTest( )
{
}

void ElementTrackerTest::TestSimple( )
{
	MockElement a, b, c, d, e ;
	
	pdf::ElementTracker subject ;
	
	subject.Add( &a ) ;
	subject.Add( &b ) ;
	subject.Add( &c ) ;
	subject.Add( &d ) ;
	subject.Add( &e ) ;
	
	// add a again
	subject.Add( &a ) ;

	pdf::ElementList r ;
	subject.Get( r ) ;
	CPPUNIT_ASSERT( r.size() == 5 ) ;
	
	// same result for second call.
	// make sure it won't accumulate
	subject.Get( r ) ;
	CPPUNIT_ASSERT( r.size() == 5 ) ;

	pdf::IElement* other[] = { &b, &c, &d, &e, &a } ;
	
	CPPUNIT_ASSERT( std::equal( r.begin( ), r.end( ), other ) ) ;
}

void ElementTrackerTest::TestComposite( )
{
	MockElement a, b[2], *pb[] = { &a, &b[0], &b[1] } ;
	MockElement big( pb, pb + 3 ) ;

	pdf::ElementTracker subject ;
	subject.Traverse( &big ) ;

	pdf::ElementList r ;
	subject.Get( r ) ;
	CPPUNIT_ASSERT( r.size() == 4 ) ;
	
	pdf::IElement* other[] = { &big, &a, &b[0], &b[1] } ;
	CPPUNIT_ASSERT( std::equal( r.begin( ), r.end( ), other ) ) ;
}
