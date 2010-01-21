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

#include "mock/Assert.hh"

#include <sstream>

SimpleFontTest::SimpleFontTest( )
{
}

void SimpleFontTest::TestSimple( )
{
	pdf::SimpleFont subject(
		std::string(TEST_DATA_DIR) +"FreeMonoBoldOblique.ttf",
		0,
		m_ft ) ;
	PDF_ASSERT_EQUAL( subject.BaseName( ), "FreeMonoBoldOblique" ) ;
}

void SimpleFontTest::setUp( )
{
	::FT_Init_FreeType( &m_ft ) ;
}

void SimpleFontTest::tearDown( )
{
	::FT_Done_FreeType( m_ft ) ;
}

#ifdef HAVE_FONTCONFIG
void SimpleFontTest::TestLoadByName( )
{
	pdf::SimpleFont subject( "Arial", m_ft ) ;
	PDF_ASSERT_EQUAL( subject.BaseName( ), "ArialMT" ) ;
}
#endif
