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
	\file	UnitTest.cc
	\brief	implementation the unit tests of the project
	\date	Sun Mar 9 2008
	\author	Nestal Wan
*/

#include <cppunit/ui/text/TestRunner.h>

#include "RealDocTest.hh"
#include "core/TokenTest.hh"
#include "core/TokenSrcTest.hh"
#include "core/NameTest.hh"
#include "core/RefTest.hh"
#include "core/ObjectTest.hh"
#include "core/StringTest.hh"
#include "core/DictionaryTest.hh"
#include "core/ArrayTest.hh"
#include "graphics/TextStateTest.hh"
#include "graphics/RealTextTest.hh"
#include "stream/StreamTest.hh"
#include "stream/DeflateFilterTest.hh"
#include "stream/InStreamBufAdaptorTest.hh"
#include "stream/OutStreamBufAdaptorTest.hh"
#include "stream/RawFilterTest.hh"
#include "page/RealResourcesTest.hh"
#include "page/RealPageTest.hh"
#include "page/RealContentTest.hh"

// freetype library is optional
#ifdef HAVE_FREETYPE
    #include "font/SimpleFontTest.hh"
#endif

#include "page/XObjectTest.hh"
#include "file/RealFileTest.hh"
#include "file/CatalogTest.hh"
#include "file/RefObjMapTest.hh"
#include "file/CompleteObjTest.hh"
#include "font/SimpleFontTest.hh"
#include "page/PageNodeTest.hh"
#include "image/PngImageTest.hh"
#include "image/JpegImageTest.hh"

int main( int argc, char **argv )
{
	CppUnit::TextUi::TestRunner runner;
	runner.addTest( RealDocTest::suite( ) ) ;
	runner.addTest( TokenTest::suite( ) ) ;
	runner.addTest( TokenSrcTest::suite( ) ) ;
	runner.addTest( NameTest::suite( ) ) ;
	runner.addTest( RefTest::suite( ) ) ;
	runner.addTest( ObjectTest::suite( ) ) ;
	runner.addTest( StringTest::suite( ) ) ;
	runner.addTest( DictionaryTest::suite( ) ) ;
	runner.addTest( ArrayTest::suite( ) ) ;
	runner.addTest( RealPageTest::suite( ) ) ;
	runner.addTest( RealResourcesTest::suite( ) ) ;
	runner.addTest( XObjectTest::suite( ) ) ;
	runner.addTest( StreamTest::suite( ) ) ;
	runner.addTest( RealFileTest::suite( ) ) ;
	runner.addTest( CompleteObjTest::suite( ) ) ;
	runner.addTest( CatalogTest::suite( ) ) ;
	runner.addTest( RefObjMapTest::suite( ) ) ;
	runner.addTest( PageNodeTest::suite( ) ) ;
	runner.addTest( TextStateTest::suite( ) ) ;
	runner.addTest( RealTextTest::suite( ) ) ;
	runner.addTest( RealContentTest::suite( ) ) ;
	
// freetype library is optional
#ifdef HAVE_FREETYPE
	runner.addTest( SimpleFontTest::suite( ) ) ;
#endif

#ifdef HAVE_PNG
	runner.addTest( PngImageTest::suite( ) ) ;
#endif

#ifdef HAVE_JPEG
	runner.addTest( JpegImageTest::suite( ) ) ;
#endif
	runner.addTest( DeflateFilterTest::suite( ) ) ;
	runner.addTest( InStreamBufAdaptorTest::suite( ) ) ;
	runner.addTest( OutStreamBufAdaptorTest::suite( ) ) ;
	runner.addTest( RawFilterTest::suite( ) ) ;

	runner.run();
  
	return 0 ;
}
