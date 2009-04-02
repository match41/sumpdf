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
#include "core/Stream.hh"
#include "core/String.hh"
#include "core/Token.hh"
#include "core/TokenSrc.hh"

#include "file/IElementSrc.hh"
#include "file/IElementDest.hh"
#include "file/DeRef.hh"

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
    : PageNode( parent ),
      m_status( disk_newer ),
      m_rotate( 0 )
{
	if ( parent )
		parent->AppendLeaf( this ) ;
}

Rect RealPage::MediaBox( ) const
{
	return m_media_box ;
}

void RealPage::Read( const Ref& link, IElementSrc *repo )
{
	assert( repo != 0 ) ;

	m_self = repo->ReadObj( link ) ;
	m_status = sync ;
	
	// read content
	ReadContent( m_self["Contents"], repo ) ;
	m_self.erase( "Contents" ) ;
// 	DecodeContent( ) ;

	// resources may not always be indirect objects
	ReadResource( m_self["Resources"], repo ) ;
	
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
	if ( str_obj.Type( ) == Object::ref )
		ReadContent( src->ReadObj( str_obj ), src ) ;
	
	// append individual stream objects
	else if ( str_obj.Type( ) == Object::stream )
	{
		const Stream& s = str_obj.As<Stream>( ) ;
		s.ReadAll( m_content.rdbuf() ) ;
/*		m_content.rdbuf()->sputn(
			reinterpret_cast<const char*>( s.Data( ) ), s.Size( ) ) ;*/
	}
	
	// catenate individual objects in array
	else if ( str_obj.Type( ) == Object::array )
	{
		const Array& a = str_obj.As<Array>( ) ;
		std::for_each( a.begin( ), a.end( ),
		               boost::bind( &RealPage::ReadContent, this, _1, src ) ) ;
	}
	
	else if ( str_obj.Type( ) != Object::null )
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

	m_content << "BT\n"
              << fname << " 12 Tf " << x << ' ' << y << " Td "
	                   << String( text ) << " Tj\n"
	          << "ET\n" ;
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

void RealPage::DecodeContent( )
{
	TokenSrc src( m_content ) ;
	while ( true )
	{
		Object obj ;
		Token  cmd ;
		
		if ( src >> obj )
		{
			std::cout << (std::size_t)m_content.tellg( ) << " is object: "
			          << obj << std::endl ;
		}
		else
		{
			src.ResetState( ) ;
			if ( src >> cmd )
			{
				std::cout << (std::size_t)m_content.tellg( )
				          << " got cmd: " << cmd.Get() << std::endl ;
			}
			else
				break ;
		}
	}
}

} // end of namespace
