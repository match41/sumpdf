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

/**	\file	DictReaderTest.cc
	\brief	implementation of the DictReaderTest class
	\date	Feb 16, 2010
	\author	Nestal Wan
*/

#include "DictReaderTest.hh"

#include "core/Dictionary.hh"
#include "file/DictReader.hh"

#include "mock/Assert.hh"
#include "mock/MockFile.hh"

namespace pdfut {

using namespace pdf ;

DictReaderTest::DictReaderTest( )
{
}

void DictReaderTest::setUp( )
{
}

void DictReaderTest::tearDown( )
{
}

void DictReaderTest::TestIntToDouble( )
{
	Dictionary dict ;
	dict["Test"] = 100 ;
	
	MockFile file ;
	DictReader subject( dict, &file ) ;
	
	double out ;
	CPPUNIT_ASSERT( subject.Detach( "Test", out ) ) ;
	PDFUT_ASSERT_EQUAL( out, 100.0 ) ;
}

} // end of namespace
