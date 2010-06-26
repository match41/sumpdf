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
#include <iomanip>

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
	std::auto_ptr<StreamFilter> src(
		new BufferedFilter( Begin(data), End(data) ) ) ;
	
	PredictFilter subject( src, 4 ) ;
	
	std::vector<unsigned char> raw( 4 ) ;
	subject.Read( &raw[0], raw.size() ) ;
	
	for ( std::size_t i = 0 ; i < raw.size() ; i++ )
	{
		if ( i % 8 == 0 && i > 0 )
			std::cout << std::endl ;
		std::cout << "0x" << std::hex << std::setw(2) << std::setfill('0') 
			<< (int)raw[i] << ", " ;
	}
	std::cout << std::endl ;

}

} // end of namespace
