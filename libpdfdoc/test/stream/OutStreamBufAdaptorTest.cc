/***************************************************************************\
 *   Copyright (C) 2009 by Nestal Wan                                      *
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
 \***************************************************************************/

/**
 \file	OutStreamBufAdaptorTest.cc
 \brief	definition the OutStreamBufAdaptorTest class
 \date	Dec 22, 2009
 \author	nestal
 */

#include "OutStreamBufAdaptorTest.hh"

#include "stream/OutStreamBufAdaptor.hh"
#include "stream/BufferedFilter.hh"

#include <ostream>
#include <cstring>

OutStreamBufAdaptorTest::OutStreamBufAdaptorTest( )
{
	// TODO Auto-generated constructor stub

}

void OutStreamBufAdaptorTest::TestWrite( )
{
	pdf::BufferedFilter f ;
	pdf::OutStreamBufAdaptor subject( &f ) ;
	std::ostream os( &subject ) ;
	
	// sizeof(str) == strlen(str) + 1 (null)
	const char str[] = "Hello world!" ;  
	os << str << std::endl ;
	
	unsigned char buf[80] ;
	CPPUNIT_ASSERT( f.Read( buf, sizeof(buf) ) == sizeof(str) ) ;
	CPPUNIT_ASSERT( std::memcmp( buf, str, sizeof(str)-1 ) == 0 ) ;
	CPPUNIT_ASSERT( buf[sizeof(str)-1] == '\n' ) ; 
}

