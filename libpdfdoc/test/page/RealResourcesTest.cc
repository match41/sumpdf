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
	\file	ResourcesTest.cc
	\brief	implementation the ResourcesTest class
	\date	Sat May 10 2008
	\author	Nestal Wan
*/

#include "RealResourcesTest.hh"

#include "mock/MockFile.hh"
#include "mock/MockFont.hh"

#include "core/Ref.hh"
#include "core/Dictionary.hh"
#include "core/Ref.hh"
#include "file/DictReader.hh"
#include "page/RealResources.hh"
#include "util/Rect.hh"

#include "mock/Assert.hh"

#include <sstream>

namespace pdfut {

using namespace pdf ;

RealResourcesTest::RealResourcesTest( )
{
}

void RealResourcesTest::TestNormal( )
{
	std::istringstream iss( "<< /Font << /F0 18 0 R >> /ProcSet [/PDF /Text]\n"
	                        "/XObject << >> >>" ) ;
	pdf::Dictionary rdict ;
	CPPUNIT_ASSERT( iss >> rdict ) ;

	std::istringstream fss( "<</BaseFont /Helvetica-Bold\n"
	                        "/Encoding /WinAnsiEncoding\n"
	                        "/FirstChar 0\n"
	                        "/LastChar 0\n"
	                        "/Subtype /Type1\n"
	                        "/Type /Font\n"
	                        ">>" ) ;
	pdf::Dictionary fd ;
	CPPUNIT_ASSERT( fss >> fd ) ;

	MockFile file ;
	file.AddObj( pdf::Ref(1,0),  rdict ) ;
	file.AddObj( pdf::Ref(18,0), fd ) ;

	pdf::Object obj( rdict ) ;
	pdf::RealResources subject( m_font_db ) ;
	
	DictReader reader( rdict, &file ) ;
	subject.Read( reader ) ;
	
	BaseFont* f = subject.FindFont( Name("F0") ) ;
	CPPUNIT_ASSERT( f != 0 ) ;
	PDFUT_ASSERT_EQUAL( f->BaseName(), "Helvetica-Bold" ) ;
}

void RealResourcesTest::TestReadExistFont( )
{
	std::istringstream iss( "<< /Font << /F0 18 0 R >> /ProcSet [/PDF /Text]\n"
	                        "/XObject << >> >>" ) ;
	pdf::Dictionary rdict ;
	CPPUNIT_ASSERT( iss >> rdict ) ;

	MockFont *f = new MockFont ;
	PDFUT_ASSERT_EQUAL( f->UseCount(), 1u ) ;

	MockFile file ;
	file.AddObj( pdf::Ref(1,0),  rdict ) ;
	file.Pool()->Add( pdf::Ref(18, 0 ), f ) ;
	
	DictReader reader( rdict, &file ) ;

	RealResources subject( m_font_db ) ;
	subject.Read( reader ) ;
	
	PDFUT_ASSERT_EQUAL( f->UseCount(), 2u ) ;
	PDFUT_ASSERT_EQUAL( subject.FindFont( Name("F0") ), f ) ;
	PDFUT_ASSERT_EQUAL( subject.FindFont( f ), Name( "F0" ) ) ;
	
	MockFont *f2 = new MockFont ;
	Name name = subject.AddFont( f2 ) ;
	CPPUNIT_ASSERT( name != Name("F0") ) ;
	
	f->Release() ;
}

void RealResourcesTest::TestReadExistState( )
{
}

} // end of namespace
