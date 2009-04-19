/***************************************************************************
 *   Copyright (C) 2006 by Nestal Wan                                      *
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
 ***************************************************************************/

/*!
	\file	XObjectTest.cc
	\brief	implementation the XObjectTest class
	\date	Mon May 12 2008
	\author	Nestal Wan
*/

#include "XObjectTest.hh"

#include "core/Ref.hh"
#include "core/Dictionary.hh"
#include "stream/Stream.hh"
#include "file/ElementDest.hh"
#include "file/ElementReader.hh"
#include "mock/MockFile.hh"
#include "page/XObject.hh"

#include <sstream>
#include <iostream>

XObjectTest::XObjectTest( )
{
}

void XObjectTest::TestRead( )
{
/*	MockFile file ;
	file.AddObj( pdf::Ref( 2, 0 ), pdf::Name( "Image" ) ) ;
	
	pdf::ElementSrc repo( &file ) ;
	pdf::Stream s ;
	pdf::Dictionary& dict = s.GetDictionary() ;
	dict["SubType"]	= pdf::Ref( 2, 0 ) ;
	dict["Haha"]	= pdf::Name( "Wahaha" ) ;
	file.AddObj( pdf::Ref( 1, 0 ), s ) ;
	
	pdf::XObject subject ;
	subject.Read( pdf::Ref( 1, 0 ), &repo ) ;
	
	MockFile out_file ;
	pdf::ElementDest dest( &out_file ) ;
	subject.Write( pdf::Ref( 1, 0 ), &dest ) ;

	const pdf::Stream& out_str = out_file.Find( pdf::Ref( 1, 0 ) ) ;
	const pdf::Dictionary& out_dict = out_str.GetDictionary( ) ;

	CPPUNIT_ASSERT( out_dict["SubType"].As<pdf::Name>()	== "Image" ) ;
	CPPUNIT_ASSERT( out_dict["Haha"].As<pdf::Name>()	== "Wahaha" ) ;
*/
}
