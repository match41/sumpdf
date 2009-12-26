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
	\file	OutStreamBufAdaptorTest.cc
	\brief	implementation the StreamBufAdaptorTest class
	\date	Wed Mar 4 2009
	\author	Nestal Wan
*/

#include "InStreamBufAdaptorTest.hh"

#include "stream/InStreamBufAdaptor.hh"
#include "stream/DeflateFilter.hh"
#include "stream/RawFilter.hh"
#include "core/Dictionary.hh"
#include "core/Token.hh"

#include <iostream>

InStreamBufAdaptorTest::InStreamBufAdaptorTest( )
{
}

void InStreamBufAdaptorTest::TestRead( )
{
	std::ifstream file( (std::string(TEST_DATA_DIR) + "obj9020").c_str() ) ;
	std::vector<unsigned char> src( (std::istreambuf_iterator<char>( file )),
	                                (std::istreambuf_iterator<char>( )) ) ;

	std::vector<unsigned char> c( ::compressBound( src.size() ) ) ;
	::uLongf dest_len = c.size( ) ;
	::compress2( &c[0], &dest_len, &src[0], src.size(), 9 ) ;

	std::istringstream ss( std::string( &c[0], &c[dest_len] ) ) ;
	pdf::RawFilter *raw = new pdf::RawFilter( ss.rdbuf() ) ;
	pdf::DeflateFilter def( raw ) ;

	pdf::InStreamBufAdaptor subject( &def ) ;
	std::istream is( &subject ) ;
	
	pdf::Dictionary d ;
	CPPUNIT_ASSERT( is >> d ) ;
	CPPUNIT_ASSERT( d["Subtype"].As<pdf::Name>() == pdf::Name("CIDFontType0"));
	CPPUNIT_ASSERT( d["Type"].As<pdf::Name>() == pdf::Name("Font"));
}
