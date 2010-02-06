/***************************************************************************
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
 ***************************************************************************/

/*!
	\file	StreamTest.cc
	\brief	implementation the StreamTest class
	\date	Tue Apr 1 2008
	\author	Nestal Wan
*/

#include "StreamTest.hh"

#include "core/Dictionary.hh"
#include "core/Token.hh"
#include "core/TokenSrc.hh"

#include "stream/Stream.hh"

#include "mock/Assert.hh"

#include <zlib.h>

#include <cstring>
#include <sstream>
#include <iostream>

StreamTest::StreamTest( )
{
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
	
	std::istream is( subject.InStreamBuf() ) ;
	
	CPPUNIT_ASSERT( is >> t ) ;
	CPPUNIT_ASSERT( t.Get() == "hello" ) ;
}

void StreamTest::TestWrite( )
{
	std::string str = "0 12 TD (string string) Tj" ;
	pdf::Stream subject( str ) ;
	CPPUNIT_ASSERT( subject.Length( ) == str.size() ) ;	
	
	std::stringstream ss ;
	CPPUNIT_ASSERT( ss << subject ) ;

	pdf::Dictionary dict ;
	ss >> dict ;
	CPPUNIT_ASSERT( dict["Length"] == str.size() ) ;
}

void StreamTest::TestWrite2( )
{
	pdf::Stream subject ;
	std::ostream os( subject.OutStreamBuf() ) ;
	
	char text[] = "libpdfdoc test" ;
	os << text ;
	os.flush() ;
	
	unsigned char out[80] ;
	CPPUNIT_ASSERT( subject.CopyData( out, sizeof(out) ) == sizeof(text)-1 ) ;
	CPPUNIT_ASSERT( std::memcmp( text, out, sizeof(text)-1 ) == 0 ) ;
}

void StreamTest::TestReset( )
{
	const std::string str = "0 12 TD (string string) Tj" ;
	pdf::Stream subject( str ) ;
	
	std::ostringstream ss ; 
	std::size_t count = subject.CopyData( ss.rdbuf() ) ;
	CPPUNIT_ASSERT( count == str.size() ) ;
 	CPPUNIT_ASSERT( ss.str() == str ) ;
 	CPPUNIT_ASSERT( subject.Length( ) == str.size( ) ) ;
	
	// Reset() should not affect Length()
	subject.Rewind( ) ;
 	CPPUNIT_ASSERT( subject.Length( ) == str.size( ) ) ;

 	std::ostringstream ss2 ; 
	count = subject.CopyData( ss2.rdbuf() ) ;
	CPPUNIT_ASSERT( count == str.size() ) ;
 	CPPUNIT_ASSERT( ss2.str() == str ) ;
}

void StreamTest::TestReadDeflate( )
{
	const std::string str( m_compressed.begin(), m_compressed.end() ) ;
	pdf::Stream subject( m_compressed, pdf::Name( "FlateDecode" ) ) ;

	CPPUNIT_ASSERT( subject.Length( ) == str.size( ) ) ;
	
	std::ostringstream ss ; 
	std::size_t count = subject.CopyData( ss.rdbuf() ) ;
	CPPUNIT_ASSERT( count == m_original.size() ) ;
	CPPUNIT_ASSERT( subject.Length() == str.size( ) ) ;
	
	subject.Rewind( ) ;
	
	std::istream is( subject.InStreamBuf() ) ;

	pdf::Dictionary d ;
	CPPUNIT_ASSERT( is >> d ) ;
	CPPUNIT_ASSERT( d["Subtype"].As<pdf::Name>() == "CIDFontType0" ) ;
}

void StreamTest::TestClone( )
{
	const std::string str( m_compressed.begin(), m_compressed.end() ) ;
	pdf::Stream subject( m_compressed, pdf::Name( "FlateDecode" ) ) ;

	pdf::Stream cloned = subject.Clone( ) ;
	CPPUNIT_ASSERT( subject != cloned ) ;
	CPPUNIT_ASSERT( subject.IsContentEqual( cloned ) ) ;
	CPPUNIT_ASSERT( cloned.Self()["Filter"] == pdf::Object::NullObj() ) ;
}

