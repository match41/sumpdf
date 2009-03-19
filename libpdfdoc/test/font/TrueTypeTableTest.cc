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
	\file	TrueTypeTableTest.cc
	\brief	implementation the TrueTypeTableTest class
	\date	Sat Mar 7 2009
	\author	Nestal Wan
*/

#include "TrueTypeTableTest.hh"

#include "font/TrueTypeTable.hh"
#include "util/Util.hh"

#include <sstream>
#include <fstream>

TrueTypeTableTest::TrueTypeTableTest( )
{
}

void TrueTypeTableTest::TestWrite( )
{
	std::ostringstream oss ;
	font::int32_t val = 0x12345678 ;
	font::TrueTypeTable subject( oss.rdbuf() ) ;
	subject << val ;
	std::string out = oss.str() ;
	CPPUNIT_ASSERT( out[0] == 0x12 ) ;
	CPPUNIT_ASSERT( out[1] == 0x34 ) ;
	CPPUNIT_ASSERT( out[2] == 0x56 ) ;
	CPPUNIT_ASSERT( out[3] == 0x78 ) ;
}

void TrueTypeTableTest::TestRead( )
{
	char in[] = { 0x78, 0x56, 0x34, 0x12, 0xab, 0xcd, 0xef, 0xff } ;
	std::istringstream iss( std::string( pdf::Begin(in), pdf::End(in) ) ) ;
	
	font::TrueTypeTable subject( iss.rdbuf() ) ;
	
	font::int64_t val ;
	subject >> val ;
	
	CPPUNIT_ASSERT( val == 0x78563412abcdefff ) ;
}

void TrueTypeTableTest::TestChecksum( )
{
	std::ifstream infile( (std::string(TEST_DATA_DIR) +
	                       "FreeMonoBoldOblique.ttf").c_str(),
	                      std::ios::in | std::ios::binary ) ;
	infile.rdbuf()->pubseekpos( 39900 ) ;
	
	std::ostringstream oss ;
	font::TrueTypeTable subject( oss.rdbuf() ) ;
	CPPUNIT_ASSERT( subject.Copy( infile.rdbuf(), 81174 ) ) ;
	CPPUNIT_ASSERT( subject.Size() == 79924 ) ;
	CPPUNIT_ASSERT( oss.str().size() == 79924 ) ;
	
	// after calling EndStream(), size will be 4-byte algined.
	subject.EndStream( ) ;
	CPPUNIT_ASSERT( subject.Checksum() == 45303795 ) ;
	CPPUNIT_ASSERT( subject.Size() == 79924 ) ;
	CPPUNIT_ASSERT( oss.str().size() == 79924 ) ;
}
