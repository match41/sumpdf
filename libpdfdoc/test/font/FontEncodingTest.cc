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

/**	\file	FontEncodingTest.cc
	\brief	implementation of the FontEncodingTest class
	\date	Mar 21, 2010
	\author	Nestal Wan
*/

#include "FontEncodingTest.hh"

#include "file/DictReader.hh"

#include "font/FontEncoding.hh"

#include "mock/Assert.hh"
#include "mock/MockFile.hh"

#include <sstream>

namespace pdfut {

using namespace pdf ;

FontEncodingTest::FontEncodingTest( )
{
}

void FontEncodingTest::setUp( )
{
}

void FontEncodingTest::tearDown( )
{
}

void FontEncodingTest::Test( )
{
	std::istringstream ss(
		"<</Type /Encoding/Differences [ 0 /.notdef 15/bullet 16/.notdef]>>" ) ;
	
	Dictionary self ;
	CPPUNIT_ASSERT( ss >> self ) ;
	
	MockFile file ;
	DictReader dr( self, &file ) ;

	FontEncoding subject( dr ) ;
	PDFUT_ASSERT_EQUAL( subject.ToUnicode( 15 ), 8226 ) ;
}

void FontEncodingTest::TestRoundTrip( )
{
	std::istringstream ss(
		"<</BaseEncoding /WinAnsiEncoding"
		"/Differences [1 /space /W /h /i /t /e /P /a /p /r /u /b /l /s /d "
		"/colon /A /two /zero /four /I /n /o /c /period /one /R /T /F /S /y /x "
		"/C /v /f /m /five /six /H /D /nine /E /N /three /eight /M /g /seven"
		" /w /O /hyphen /L /B /endash /U /Z /k /parenleft /less /greater "
		"/parenright /V /j /slash /comma /quotedblleft /quotedblright /Y "
		"/quotesingle /G /backslash /q /z /K /braceleft /braceright /semicolon "
		"/numbersign /question /plus /asterisk /bar /ampersand /J /underscore "
		"/quotedbl /quoteleft /quoteright /emdash /equal /X /Q /dagger "
		"/daggerdbl /section /registered /asciitilde /percent /at /ydieresis "
		"/bracketleft /bracketright]/Type /Encoding>>" ) ;
	
	Dictionary self ;
	CPPUNIT_ASSERT( ss >> self ) ;
	
	MockFile file ;
	DictReader dr( self, &file ) ;

	FontEncoding subject( dr ) ;

	std::wstring str = L"White" ;
	char out[] = { 2, 3, 4, 5, 6, '\0' } ;
	
	std::ostringstream oss ;
	subject.Encode( str.begin(), str.end(), oss ) ;
	
	PDFUT_ASSERT_EQUAL( oss.str(), out ) ;
	std::wstring wout = subject.Decode( oss.str() ) ;
	PDFUT_ASSERT_EQUAL( wout, str ) ;
}

} // end of namespace