void StreamTest::TestWriteOstream( )
{
	const char str[] = "0 12 TD (string string) Tj" ;
	pdf::Stream subject( str ) ;
	CPPUNIT_ASSERT( subject.Length() == std::strlen( str ) ) ;
	
	std::stringstream ss ;
	CPPUNIT_ASSERT( ss << subject ) ;
	CPPUNIT_ASSERT( subject.Length() > 0 ) ;

	pdf::TokenSrc src( ss ) ;

	// stream dictionary
	pdf::Object self ;
	CPPUNIT_ASSERT( src >> self ) ;
	CPPUNIT_ASSERT( self == subject.Self() ) ;

	// "stream" keyword after dictionary
	pdf::Token t ;
	CPPUNIT_ASSERT( src >> t ) ;
	CPPUNIT_ASSERT( t.Get() == "stream" ) ;

	// stream content after keyword
	char ch ;
	CPPUNIT_ASSERT( ss.get( ch ) ) ;
	CPPUNIT_ASSERT( ch == '\n' ) ;
	char out[sizeof(str)] = {} ;
	CPPUNIT_ASSERT( ss.rdbuf()->sgetn( out, sizeof(out)- 1 ) ==
		static_cast<std::streamsize>(subject.Length() ) ) ;
	CPPUNIT_ASSERT( std::memcmp( str, out, subject.Length() ) == 0 ) ;
	
	CPPUNIT_ASSERT( ss.get( ch ) ) ;
	CPPUNIT_ASSERT( ch == '\n' ) ;
	CPPUNIT_ASSERT( src >> t ) ;
	CPPUNIT_ASSERT( t.Get() == "endstream" ) ;
}

void StreamTest::TestWriteOstreamTwice( )
{
	const char str[] = "0 12 TD (string string) Tj" ;
	pdf::Stream subject( str ) ;

	std::ostringstream ss1 ;
	CPPUNIT_ASSERT( ss1 << subject ) ;
	CPPUNIT_ASSERT( subject.Length() > 0 ) ;
	
	std::ostringstream ss2 ;
	CPPUNIT_ASSERT( ss2 << subject ) ;
	CPPUNIT_ASSERT( subject.Length() > 0 ) ;
	
	CPPUNIT_ASSERT( ss1.str() == ss2.str() ) ;
}

void StreamTest::TestName( )
{
	pdf::Stream s( pdf::Stream::deflate ) ;
	CPPUNIT_ASSERT( s.Self()["Filter"].As<pdf::Name>()
		== pdf::Name("FlateDecode") ) ;
}

void StreamTest::TestWriteDeflate( )
{
	pdf::Stream s( pdf::Stream::deflate ) ;
	
	// at least 20 bytes of test data
	unsigned char text[] =
		"some long long repeatable some long long repeatable some text" ;
	CPPUNIT_ASSERT( sizeof(text) > 20 ) ;
	
	// write the text data to a stream
	s.Append( text, 20 ) ;
	s.Append( text + 20, sizeof(text) - 20  ) ;
	s.Flush() ;

	// data is compressed, so it is smaller now.
	CPPUNIT_ASSERT( s.Length() < sizeof(text) ) ;
	
	// read them back
	unsigned char out[sizeof(text)*2] ;
	s.CopyData( out, sizeof(out) ) ;
	
	// compare!
	CPPUNIT_ASSERT( std::memcmp( text, out, sizeof(text) ) == 0 ) ;
}

void StreamTest::TestEqual( )
{
	pdf::Stream
		comp( m_compressed, pdf::Name( "FlateDecode" ) ),
		original( m_original, pdf::Object() ) ;

	CPPUNIT_ASSERT( comp.IsContentEqual( original ) ) ;
}

void StreamTest::TestFlush( )
{
	pdf::Stream s( pdf::Stream::deflate ) ;
	unsigned char text[] = "abc" ;
	s.Append( text, sizeof(text) ) ;
	s.Flush() ;
	
	std::size_t len = s.Length() ;
	s.Flush() ;
	PDFUT_ASSERT_EQUAL( len, s.Length() ) ;
}
