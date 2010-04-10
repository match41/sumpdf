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

namespace pdfut {

using namespace pdf ;

SfntTest::SfntTest( )
{
}

void SfntTest::setUp( )
{
	FT_Init_FreeType( &m_ft ) ;
	FT_New_Face(
		m_ft,
		(std::string(TEST_DATA_DIR) + "FreeMonoBoldOblique.ttf").c_str(),
		0,
		&m_face ) ;
}

void SfntTest::tearDown( )
{
	FT_Done_Face( m_face ) ;
	FT_Done_FreeType( m_ft ) ;
}

void SfntTest::Test( )
{
	Sfnt subject( m_face ) ;
	
	std::stringstream out ;
	subject.Write( out.rdbuf() ) ;
	std::string os = out.str() ;
	
	PDFUT_ASSERT_EQUAL( os.size(), 284U ) ;
	std::vector<char> exp( os.size() ) ;
	std::ifstream font_file(
		(std::string(TEST_DATA_DIR) + "FreeMonoBoldOblique.ttf").c_str(),
		std::ios::in | std::ios::binary ) ;
	
	font_file.rdbuf()->sgetn( &exp[0], exp.size() ) ;
	PDFUT_ASSERT_RANGE_EQUAL( os.begin(), os.end(), exp.begin() ) ;
}

} // end of namespace
