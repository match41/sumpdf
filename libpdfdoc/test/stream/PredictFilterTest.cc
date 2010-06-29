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

/**	\file	PredictFilterTest.cc
	\brief	implementation of the PredictFilterTest class
	\date	Jun 25, 2010
	\author	Nestal Wan
*/

#include "PredictFilterTest.hh"

#include "stream/PredictFilter.hh"
#include "stream/BufferedFilter.hh"

#include "util/Util.hh"

#include "mock/Assert.hh"

#include <iostream>
#include <cstring>

namespace pdfut {

using namespace pdf ;

PredictFilterTest::PredictFilterTest( )
{
}

void PredictFilterTest::setUp( )
{
}

void PredictFilterTest::tearDown( )
{
}

void PredictFilterTest::Test( )
{
	const unsigned char data[] =
	{
		0x02, 0x01, 0x00, 0x10, 0x00, 0x02, 0x00, 0x02, 
		0x55, 0x00, 0x02, 0x00, 0x01, 0x13, 0x00, 0x02, 
		0x00, 0x01, 0xdc, 0x00, 0x02, 0x00, 0x01, 0xb3, 
		0x00, 0x02, 0x00, 0xfb, 0x6d, 0x00, 0x02, 0x01, 
		0x00, 0xa3, 0x00, 0x02, 0x00, 0x00, 0x00, 0x01, 
		0x02, 0x00, 0x00, 0x00, 0x01, 0x02, 0xff, 0x01, 
		0xab, 0xfe, 
	} ;
	
	const unsigned char expected[] =
	{
		0x01, 0x00, 0x10, 0x00, 0x01, 0x02, 0x65, 0x00, 
		0x01, 0x03, 0x78, 0x00, 0x01, 0x04, 0x54, 0x00, 
		0x01, 0x05, 0x07, 0x00, 0x01, 0x00, 0x74, 0x00, 
		0x02, 0x00, 0x17, 0x00, 0x02, 0x00, 0x17, 0x01, 
		0x02, 0x00, 0x17, 0x02, 0x01, 0x01, 0xc2, 0x00,
	} ;
	
	std::auto_ptr<StreamFilter> src(
		new BufferedFilter( Begin(data), End(data) ) ) ;
	
	PredictFilter subject( src, 4 ) ;
	
	std::vector<unsigned char> raw( 100 ) ;
	std::size_t count = subject.Read( &raw[0], raw.size() ) ;
	PDFUT_ASSERT_EQUAL( count, 40U ) ;
	CPPUNIT_ASSERT( std::memcmp( &raw[0], expected, 40U ) == 0 ) ;
}

} // end of namespace
