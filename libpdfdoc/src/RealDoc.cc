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
	\file	RealDoc.cc
	\brief	implementation the PDF document class
	\date	Wed Mar 5 2008
	\author	Nestal Wan
*/

#include "RealDoc.hh"

#include "core/Array.hh"
#include "core/Ref.hh"
#include "core/Object.hh"

#include "file/ElementReader.hh"
#include "file/ElementDest.hh"
#include "file/ElementList.hh"
#include "file/ElementTracker.hh"
#include "file/File.hh"

#include "page/RealPage.hh"
#include "page/PageTree.hh"

#include <boost/bind.hpp>
#include <boost/lambda/construct.hpp>

#include <algorithm>
#include <sstream>
#include <stdexcept>

#include <iostream>

namespace pdf {

RealDoc::RealDoc( )
	: m_catalog( new Catalog )		// not exception safe
{
}

/*!	The destructor will delete all the elements contained. It traverses the
	whole document to find out all elements and free them up.
*/
RealDoc::~RealDoc( )
{
	// traverse the document to get all elements
//	ElementTracker tracker ;
//	tracker.Traverse( m_catalog ) ;
//
//	ElementList list ;
//	tracker.Get( list ) ;
//
//	// delete them all
//	std::for_each( list.begin( ), list.end( ), boost::lambda::delete_ptr( ) ) ;
	delete m_catalog ;
}

void RealDoc::Read( const std::string& filename )
{
	m_readfs.open( filename.c_str(), std::ios::in | std::ios::binary ) ;
	if ( !m_readfs )
		throw std::runtime_error( "cannot open file " + filename ) ;

	// read the cross reference of the PDF file
	File file( &m_readfs ) ;

	ElementReader repo( &file ) ;
	m_catalog = new Catalog ;
	m_catalog->Read( file.Root( ), &file ) ;
}

void RealDoc::Write( const std::string& filename ) const
{
	std::ofstream fs( filename.c_str(), std::ios::out | std::ios::binary ) ;
	if ( !fs )
		throw std::runtime_error( "cannot open file " + filename ) ;

	File file( &fs ) ;
	file.WriteTrailer( m_catalog->Write( &file ) ) ;
}

RealPage* RealDoc::AppendPage( )
{
	assert( m_catalog != 0 ) ;

    RealPage *p = new RealPage ;
	m_catalog->AddPage( p ) ;
	return p ;
}

std::size_t RealDoc::PageCount( ) const
{
	assert( m_catalog != 0 ) ;
	return m_catalog->PageCount( ) ;
}

RealPage* RealDoc::GetPage( std::size_t index )
{
	return m_catalog->GetPage( index ) ;
}

StandardFont* RealDoc::CreateSimpleFont( const std::string& name )
{
	return new StandardFont( Name(name) ) ;
}

RealPage* RealDoc::AddPage( std::size_t index )
{
    return 0 ;
}

} // end of namespace
