/***************************************************************************\
 *   Copyright (C) 2009 by Nestal Wan                                      *
 *   me@nestal.net                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
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
	\file	ElementReaderTest.cc
	\brief	implementation the ElementReaderTest class
	\date	Sat Apr 18 2009
	\author	Nestal Wan
*/

#include "ElementReaderTest.hh"

#include "file/ElementReader.hh"
#include "file/RawElement.hh"

#include "mock/MockFile.hh"

ElementReaderTest::ElementReaderTest( )
{
}

void ElementReaderTest::TestStoreFind( )
{
	MockFile f ;
	pdf::ElementReader sub( &f ) ;

	f.AddObj( pdf::Ref( 1, 0 ), pdf::Object( 1.0f ) ) ;

	pdf::RawElement *e = sub.Read<pdf::RawElement>( pdf::Ref( 1, 0 ) ) ;
	CPPUNIT_ASSERT( e == sub.Find( pdf::Ref( 1, 0 ) ) ) ;
	CPPUNIT_ASSERT( e->Get().As<double>() == 1.0f ) ;
}

void ElementReaderTest::TestRead( )
{
	MockFile f ;
	pdf::ElementReader sub( &f ) ;

	f.AddObj( pdf::Ref( 1, 0 ), pdf::Object( "I am a string" ) ) ;
	
	pdf::IElement *e = sub.Read( pdf::Ref( 1, 0 ) ) ;
	CPPUNIT_ASSERT( e == sub.Find( pdf::Ref( 1, 0 ) ) ) ;
	pdf::RawElement *re = dynamic_cast<pdf::RawElement*>( e ) ;
	CPPUNIT_ASSERT( re != 0 ) ;
	CPPUNIT_ASSERT( re->Get().As<std::string>() == "I am a string" ) ;
}

void ElementReaderTest::TestDetach( )
{
	MockFile f ;
	pdf::ElementReader sub( &f ) ;

	pdf::Dictionary dict ;
	dict["Type"]	= pdf::Name( "Font" ) ;
	dict["Field1"]	= 100.1f ;
	f.AddObj( pdf::Ref( 1, 0 ), dict ) ;

	pdf::Name out ;
	CPPUNIT_ASSERT( sub.Detach( dict, "Type", out ) ) ;
	CPPUNIT_ASSERT( out == pdf::Name( "Font" ) ) ;
	CPPUNIT_ASSERT( dict.find( "Type" ) == dict.end( ) ) ;
}

void ElementReaderTest::TestDeRef( )
{
	MockFile f ;
	pdf::ElementReader sub( &f ) ;
	
	f.AddObj( pdf::Ref( 1, 0 ), pdf::Object( false ) ) ;
	pdf::Object obj( "hahaha" ) ;
	CPPUNIT_ASSERT( &obj == &sub.DeRef( obj ) ) ;
	CPPUNIT_ASSERT( obj.As<std::string>() == "hahaha" ) ;
	
	pdf::Object ref( pdf::Ref( 1, 0 ) ) ;
	CPPUNIT_ASSERT( &ref == &sub.DeRef(ref) ) ;
	CPPUNIT_ASSERT( ref.As<bool>() == false ) ;
}

void ElementReaderTest::TestDetachElement( )
{
	MockFile f ;
	pdf::ElementReader sub( &f ) ;
	
	f.AddObj( pdf::Ref( 1, 0 ), pdf::Name( "Nobuo" ) ) ;
	
	pdf::Dictionary dict ;
	dict["Link"] = pdf::Ref( 1, 0 ) ;
	
	std::auto_ptr<pdf::RawElement> e(
		sub.DetachElement<pdf::RawElement>( dict, "Link" ) ) ;

	CPPUNIT_ASSERT( e.get() != 0 ) ;
	CPPUNIT_ASSERT( dict.find( "Link" ) == dict.end() ) ;
	CPPUNIT_ASSERT( e->Get().As<pdf::Name>() == pdf::Name( "Nobuo" ) ) ;
}
