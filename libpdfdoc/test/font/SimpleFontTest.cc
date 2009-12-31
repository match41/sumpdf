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

// freetype headers
#include <ft2build.h>
#include FT_FREETYPE_H

#include <sstream>

SimpleFontTest::SimpleFontTest( )
{
}

void SimpleFontTest::TestSimple( )
{
	FT_Library lib ;
	::FT_Init_FreeType( &lib ) ;
	
	FT_Face face ;
	::FT_New_Face(
		lib,
		(std::string(TEST_DATA_DIR) +"FreeMonoBoldOblique.ttf").c_str(),
		0,
		&face ) ;
	
	{
		pdf::SimpleFont subject( face ) ;
		CPPUNIT_ASSERT( subject.BaseName( ) == "FreeMonoBoldOblique" ) ;
		
		
	}
	
	::FT_Done_Face( face ) ;
	::FT_Done_FreeType( lib ) ;
}
