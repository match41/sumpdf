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

/**	\file	FontPoolTest.cc
	\brief	implementation of the FontPoolTest class
	\date	Jan 18, 2010
	\author	Nestal Wan
*/

#include "FacePoolTest.hh"

#include "core/Object.hh"
#include "font/FacePool.hh"
#include "stream/Stream.hh"
#include "mock/MockFile.hh"

#include <fstream>
#include <iterator>
#include <vector>

/**	constructor
*/
FacePoolTest::FacePoolTest( )
{
}

void FacePoolTest::setUp( )
{
	::FT_Init_FreeType( &m_lib ) ;
}

void FacePoolTest::tearDown( )
{
	::FT_Done_FreeType( m_lib ) ;
}

void FacePoolTest::TestSimple( )
{
	std::ifstream progif(
		(std::string(TEST_DATA_DIR) +"FreeMonoBoldOblique.ttf").c_str() ) ;

	std::vector<unsigned char> prog(
		(std::istreambuf_iterator<char>(progif)),
		(std::istreambuf_iterator<char>()) ) ;
	pdf::Stream s( prog, pdf::Object() ) ;
	MockFile file ;
	file.AddObj( pdf::Ref(1,0), s ) ;

	pdf::FacePool subject( m_lib ) ;
	FT_Face f = subject.GetFace( pdf::Ref(1,0), &file ) ;
}
