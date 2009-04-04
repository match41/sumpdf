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
	\file	StreamTest.cc
	\brief	implementation the StreamTest class
	\date	Tue Apr 1 2008
	\author	Nestal Wan
*/

#include "StreamTest.hh"

#include "core/Dictionary.hh"
#include "core/Stream.hh"
#include "core/Token.hh"
#include "core/TokenSrc.hh"

#include <zlib.h>

#include <sstream>
#include <iostream>

StreamTest::StreamTest( )
{
}

void StreamTest::TestRead( )
{
	std::istringstream oss( "<<\n/Length 45\n>>\nstream\nBT\n"
	                          "/F0 12 Tf 100 100 Td (Hello world!) Tj\n"
	                          "ET\nendstream" ) ;
	pdf::TokenSrc src( oss ) ;
	pdf::Dictionary d ;
	CPPUNIT_ASSERT( src >> d ) ;
	
	pdf::Token objstr ;
	CPPUNIT_ASSERT( src >> objstr ) ;
	CPPUNIT_ASSERT( objstr.Get() == "stream" ) ;
}

void StreamTest::TestRead2( )
{
	pdf::Dictionary d ;
	d["Length"] = 5 ;
	std::istringstream iss( "hello" ) ;
	pdf::Stream subject( iss.rdbuf(), 0, d ) ;
	pdf::Token t ;
	CPPUNIT_ASSERT( subject.InStream() >> t ) ;
	CPPUNIT_ASSERT( t.Get() == "hello" ) ;
}

void StreamTest::TestWrite( )
{
	std::string str = "0 12 TD (string string) Tj" ;
	pdf::Stream subject( str ) ;
	
	std::stringstream ss ; 
	std::size_t count = subject.Write( ss, pdf::Ref( 101, 0 ) ) ;
	CPPUNIT_ASSERT( count == str.size() ) ;

	pdf::Dictionary dict ;
	ss >> dict ;
	CPPUNIT_ASSERT( dict["Length"] == pdf::Ref( 101, 0 ) ) ;
}

void StreamTest::TestReset( )
{
	std::string str = "0 12 TD (string string) Tj" ;
	pdf::Stream subject( str ) ;
	
	std::ostringstream ss ; 
	std::size_t count = subject.Write( ss, pdf::Ref( 101, 0 ) ) ;
	CPPUNIT_ASSERT( count == str.size() ) ;
// 	CPPUNIT_ASSERT( ss.str() == str ) ;
	
	subject.Reset( ) ;
	std::ostringstream ss2 ; 
	count = subject.Write( ss2, pdf::Ref( 101, 0 ) ) ;
	CPPUNIT_ASSERT( count == str.size() ) ;
// 	CPPUNIT_ASSERT( ss2.str() == str ) ;
}

void StreamTest::TestReadDeflate( )
{
	std::string str( m_compressed.begin(), m_compressed.end() ) ;
	pdf::Stream subject( m_compressed, pdf::Name( "FlateDecode" ) ) ;
	
	std::ostringstream ss ; 
	std::size_t count = subject.Write( ss, pdf::Ref( 101, 0 ) ) ;
	CPPUNIT_ASSERT( count == m_original.size() ) ;
	
	subject.Reset( ) ;
	
	pdf::Dictionary d ;
	CPPUNIT_ASSERT( subject.InStream() >> d ) ;
	CPPUNIT_ASSERT( d["Subtype"].As<pdf::Name>() == "CIDFontType0" ) ;
}

void StreamTest::setUp( )
{
	std::ifstream file( (std::string(TEST_DATA_DIR) + "obj9020").c_str() ) ;
	m_original.assign( (std::istreambuf_iterator<char>( file )),
	                   (std::istreambuf_iterator<char>( )) ) ;

	std::vector<unsigned char> c( ::compressBound( m_original.size() ) ) ;
	::uLongf dest_len = c.size( ) ;
	::compress2( &c[0], &dest_len, &m_original[0], m_original.size(), 9 ) ;
	c.resize( dest_len ) ;
	
	m_compressed.swap( c ) ;
}

void StreamTest::tearDown( )
{
	m_compressed.clear( ) ;
}

void StreamTest::TestWriteOstream( )
{
	pdf::Stream subject( "hello world" ) ;
	pdf::Object obj( subject ) ;
	std::ostringstream ss ;
	ss << obj ;
}
