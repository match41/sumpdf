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

/**
	\file	CatalogTest.cpp
	\brief	definition the CatalogTest class
	\date	Dec 13, 2009
	\author	Nestal Wan
*/

#include "CatalogTest.hh"

#include "core/Dictionary.hh"
#include "file/Catalog.hh"

#include "mock/MockFile.hh"
#include "mock/Assert.hh"

#include <sstream>

CatalogTest::CatalogTest( )
{
}

void CatalogTest::TestRead( )
{
	std::istringstream ss( "<</PageLayout /SinglePage/PageMode /UseNode"
	                       "/Pages 1 0 R/Type /Catalog/Version /1.4>>" ) ;
	pdf::Dictionary cat_dict ;
	CPPUNIT_ASSERT( ss >> cat_dict ) ;
	
	std::istringstream ss2( "<</Count 1/Kids [3 0 R] /Type/Pages>>" ) ;
	pdf::Dictionary ptree_dict ;
	CPPUNIT_ASSERT( ss2 >> ptree_dict ) ;
	
	std::istringstream ss3( "<</Parent 1 0 R/Type /Page>>" ) ;
	pdf::Dictionary page_dict ;
	CPPUNIT_ASSERT( ss3 >> page_dict ) ;

	MockFile file ;
	file.AddObj( pdf::Ref( 1, 0 ), 	ptree_dict ) ;
	file.AddObj( pdf::Ref( 2, 0 ),	cat_dict ) ;
	file.AddObj( pdf::Ref( 3, 0 ),	page_dict ) ;
	
	pdf::Catalog c( pdf::Ref( 2, 0 ), &file ) ;
	PDF_ASSERT_EQUAL( c.PageCount( ), 1U ) ;
	CPPUNIT_ASSERT( c.GetPage( 0 ) != 0 ) ;
}
