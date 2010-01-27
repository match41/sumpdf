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
	\file	RealPage.cc
	\brief	implementation the RealPage class
	\date	Sat Mar 15 2008
	\author	Nestal Wan
*/

#include "RealPage.hh"

#include "PageTree.hh"

// core object headers
#include "core/Array.hh"
#include "core/Object.hh"
#include "core/String.hh"

// other libpdfdoc headers
#include "file/ObjectReader.hh"
#include "file/IFile.hh"
#include "font/BaseFont.hh"
#include "graphics/RealText.hh"
#include "util/Rect.hh"
#include "util/Util.hh"

#include <boost/bind.hpp>

#include <algorithm>
#include <cassert>
#include <iostream>
#include <stdexcept>

namespace pdf {

RealPage::RealPage( PageTree *parent )
	: m_parent( parent ),
	  m_resources( parent->GetResource() ),
	  m_media_box( 0, 0, 595, 842 ),
	  m_rotate( 0 )
{
	assert( parent != 0 ) ;
	parent->AppendLeaf( this ) ;
}

void RealPage::Read( Dictionary& self, IFile *file )
{
	assert( file != 0 ) ;
	
	// read content
	Object contents ;
	if ( Detach( file, self, "Contents", contents ) )
	    ReadContent( contents, file ) ;

	// media box
	Array a ;
	if ( Detach( file, self, "MediaBox", a ) )
		m_media_box = Rect( a.begin( ), a.end( ) ) ;

	Dictionary res ;
	if ( Detach( file, self, "Resources", res ) )
		m_resources.Read( res, file ) ;
	
	m_self.Read( self, file ) ;
}

Rect RealPage::MediaBox( ) const
{
	return m_media_box ;
}

void RealPage::ReadContent( const Object& str_obj, IFile *src )
{
	// for indirect objects, dereference it
	if ( str_obj.Is<Ref>( ) )
		ReadContent( src->ReadObj( str_obj ), src ) ;

	// append individual stream objects
	else if ( str_obj.Is<Stream>( ) )
		m_cstrs.push_back( str_obj.As<Stream>( ) ) ;

	// catenate individual objects in array
	else if ( str_obj.Is<Array>( ) )
	{
		const Array& a = str_obj.As<Array>( ) ;
		std::for_each(
			a.begin( ), a.end( ),
		    boost::bind( &RealPage::ReadContent, this, _1, src ) ) ;
	}

	else if ( !str_obj.Is<void>( ) )
		throw std::runtime_error( "invalid page content" ) ;
}

void RealPage::Write( const Ref& link, IFile *file, const Ref& parent ) const
{
	assert( file != 0 ) ;
	assert( m_parent != 0 ) ;
	assert( GetResource( ) != 0 ) ;

	CompleteObj self( m_self ) ;
	self.Get()["Type"]		= Name( "Page" ) ;
 	self.Get()["Contents"]	= WriteContent( file ) ;
	self.Get()["Resources"]	= GetResource( )->Write( file ) ;
	self.Get()["Parent"]	= parent ;

    if ( m_media_box != Rect() )
    	self.Get()["MediaBox"] = Array(
    		m_media_box.begin( ),
    		m_media_box.end( ) ) ;

	self.Write( file, link ) ;
}

Object RealPage::WriteContent( IFile *file ) const
{
	assert( file != 0 ) ;

	if ( m_cstrs.empty() )
	{
		Stream s ;
		m_content.Write( s, &m_resources ) ;
		return file->WriteObj( s ) ;
	}
	else if ( m_cstrs.size() == 1 )
		return file->WriteObj( m_cstrs.front() ) ;
	else
	{
		Array strs( m_cstrs.size() ) ;
		std::transform(
			m_cstrs.begin(),
			m_cstrs.end(),
		    strs.begin(),
		    boost::bind( &IFile::WriteObj, file, _1 ) ) ;
		return strs ;
	}
}

/*
void RealPage::DrawText( double x, double y, Font *f, const std::string& text )
{
	assert( f != 0 ) ;
	assert( GetResource( ) != 0 ) ;

	BaseFont *font = dynamic_cast<BaseFont*>( f ) ;
	assert( font != 0 ) ;
	Name fname = m_resources.AddFont( font ) ;

	if ( m_cstrs.empty() || !m_cstrs.back().IsDirty() )
		m_cstrs.push_back( Stream( Stream::deflate ) ) ;

	std::ostream ss( m_cstrs.back().OutStreamBuf( ) ) ;
	ss << "BT\n"
       << fname << " 12 Tf " << x << ' ' << y << " Td "
	            << String( text ) << " Tj\n"
	   << "ET\n" << std::flush ;
}
*/

void RealPage::UseFont( Font *f )
{
	BaseFont *font = dynamic_cast<BaseFont*>( f ) ;
	assert( font != 0 ) ;
	m_resources.AddFont( font ) ;
}

/*
void RealPage::Finish( )
{
	if ( !m_cstrs.empty() )
		m_cstrs.back().Flush( ) ;
}
*/

std::size_t RealPage::Count( ) const
{
	return 1 ;
}

PageTree* RealPage::Parent( )
{
	return m_parent ;
}

PageNode* RealPage::GetLeaf( std::size_t index )
{
	// we are a leaf node. we have no children but ourselves.
	return index == 0 ? this : 0 ;
}

RealResources* RealPage::GetResource( )
{
	return &m_resources ;
}

const RealResources* RealPage::GetResource( ) const
{
	return &m_resources ;
}

///	Get the contents of a page
/**	This function will decode the page content when called. Decoding page
	content takes some time, so unless the user calls this function, the
	content won't be decode when creating the page object.
*/
RealContent* RealPage::GetContent( )
{
	if ( m_content.IsEmpty() )
	{
		// decode the graphics commands
		m_content.Load( m_cstrs.begin(), m_cstrs.end(), &m_resources ) ;
		
		// destroy the streams
		m_cstrs.clear( ) ;
	}
	
	return &m_content ;
}

} // end of namespace
