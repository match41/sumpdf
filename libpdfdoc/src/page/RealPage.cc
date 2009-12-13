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
	\file	RealPage.cc
	\brief	implementation the RealPage class
	\date	Sat Mar 15 2008
	\author	Nestal Wan
*/

#include "RealPage.hh"

#include "PageTree.hh"

#include "core/Array.hh"
#include "core/Object.hh"
#include "core/String.hh"
#include "core/Token.hh"
#include "core/TokenSrc.hh"

#include "file/ObjectReader.hh"
#include "file/IFile.hh"

#include "font/BaseFont.hh"

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
	  m_rotate( 0 )
{
	if ( parent )
		parent->AppendLeaf( this ) ;
}

RealPage::RealPage( PageTree *parent, const Dictionary& self, IFile *file )
    : m_parent( parent ),
      m_self( self ),
      m_rotate( 0 )
{
	assert( file != 0 ) ;

	if ( parent )
		parent->AppendLeaf( this ) ;

	// read content
	Object contents ;
	if ( Detach( file, m_self, "Contents", contents ) )
	    ReadContent( contents, file ) ;

	// media box
	Array a ;
	if ( Detach( file, m_self, "MediaBox", a ) )
		m_media_box = Rect( a.begin( ), a.end( ) ) ;
}

Rect RealPage::MediaBox( ) const
{
	return m_media_box ;
}

void RealPage::ReadContent( const Object& str_obj, IFile *src )
{
	// for indirect objects, dereference it
	if ( str_obj.IsType<Ref>( ) )
		ReadContent( src->ReadObj( str_obj ), src ) ;

	// append individual stream objects
	else if ( str_obj.IsType<Stream>( ) )
		DecodeContent( str_obj.As<Stream>( ) ) ;

	// catenate individual objects in array
	else if ( str_obj.IsType<Array>( ) )
	{
		const Array& a = str_obj.As<Array>( ) ;
		std::for_each( a.begin( ), a.end( ),
		               boost::bind( &RealPage::ReadContent, this, _1, src ) ) ;
	}

	else if ( !str_obj.IsNull( ) )
		throw std::runtime_error( "invalid page content" ) ;
}

void RealPage::Write( const Ref& link, IFile *file, const Ref& parent ) const
{
	assert( file != 0 ) ;
	assert( m_parent != 0 ) ;
	assert( GetResource( ) != 0 ) ;

	Dictionary self( m_self ) ;
	self["Type"]		= Name( "Page" ) ;
// 	self["Contents"]    = file->WriteObj( Stream( m_content.str( ) ) ) ;
	self["Resources"]   = GetResource( )->Write( file ) ;
	self["Parent"]   	= parent ;

    if ( m_media_box != Rect() )
    	self["MediaBox"] = Array( m_media_box.begin( ), m_media_box.end( ) ) ;

	file->WriteObj( self, link ) ;
}

void RealPage::DrawText( double x, double y, Font *f, const std::string& text )
{
	assert( f != 0 ) ;
	assert( GetResource( ) != 0 ) ;

	BaseFont *font = dynamic_cast<BaseFont*>( f ) ;
	assert( font != 0 ) ;
	Name fname = GetResource( )->AddFont( font ) ;
	
	if ( m_content.empty() )
		m_content.push_back( Stream() ) ;

//	m_content.front()
/*
	m_content << "BT\n"
              << fname << " 12 Tf " << x << ' ' << y << " Td "
	                   << String( text ) << " Tj\n"
	          << "ET\n" ;
*/
}

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
	// we are a leaf node. we have no children but ourself.
	return index == 0 ? this : 0 ;
}

void RealPage::DecodeContent( const Stream& s )
{
	m_content.push_back( s ) ;
/*
	TokenSrc src( s.InStream() ) ;
	std::vector<Object> args ;

	while ( true )
	{
		Token  cmd ;
		Object obj ;

		if ( src >> obj )
			args.push_back( obj ) ;

		else
		{
			src.ResetState( ) ;
			if ( src >> cmd )
			{
				m_contents.push_back( PaintOp( cmd.Get(),
				                               args.empty() ? 0 : &args[0],
				                               args.size() ) ) ;
				args.clear( ) ;
			}
			else
				break ;
		}
	}
*/
}

} // end of namespace
