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

/**	\file	CompositeFontTest.cc
	\brief	implementation of the CompositeFontTest class
	\date	Mar 22, 2010
	\author	Nestal Wan
*/

#include "CompositeFontTest.hh"

#include "file/DictReader.hh"

#include "font/CompositeFont.hh"

#include "mock/Assert.hh"
#include "mock/MockFile.hh"
#include "mock/MockFontDb.hh"

#include <sstream>

namespace pdfut {

using namespace pdf ;

CompositeFontTest::CompositeFontTest( )
{
}

void CompositeFontTest::Test( )
{
	std::istringstream iss(
		"<</BaseFont /Arial/DescendantFonts [27 0 R]/Encoding /Identity-H"
		"/Subtype /Type0/ToUnicode 24 0 R/Type /Font>>" ) ;

	Dictionary fdict ;
	CPPUNIT_ASSERT( iss >> fdict ) ;

	MockFile file ;
	file.AddText( Ref(27,0),
		"<< /Type /Font/Subtype /CIDFontType2/BaseFont /Arial"
		"/CIDSystemInfo<< /Registry (Adobe)/Ordering (Identity)/Supplement 0>>"
		"/FontDescriptor 26 0 R"
		"/W [0 [ 750 666 333 222 556 277 556 556 222 556 277 666 500 556 277 "
		"500 833 833 722 556 722 666 777 722 722 277 666 500 556 722 277 722 "
		"666 610 556 500 556 556 556 556 556 556 556 277 556 666 610 722 556 "
		"500 333 500 777 556 666 556 556 277 277 943 556 333 333 666 ]]>>" ) ;

	file.AddText( Ref(26,0),
		"<</Type /FontDescriptor /FontName /Arial /Flags 4"
		"/FontBBox [ -664 -324 2000 1005 ]/ItalicAngle 0/Ascent 905"
		"/Descent -211/CapHeight 1005/StemV 80/StemH 80/FontFile2 22 0 R>>" ) ;

	DictReader dr( fdict, &file ) ;
	
	CompositeFont subject( dr, m_mock_fdb ) ;
	PDFUT_ASSERT_EQUAL( subject.BaseName(), "Arial" ) ;
}

} // end of namespace
