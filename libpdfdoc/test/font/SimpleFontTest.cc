/***************************************************************************\
 *   Copyright (C) 2009 by Nestal Wan                                      *
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

/*!
	\file	SimpleFontTest.cc
	\brief	implementation the SimpleFontTest class
	\date	Sun Mar 8 2009
	\author	Nestal Wan
*/

#include "SimpleFontTest.hh"

#include "font/SimpleFont.hh"
#include "font/FontDescriptor.hh"

#include "mock/Assert.hh"
#include "mock/MockFile.hh"
#include "mock/MockFontDb.hh"

#include <sstream>
#include <iostream>

namespace pdfut {

using namespace pdf ;

SimpleFontTest::SimpleFontTest( )
{
}

void SimpleFontTest::TestLoadByName( )
{
	pdf::SimpleFont subject( "FreeMonoBoldOblique", m_font_db ) ;
	PDFUT_ASSERT_EQUAL( subject.BaseName( ), "FreeMonoBoldOblique" ) ;
	CPPUNIT_ASSERT( !subject.IsSubset() ) ;
	
	pdf::FontDescriptor *fd = subject.Descriptor( ) ;
	CPPUNIT_ASSERT( fd != 0 ) ;
	
	MockFile file ;
	pdf::Ref r = subject.Write( &file, 0 ) ;
	
	pdf::Object fdo = file.ReadObj( r ) ;
	CPPUNIT_ASSERT( fdo.Is<pdf::Dictionary>() ) ;
	PDFUT_ASSERT_EQUAL( fd->Family(), "" ) ; 

std::cout << "dict = " << fdo << std::endl ;	
}

void SimpleFontTest::TestParseBold( )
{
	SimpleFont subject( "Helvetica-Bold", m_font_db ) ;
	PDFUT_ASSERT_EQUAL( m_mock_fdb->LastQueryName(), "Arial" ) ;
	PDFUT_ASSERT_EQUAL( m_mock_fdb->LastQueryWeight(), font::bold ) ;

	SimpleFont subject2( "Helvetica-BoldOblique", m_font_db ) ;
	PDFUT_ASSERT_EQUAL( m_mock_fdb->LastQueryName(), "Arial" ) ;
	PDFUT_ASSERT_EQUAL( m_mock_fdb->LastQueryWeight(), font::bold ) ;
}

} // end of namespace
