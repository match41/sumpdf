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
	\file	DictionaryTest.cc
	\brief	implementation the DictionaryTest class
	\date	Tue Mar 11 2008
	\author	Nestal Wan
*/

#include "DictionaryTest.hh"

#include "core/Dictionary.hh"
#include "mock/Assert.hh"

#include <algorithm>
#include <iterator>
#include <sstream>
#include <vector>

DictionaryTest::DictionaryTest( )
{
}

void DictionaryTest::TestNormal( )
{
	std::istringstream ss( "<< /Name (Me) /Not 1234.6 /Wahaha 1 0 R>>" ) ;
	pdf::Dictionary d ;
	CPPUNIT_ASSERT( ss >> d ) ;
	
	pdf::Dictionary::iterator i = d.begin( ) ;
	CPPUNIT_ASSERT( d.size() == 3 ) ;
	CPPUNIT_ASSERT( i->first == "Name" ) ;
	CPPUNIT_ASSERT( i->second.As<std::string>( ) == "Me" );

	++i ;

	CPPUNIT_ASSERT( i->first == "Not" ) ;
	CPPUNIT_ASSERT( i->second == 1234.6 ) ;

	++i ;

	CPPUNIT_ASSERT( i->first == "Wahaha" ) ;
	CPPUNIT_ASSERT( i->second == pdf::Ref( 1, 0 ) ) ;
}

void DictionaryTest::TestRead( )
{
	pdf::Dictionary d ;
	d.Add( pdf::Name( "Name" ), "Me" ) ;
	d.Add( pdf::Name( "Not" ), 1234.6 ) ;

	std::stringstream ss ;
	CPPUNIT_ASSERT( ss << d ) ;

	pdf::Dictionary d2 ;
	CPPUNIT_ASSERT( ss >> d2 ) ;
	CPPUNIT_ASSERT( d == d2 ) ;
}

void DictionaryTest::TestTrailer( )
{
	std::istringstream ss( "<</Size 733/Prev 4567301/Root 702 0 R"
	                       "/Encrypt 701 0 R/Info 699 0 R"
	                       "/ID[<dfd22bf09540d635f4dc67eeab95fbd1>"
	                       "<76c3a084d963384284c180f6ea6c2185>]>>" ) ;

	pdf::Dictionary d ;
	CPPUNIT_ASSERT( ss >> d ) ;
	CPPUNIT_ASSERT( d["Prev"] == 4567301 ) ;
	CPPUNIT_ASSERT( !d.empty() ) ;
}

void DictionaryTest::TestDoubleEnd( )
{
	std::istringstream ss( "<</Pages 677 0 R/Type/Catalog/OpenAction "
	                       "703 0 R/PageLabels 675 0 R/Metadata 698 "
	                       "0 R/OCProperties<</D<</Order[]/AS[<</OCGs[705 0 "
	                       "R]/Category[/View]/Event/View>><</OCGs[705 0 "
	                       "R]/Category[/Print]/Event/Print>><</OCGs[705 0 "
	                       "R]/Category[/Export]/Event/Export>>]/ON[705 0 "
	                       "R]/RBGroups[]>>/OCGs[705 0 R]>>>>" ) ;
	pdf::Dictionary d ;
	CPPUNIT_ASSERT( ss >> d ) ;
	CPPUNIT_ASSERT( d["Pages"] == pdf::Ref( 677, 0 ) ) ;
	CPPUNIT_ASSERT( !d.empty() ) ;
}

