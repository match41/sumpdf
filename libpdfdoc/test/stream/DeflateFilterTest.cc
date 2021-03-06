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
	\file	DeflateFilterTest.cc
	\brief	implementation the DeflateFilterTest class
	\date	Wed Mar 4 2009
	\author	Nestal Wan
*/

#include "DeflateFilterTest.hh"

#include "core/Object.hh"
#include "stream/DeflateFilter.hh"
#include "stream/RawFilter.hh"
#include "stream/BufferedFilter.hh"

#include "mock/Assert.hh"

#include <vector>
#include <fstream>
#include <iterator>
#include <cstring>

#include <zlib.h>

typedef std::auto_ptr<pdf::StreamFilter> StreamFilterPtr ;

DeflateFilterTest::DeflateFilterTest( )
{
}

void DeflateFilterTest::TestRead( )
{
	std::istringstream ss( std::string( m_compressed.begin(),
	                                    m_compressed.end() ) ) ;
	
	// put some data at the end
	char enddat[] = "Hello!" ;
	ss.rdbuf()->sputn( enddat, sizeof(enddat) ) ;
	
	pdf::RawFilter *raw = new pdf::RawFilter( ss.rdbuf() ) ;
	pdf::DeflateFilter subject(( StreamFilterPtr(raw) )) ;
	
	std::vector<unsigned char> src = m_original ;
	
	unsigned char buf[200] ;
	std::size_t count = subject.Read( buf, sizeof( buf ) ) ;
	std::size_t total = count ;
	
	while ( count != 0 )
	{
		CPPUNIT_ASSERT( std::equal( buf, buf + count, src.begin() ) ) ;
		src.erase( src.begin( ), src.begin() + count ) ;
		
		count = subject.Read( buf, sizeof( buf ) - 1 ) ;
		total += count ;
	}
	
	CPPUNIT_ASSERT( total == m_original.size() ) ;
}

void DeflateFilterTest::TestReset( )
{
	std::istringstream ss( std::string( m_compressed.begin(),
	                                    m_compressed.end() ) ) ;
	pdf::RawFilter *raw = new pdf::RawFilter( ss.rdbuf() ) ;
	pdf::DeflateFilter subject(( StreamFilterPtr(raw) )) ;
	
	std::vector<unsigned char> src = m_original ;
	
	unsigned char buf[100] ;
	std::size_t count = subject.Read( buf, sizeof( buf ) ) ;
	subject.Rewind( ) ;	
	
	count = subject.Read( buf, sizeof( buf ) ) ;
	while ( count != 0 )
	{
		CPPUNIT_ASSERT( std::equal( buf, buf + count, src.begin() ) ) ;
		src.erase( src.begin( ), src.begin() + count ) ;
		
		count = subject.Read( buf, sizeof( buf ) - 1 ) ;
	}
}

void DeflateFilterTest::TestName( )
{
	std::istringstream ss( std::string( m_compressed.begin(),
	                                    m_compressed.end() ) ) ;
	pdf::RawFilter *raw = new pdf::RawFilter( ss.rdbuf() ) ;
	pdf::DeflateFilter subject(( StreamFilterPtr(raw) )) ;
	CPPUNIT_ASSERT( subject.NameChain().As<pdf::Name>()
		== pdf::Name( "FlateDecode" ) ) ;
}

void DeflateFilterTest::TestWrite( )
{
	pdf::BufferedFilter *raw = new pdf::BufferedFilter ;
	pdf::DeflateFilter subject(( StreamFilterPtr(raw) )) ;

	// compress some data
	unsigned char text[] = "HelloHello!" ;
	subject.Write( text, sizeof(text) ) ;
	subject.Flush( ) ;
	CPPUNIT_ASSERT( raw->Length() > 0 ) ;
	
	// read the compress data back
	std::vector<unsigned char> out( raw->Length() ) ;
	CPPUNIT_ASSERT( raw->Read( &out[0], out.size() ) == out.size() ) ;

	// uncompress it and check the result
	unsigned char rev[sizeof(text)*2] ;
	unsigned long rsize = sizeof(rev) ;
	CPPUNIT_ASSERT( ::uncompress( rev, &rsize, &out[0], out.size() ) == Z_OK ) ;
	CPPUNIT_ASSERT( rsize == sizeof(text) ) ;
	CPPUNIT_ASSERT( std::memcmp( rev, text, rsize ) == 0 ) ;
}
/*
void DeflateFilterTest::TestUnknownLength( )
{
	unsigned char src[] =
	{
		0x78, 0x9C, 0x63, 0x64, 0x40, 0x02, 0x8C, 0x44, 0x72, 0xFE, 0xFF,
		0xFF, 0x4F, 0x2A, 0x07, 0x00, 0x6E, 0xDD, 0x0B, 0xFD
	} ;

	unsigned long srclen = sizeof(src), destlen = 0 ;
	int r = puff( 0, &destlen, src, &srclen ) ;
	std::cout << "puff = " << r << " " << destlen << std::endl ;
}
*/

void DeflateFilterTest::TestExcessData( )
{
	unsigned char src[1000] =
	{
		0x78, 0x9C, 0x63, 0xFC, 0xFF, 0xFF, 0x3F, 0x03, 0x03, 0x03, 0x23, 0x23,
		0x23, 0x03, 0x0C, 0x40, 0x45, 0xA8, 0x27, 0x01, 0x00, 0xD3, 0x54, 0x17,
		0xF9,
	} ;
	
	pdf::BufferedFilter *raw = new pdf::BufferedFilter(src, src + sizeof(src)) ;
	pdf::DeflateFilter subject(( StreamFilterPtr(raw) )) ;

	unsigned char buf[1000] ;
	std::size_t count = subject.Read( buf, sizeof( buf ) ) ;
	PDFUT_ASSERT_EQUAL( count, 100UL ) ;
}

void DeflateFilterTest::setUp( )
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

void DeflateFilterTest::tearDown( )
{
	m_compressed.clear( ) ;
}
