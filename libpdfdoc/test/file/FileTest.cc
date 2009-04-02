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
	\file	FileTest.cc
	\brief	implementation the FileTest class
	\date	Thu Apr 10 2008
	\author	Nestal Wan
*/

#include "FileTest.hh"

#include "file/File.hh"

#include "core/Ref.hh"
#include "core/Dictionary.hh"
#include "core/Array.hh"
#include "core/Stream.hh"
#include "core/Object.hh"
#include "core/Token.hh"

FileTest::FileTest( )
{
}

void FileTest::TestSimple( )
{
	using namespace pdf ;
	
	std::ostringstream file ;
// 	std::ofstream file( "a.pdf", std::ios::out | std::ios::binary ) ;
	File f( &file ) ;
	
	Ref link[] =
	{
		Ref( ),
		f.AllocLink( ),
		f.AllocLink( ), 
		f.AllocLink( ), 
		f.AllocLink( ), 
		f.AllocLink( ), 
		f.AllocLink( ), 
		f.AllocLink( ), 
	} ; 
	
	Dictionary obj3 ;
	obj3["Type"] = Name( "Page" ) ;
	obj3["Parent"] = Ref( 1, 0 ) ;
	obj3["Resources"] = Ref( 2, 0 ) ;
	obj3["Contents"] = Ref( 4, 0 ) ;
	f.WriteObj( obj3, link[3] ) ;

	Stream obj4( "2 J\n0.57 w\nBT /F1 16.00 Tf ET\n"
	             "BT 31.19 794.57 Td (Hello World!) Tj ET\n" ) ;
	f.WriteObj( obj4, link[4] ) ;

	Ref pages[] = { Ref( 3, 0 ) } ;
	double mbox[] = { 0, 0, 595.28, 841.89 } ;
	Dictionary obj1 ;
	obj1["Type"]		= Name( "Pages" ) ;
	obj1["Kids"]		= Array( pages, pages + 1 ) ;
	obj1["Count"]		= 1 ;
	obj1["MediaBox"]	= Array( mbox, mbox+4 ) ;
	f.WriteObj( obj1, link[1] ) ;
	
	Dictionary obj5 ;
	obj5["Type"]		= Name( "Font" ) ;
	obj5["BaseFont"]	= Name( "Helvetica-Bold" ) ;
	obj5["Subtype"]		= Name( "Type1" ) ;
	obj5["Encoding"]	= Name( "WinAnsiEncoding" ) ;
	f.WriteObj( obj5, link[5] ) ;
	
	Name pset[] = { Name("PDF"), Name("Text") } ;
	
	Dictionary obj2 ;
	obj2["ProcSet"] = Array( pset, pset+2 ) ;
	Dictionary font ;
	font["F1"]  = Ref( 5, 0 ) ;
	obj2["Font"] = font ;
	f.WriteObj( obj2, link[2] ) ;
	
	Dictionary obj6 ;
	obj6["Producer"] = "nestal" ;
	obj6["CreationDate"] = "D:20080410074227" ;
	f.WriteObj( obj6, link[6] ) ;
	
	Dictionary obj7 ;
	obj7["Type"] = Name( "Catalog" ) ;
	obj7["Pages"] = Ref( 1, 0 ) ;
	f.WriteObj( obj7, link[7] ) ;
	
	f.WriteTrailer( link[7], "nestal", "D:20080410074227" ) ;
	
	// open expected file to compare and verify
	std::ifstream exp( (std::string(TEST_DATA_DIR) +
	                   "FileTestSimple.pdf").c_str( ),
	                   std::ios::in | std::ios::binary ) ;
	CPPUNIT_ASSERT( exp ) ;
	
	std::string file_str = file.str() ; 
	CPPUNIT_ASSERT( std::equal( file_str.begin( ), file_str.end( ),
	                            std::istreambuf_iterator<char>( exp ) ) ) ;

}

void FileTest::TestReadStream( )
{
	// open expected file to compare and verify
	std::ifstream file( (std::string(TEST_DATA_DIR) +
	                    "FileTestSimple.pdf").c_str( ),
	                    std::ios::in | std::ios::binary ) ;
	pdf::File f( &file ) ;
	
	pdf::Object obj = f.ReadObj( pdf::Ref( 4, 0 ) ) ;
	CPPUNIT_ASSERT( obj.IsType<pdf::Stream>() ) ;

	std::stringstream output ;
	std::size_t count = obj.As<pdf::Stream>().ReadAll( output.rdbuf() ) ;
	CPPUNIT_ASSERT( count == 70 ) ;
	CPPUNIT_ASSERT( output.str() == "2 J\n0.57 w\nBT /F1 16.00 Tf ET\n"
	                        "BT 31.19 794.57 Td (Hello World!) Tj ET\n" ) ;
}
