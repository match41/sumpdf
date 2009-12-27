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

const std::string RealDoc::Info_::m_empty ;

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
	delete m_catalog ;
}

void RealDoc::Read( const std::string& filename )
{
	m_readfs.open( filename.c_str(), std::ios::in | std::ios::binary ) ;
	if ( !m_readfs )
		throw std::runtime_error( "cannot open file " + filename ) ;

	// read the cross reference of the PDF file
	File file( &m_readfs ) ;

	m_catalog		= new Catalog( file.Root( ), &file ) ;
	
	// DocInfo is optional
	if ( file.DocInfo( ) != Ref() )
		m_info.m_dict = file.ReadObj( file.DocInfo() ) ;
}

void RealDoc::Write( const std::string& filename ) const
{
	std::ofstream fs( filename.c_str(), std::ios::out | std::ios::binary ) ;
	if ( !fs )
		throw std::runtime_error( "cannot open file " + filename ) ;

	File file( &fs ) ;
	file.WriteTrailer(
		m_catalog->Write( &file ),
		m_info.m_dict.empty() ? Ref() : file.WriteObj( m_info.m_dict ) ) ;
}

RealPage* RealDoc::AppendPage( )
{
	assert( m_catalog != 0 ) ;

	return m_catalog->AddPage( ) ;
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

const DocInfo* RealDoc::Info( ) const
{
	return &m_info ;
}

DocInfo* RealDoc::Info( )
{
	return &m_info ;
}

const std::string& RealDoc::Info_::Creator() const
{
	return Field( "Creator" ) ;
}

const std::string& RealDoc::Info_::Producer() const
{
	return Field( "Producer" ) ;
}

const std::string& RealDoc::Info_::Author() const
{
	return Field( "Author" ) ;
}

const std::string& RealDoc::Info_::Subject() const
{
	return Field( "Subject" ) ;
}

const std::string& RealDoc::Info_::Keywords() const
{
	return Field( "Keywords" ) ;
}

const std::string& RealDoc::Info_::Field( const Name& name ) const
{
	Dictionary::const_iterator i = m_dict.find( name ) ;
	return i != m_dict.end() ? i->second.As<std::string>( ) : m_empty ;
}

void RealDoc::Info_::SetCreator( const std::string& creator )
{
	SetField( "Creator", creator ) ;
}

void RealDoc::Info_::SetProducer( const std::string& creator )
{
	SetField( "Producer", creator ) ;
}

void RealDoc::Info_::SetAuthor( const std::string& creator )
{
	SetField( "Author", creator ) ;
}

void RealDoc::Info_::SetSubject( const std::string& creator )
{
	SetField( "Subject", creator ) ;
}

void RealDoc::Info_::SetKeywords( const std::string& creator )
{
	SetField( "Keywords", creator ) ;
}

void RealDoc::Info_::SetField(const Name& name, const std::string& value)
{
	if ( value.empty() )
		m_dict.erase( name ) ;
	else
		m_dict[name] = value ;
}

} // end of namespace
