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
	\file	TokenTest.cc
	\brief	implementation the TokenTest class
	\date	Sun Mar 9 2008
	\author	Nestal Wan
*/

#include "TokenTest.hh"

#include "core/Token.hh"
#include "util/Util.hh"

#include <algorithm>
#include <iterator>
#include <sstream>
#include <vector>

TokenTest::TokenTest( )
{
}

void TokenTest::TestAllNumbers( )
{
	std::istringstream ss( "1 2 3 4 5 6 7 89" ) ;
	std::vector<pdf::Token> vec( (std::istream_iterator<pdf::Token>( ss )),
	                             (std::istream_iterator<pdf::Token>( )) ) ;

	pdf::Token result[] =
	{
		pdf::Token( "1" ),
		pdf::Token( " " ),
		pdf::Token( "2" ),
		pdf::Token( " " ),
		pdf::Token( "3" ),
		pdf::Token( " " ),
		pdf::Token( "4" ),
		pdf::Token( " " ),
		pdf::Token( "5" ),
		pdf::Token( " " ),
		pdf::Token( "6" ),
		pdf::Token( " " ),
		pdf::Token( "7" ),
		pdf::Token( " " ),
		pdf::Token( "89" ),
	} ;
	
	CPPUNIT_ASSERT( std::equal( vec.begin( ), vec.end( ), result ) ) ;
	CPPUNIT_ASSERT( vec.size( ) == pdf::Count( result ) ) ;
}

void TokenTest::TestAllDelimitor( )
{
	std::istringstream ss( "<< [ { % > ] [" ) ;
	std::vector<pdf::Token> vec( (std::istream_iterator<pdf::Token>( ss )),
	                             (std::istream_iterator<pdf::Token>( )) ) ;

	pdf::Token result[] =
	{
		pdf::Token( "<<" ),
		pdf::Token( " " ),
		pdf::Token( "[" ),
		pdf::Token( " " ),
		pdf::Token( "{" ),
		pdf::Token( " " ),
		pdf::Token( "%" ),
		pdf::Token( " " ),
		pdf::Token( ">" ),
		pdf::Token( " " ),
		pdf::Token( "]" ),
		pdf::Token( " " ),
		pdf::Token( "[" ),
	} ;
	
	CPPUNIT_ASSERT( std::equal( vec.begin( ), vec.end( ), result ) ) ;
	CPPUNIT_ASSERT( vec.size( ) == pdf::Count( result ) ) ;
}

void TokenTest::TestMixChar( )
{
	std::istringstream ss( "<< /Name 1/Add [192 168 0 1]>> (string )" ) ;
	std::vector<pdf::Token> vec( (std::istream_iterator<pdf::Token>( ss )),
	                             (std::istream_iterator<pdf::Token>( )) ) ;

	pdf::Token result[] =
	{
		pdf::Token( "<<" ),
		pdf::Token( " " ),
		pdf::Token( "/" ),
		pdf::Token( "Name" ),
		pdf::Token( " " ),
		pdf::Token( "1" ),
		pdf::Token( "/" ),
		pdf::Token( "Add" ),
		pdf::Token( " " ),
		pdf::Token( "[" ),
		pdf::Token( "192" ),
		pdf::Token( " " ),
		pdf::Token( "168" ),
		pdf::Token( " " ),
		pdf::Token( "0" ),
		pdf::Token( " " ),
		pdf::Token( "1" ),
		pdf::Token( "]" ),
		pdf::Token( ">>" ),
		pdf::Token( " " ),
		pdf::Token( "(" ),
		pdf::Token( "string" ),
		pdf::Token( " " ),
		pdf::Token( ")" ),
	} ;
	
	CPPUNIT_ASSERT( std::equal( vec.begin( ), vec.end( ), result ) ) ;
	CPPUNIT_ASSERT( vec.size( ) == pdf::Count( result ) ) ;
}
