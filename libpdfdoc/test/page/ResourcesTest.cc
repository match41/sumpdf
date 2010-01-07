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

#include "ResourcesTest.hh"

#include "mock/MockFile.hh"

#include "core/Ref.hh"
#include "core/Dictionary.hh"
#include "core/Ref.hh"
#include "font/StandardFont.hh"
#include "page/Resources.hh"
#include "util/Rect.hh"

#include "mock/Assert.hh"

#include <sstream>

ResourcesTest::ResourcesTest( )
{
}

void ResourcesTest::TestNormal( )
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
	pdf::Resources subject ;
	subject.Read( rdict, &file ) ;
}

void ResourcesTest::TestReadExistFont( )
{
	std::istringstream iss( "<< /Font << /F0 18 0 R >> /ProcSet [/PDF /Text]\n"
	                        "/XObject << >> >>" ) ;
	pdf::Dictionary rdict ;
	CPPUNIT_ASSERT( iss >> rdict ) ;

	pdf::StandardFont *f = new pdf::StandardFont( "Times-Roman" ) ;
	PDF_ASSERT_EQUAL( f->UseCount(), 1u ) ;

	MockFile file ;
	file.AddObj( pdf::Ref(1,0),  rdict ) ;

	file.Pool()->fonts.Add( pdf::Ref(18, 0 ), f ) ;

	pdf::Object obj( rdict ) ;
	pdf::Resources subject ;
	subject.Read( rdict, &file ) ;
	PDF_ASSERT_EQUAL( f->UseCount(), 2u ) ;
}
