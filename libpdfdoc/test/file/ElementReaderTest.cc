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