void DictionaryTest::TestImage( )
{
	std::istringstream ss( 
					"<< /Type /XObject"
					"   /Subtype /Image"
					"   /Width 56"
					"   /Height 28"
					"   /BitsPerComponent 8"
					"   /Length 24 0 R"
					"   /Filter /FlateDecode"
					"   /ColorSpace [  /Indexed /DeviceRGB 255 <"
					"000000 240000 480000 6D0000 910000 B60000 DA0000 FF0000"
					"002400 242400 482400 6D2400 912400 B62400 DA2400 FF2400"
					"004800 244800 484800 6D4800 914800 B64800 DA4800 FF4800"
					"006D00 246D00 486D00 6D6D00 916D00 B66D00 DA6D00 FF6D00"
					"009100 249100 489100 6D9100 919100 B69100 DA9100 FF9100"
					"00B600 24B600 48B600 6DB600 91B600 B6B600 DAB600 FFB600"
					"00DA00 24DA00 48DA00 6DDA00 91DA00 B6DA00 DADA00 FFDA00"
					"00FF00 24FF00 48FF00 6DFF00 91FF00 B6FF00 DAFF00 FFFF00"
					"000055 240055 480055 6D0055 910055 B60055 DA0055 FF0055"
					"002455 242455 482455 6D2455 912455 B62455 DA2455 FF2455"
					"004855 244855 484855 6D4855 914855 B64855 DA4855 FF4855"
					"006D55 246D55 486D55 6D6D55 916D55 B66D55 DA6D55 FF6D55"
					"009155 249155 489155 6D9155 919155 B69155 DA9155 FF9155"
					"00B655 24B655 48B655 6DB655 91B655 B6B655 DAB655 FFB655"
					"00DA55 24DA55 48DA55 6DDA55 91DA55 B6DA55 DADA55 FFDA55"
					"00FF55 24FF55 48FF55 6DFF55 91FF55 B6FF55 DAFF55 FFFF55"
					"0000AA 2400AA 4800AA 6D00AA 9100AA B600AA DA00AA FF00AA"
					"0024AA 2424AA 4824AA 6D24AA 9124AA B624AA DA24AA FF24AA"
					"0048AA 2448AA 4848AA 6D48AA 9148AA B648AA DA48AA FF48AA"
					"006DAA 246DAA 486DAA 6D6DAA 916DAA B66DAA DA6DAA FF6DAA"
					"0091AA 2491AA 4891AA 6D91AA 9191AA B691AA DA91AA FF91AA"
					"00B6AA 24B6AA 48B6AA 6DB6AA 91B6AA B6B6AA DAB6AA FFB6AA"
					"00DAAA 24DAAA 48DAAA 6DDAAA 91DAAA B6DAAA DADAAA FFDAAA"
					"00FFAA 24FFAA 48FFAA 6DFFAA 91FFAA B6FFAA DAFFAA FFFFAA"
					"0000FF 2400FF 4800FF 6D00FF 9100FF B600FF DA00FF FF00FF"
					"0024FF 2424FF 4824FF 6D24FF 9124FF B624FF DA24FF FF24FF"
					"0048FF 2448FF 4848FF 6D48FF 9148FF B648FF DA48FF FF48FF"
					"006DFF 246DFF 486DFF 6D6DFF 916DFF B66DFF DA6DFF FF6DFF"
					"0091FF 2491FF 4891FF 6D91FF 9191FF B691FF DA91FF FF91FF"
					"00B6FF 24B6FF 48B6FF 6DB6FF 91B6FF B6B6FF DAB6FF FFB6FF"
					"00DAFF 24DAFF 48DAFF 6DDAFF 91DAFF B6DAFF DADAFF FFDAFF"
					"00FFFF 24FFFF 48FFFF 6DFFFF 91FFFF B6FFFF DAFFFF FFFFFF"
					"> ]"
					">>" ) ;

	pdf::Dictionary subject ;
	CPPUNIT_ASSERT( ss >> subject ) ;
	CPPUNIT_ASSERT( !subject.empty() ) ;
	PDF_ASSERT_EQUAL( subject["Type"].As<pdf::Name>(), "XObject" ) ;
	PDF_ASSERT_EQUAL(
		subject["Length"].As<pdf::Ref>(),
		pdf::Object(pdf::Ref( 24, 0 )) ) ;
}

void DictionaryTest::TestExtract( )
{
	pdf::Dictionary subject ;
	subject["AAA"] = pdf::Name( "BBB" ) ;
	PDF_ASSERT_EQUAL( subject.size(), 1U ) ;
	
	pdf::Name out ;
	subject.Extract( "AAA", out ) ;
	PDF_ASSERT_EQUAL( out, pdf::Name( "BBB" ) ) ;
	PDF_ASSERT_EQUAL( subject.size(), 0U ) ;
}

void DictionaryTest::TestSwap( )
{
	pdf::Dictionary s1, s2 ;
	s1["AAA"] = "bbb" ;
	s2["bbb"] = "AAA" ;
	
	s1.swap( s2 ) ;
	
	CPPUNIT_ASSERT( s1["bbb"] == "AAA" ) ;
	CPPUNIT_ASSERT( s2["AAA"] == "bbb" ) ;
}

void DictionaryTest::TestNull( )
{
	pdf::Dictionary s ;
	CPPUNIT_ASSERT( s["???"].Is<void>() ) ;
	PDF_ASSERT_EQUAL( s.size(), 0U ) ; 
	CPPUNIT_ASSERT( s.empty() ) ;
	
	s["haha"] = pdf::Object() ;
	CPPUNIT_ASSERT( s.find( "haha" ) == s.end() ) ;
	PDF_ASSERT_EQUAL( s.size(), 0U ) ;
	CPPUNIT_ASSERT( s.empty() ) ;
}

void DictionaryTest::TestErase( )
{
	pdf::Dictionary s ;
	s["???"] = "haha" ;
	PDF_ASSERT_EQUAL( s.size(), 1U ) ;
	
	s["!!!"] = "hehe" ;
	PDF_ASSERT_EQUAL( s.size(), 2U ) ;
	
	s.erase( "???" ) ;
	PDF_ASSERT_EQUAL( s.size(), 1U ) ;
	CPPUNIT_ASSERT( s["???"].Is<void>() ) ;
	CPPUNIT_ASSERT( s.find( "haha" ) == s.end() ) ;
	PDF_ASSERT_EQUAL( s["!!!"], "hehe" ) ;
}

void DictionaryTest::TestEraseByAddNull( )
{
	pdf::Dictionary s ;
	s["???"] = "haha" ;
	PDF_ASSERT_EQUAL( s.size(), 1U ) ;
	
	s["!!!"] = "hehe" ;
	PDF_ASSERT_EQUAL( s.size(), 2U ) ;
	
	s.Add( "???", pdf::Object() ) ;
	PDF_ASSERT_EQUAL( s.size(), 1U ) ;
	CPPUNIT_ASSERT( s["???"].Is<void>() ) ;
	CPPUNIT_ASSERT( s.find( "haha" ) == s.end() ) ;
	PDF_ASSERT_EQUAL( s["!!!"], "hehe" ) ;
}
