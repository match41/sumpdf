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

/**	\file	SfntTest.cc
	\brief	implementation of the SfntTest class
	\date	Apr 10, 2010
	\author	Nestal Wan
*/

#include "SfntTest.hh"

#include "font/subset/Sfnt.hh"

#include "mock/Assert.hh"

#include "util/Util.hh"

#include <fstream>
#include <iterator>

namespace pdfut {

using namespace pdf ;

const std::string SfntTest::m_font =
	TEST_DATA_DIR + std::string("FreeMonoBoldOblique.ttf") ;

SfntTest::SfntTest( )
{
}

void SfntTest::setUp( )
{
	FT_Init_FreeType( &m_ft ) ;
	FT_New_Face( m_ft, m_font.c_str(), 0, &m_face ) ;
}

void SfntTest::tearDown( )
{
	FT_Done_Face( m_face ) ;
	FT_Done_FreeType( m_ft ) ;
}

void SfntTest::TestFull( )
{
	Sfnt subject( m_face ) ;
	
	std::stringstream out ;
	subject.Write( out.rdbuf(), 0, 0 ) ;
	std::string os = out.str() ;
	CPPUNIT_ASSERT( os.size() > 0 ) ;
	
	std::ifstream font_file( m_font.c_str(), std::ios::in | std::ios::binary ) ;
	
	std::vector<char> exp(
		(std::istreambuf_iterator<char>(font_file)),
		(std::istreambuf_iterator<char>()) ) ;
	
	PDFUT_ASSERT_EQUAL( os.size(), exp.size() ) ;
	PDFUT_ASSERT_RANGE_EQUAL( os.begin(), os.end(), exp.begin() ) ;
	
	std::ofstream full( "full.ttf", std::ios::out | std::ios::binary ) ;
	subject.Write( full.rdbuf(), 0, 0 ) ;
}

void SfntTest::TestSubset( )
{
	Sfnt subject( m_face ) ;

	long glyphs[] = { 0,
		FT_Get_Char_Index( m_face, 'A' ),
		FT_Get_Char_Index( m_face, 'Z' ) } ;

	std::ofstream out( "test.ttf", std::ios::out | std::ios::binary ) ;
	subject.Write( out.rdbuf(), glyphs, Count(glyphs) ) ;
	
	// open the subset
	FT_Face sub_face ;
	FT_Error e = FT_New_Face( m_ft, "test.ttf", 0, &sub_face ) ;
	PDFUT_ASSERT_EQUAL( e, 0 ) ;
	
	// the cmap and glyph indices are the same
	PDFUT_ASSERT_EQUAL(
		FT_Get_Char_Index( m_face,		'A' ),
		FT_Get_Char_Index( sub_face,	'A' ) ) ;
	PDFUT_ASSERT_EQUAL(
		FT_Get_Char_Index( m_face,		'Z' ),
		FT_Get_Char_Index( sub_face,	'Z' ) ) ;
}

} // end of namespace
