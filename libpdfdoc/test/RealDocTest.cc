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

/**
	\file	RealDocTest.cc
	\brief	definition the RealDocTest class
	\date	Dec 27, 2009
	\author	Nestal Wan
*/

#include "RealDocTest.hh"

#include "RealDoc.hh"
#include "DocInfo.hh"

#include "mock/Assert.hh"

namespace pdfut {

using namespace pdf ;

RealDocTest::RealDocTest( )
{
}

void RealDocTest::TestRead( )
{
	pdf::RealDoc doc ;
	doc.Read( std::string(TEST_DATA_DIR) + "FileTestSimple.pdf" ) ;
	
	pdf::DocInfo *info = doc.Info( ) ;
	CPPUNIT_ASSERT( info != 0 ) ;
	PDFUT_ASSERT_EQUAL( info->Producer(),	"nestal" ) ;
	PDFUT_ASSERT_EQUAL( info->Creator(),	"D:20080410074227" ) ;
	
	CPPUNIT_ASSERT( doc.PageCount() > 0 ) ;
	
	Page *page1 = doc.GetPage(0) ;
}

} // end of namespace
