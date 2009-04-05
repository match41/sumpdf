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

#include "file/IElementSrc.hh"
#include "file/IElementDest.hh"
#include "file/DeRef.hh"

#include "font/BaseFont.hh"

#include "stream/Stream.hh"

#include "util/Rect.hh"
#include "util/Util.hh"

#include <boost/bind.hpp>

#include <algorithm>
#include <cassert>
#include <iostream>
#include <stdexcept>

namespace pdf {

RealPage::RealPage( PageTree *parent )
    : PageNode( parent ),
      m_rotate( 0 )
{
	if ( parent )
		parent->AppendLeaf( this ) ;
}

Rect RealPage::MediaBox( ) const
{
	return m_media_box ;
}

/*!	read a page from file. This function will read the page from file. It will
	also decode the stream content data.
*/
void RealPage::Init( Object& self, IElementSrc *repo )
{
	assert( repo != 0 ) ;
	PageNode::Init( self, repo ) ;

	m_self.clear( ) ;
	self.Swap( m_self ) ;
	
	// read content
	ReadContent( m_self["Contents"], repo ) ;
	m_self.erase( "Contents" ) ;

	// media box
	Array a ;
	if ( repo->Detach( m_self, "MediaBox", a ) )
		m_media_box = Rect( a.begin( ), a.end( ) ) ;
	
	// parent
	SetParent( repo->Read<PageTree>( m_self["Parent"] ) ) ;
}

void RealPage::ReadContent( const Object& str_obj, IElementSrc *src )
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

void RealPage::Write( const Ref& link, IElementDest *file ) const
{
	assert( Parent( ) != 0 ) ;
	assert( GetResource( ) != 0 ) ;
	assert( file != 0 ) ;

	Dictionary self( m_self ) ;
	self["Type"]		= Name( "Page" ) ;
// 	self["Contents"]    = file->WriteObj( Stream( m_content.str( ) ) ) ;
	self["Resources"]   = file->Write( GetResource( ) ) ;
	self["Parent"]   	= file->Write( Parent( ) ) ;
    
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


PageNode* RealPage::GetLeaf( std::size_t index )
{
	// we are a leaf node. we have no children but ourself.
	return index == 0 ? this : 0 ;
}

void RealPage::DecodeContent( const Stream& s )
{
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
}

} // end of namespace
