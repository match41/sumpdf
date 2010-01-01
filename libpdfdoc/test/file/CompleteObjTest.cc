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

/**
	\file	CompleteObjTest.cc
	\brief	implementation of the CompleteObjTest class
	\date	Jan 1, 2010
	\author	Nestal Wan
*/

#include "CompleteObjTest.hh"

#include "file/CompleteObj.hh"
#include "file/RealFile.hh"

#include "mock/MockFile.hh"

/**	constructor
*/
CompleteObjTest::CompleteObjTest( )
{
}

void CompleteObjTest::TestReadWrite( )
{
	using namespace pdf ;

	// open expected file to compare and verify
	std::ifstream file( (std::string(TEST_DATA_DIR) +
	                    "FileTestSimple.pdf").c_str( ),
	                    std::ios::in | std::ios::binary ) ;
	RealFile f( &file ) ;
	Dictionary root = f.ReadObj( f.Root() ) ;
	
	CompleteObj obj ;
	obj.Read( root, &f ) ;
	
	MockFile dest ;
	
	// bump the link value up by 5
	for ( int i = 0 ; i < 5 ; ++i )
		dest.AllocLink( ) ;
	
	Ref r = obj.Write( &dest ) ;
	CPPUNIT_ASSERT( r.ID() > 5 ) ;
}
