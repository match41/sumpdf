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
	\file	FileTest.hh
	\brief	definition the FileTest class
	\date	Thu Apr 10 2008
	\author	Nestal Wan
*/

#ifndef __PDFUT_FILE_TEST_HEADER_INCLUDED__
#define __PDFUT_FILE_TEST_HEADER_INCLUDED__

#include <cppunit/TestFixture.h>

#include <cppunit/extensions/HelperMacros.h>

namespace pdf
{
	class Dictionary ;
	class Stream ;
	class File ;
}

/*!	\brief	brief description
	
	this class represents
*/
class RealFileTest : public CppUnit::TestFixture
{
public :
	RealFileTest( ) ;

	// declare suit function
	CPPUNIT_TEST_SUITE( RealFileTest ) ;
		CPPUNIT_TEST( TestSimple ) ;
		CPPUNIT_TEST( TestReadStream ) ;
	CPPUNIT_TEST_SUITE_END();

public :
	void setUp( ) ;
	void tearDown( ) ;

private :
	void TestSimple( ) ;
	void TestReadStream( ) ;

private :
	pdf::Dictionary	*m_page ;
	pdf::Stream		*m_content ;
	pdf::Dictionary	*m_page_tree ;
	pdf::Dictionary	*m_font ;
	pdf::Dictionary	*m_res ;
	pdf::Dictionary	*m_doc_info ;
	pdf::Dictionary	*m_catalog ;
} ;

#endif
