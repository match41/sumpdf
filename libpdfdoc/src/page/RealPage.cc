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
#include "core/Token.hh"
#include "core/TokenSrc.hh"

// other libpdfdoc headers
#include "file/ObjectReader.hh"
#include "file/IFile.hh"
#include "font/BaseFont.hh"
#include "page/PaintOp.hh"
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
	if ( str_obj.IsType<Ref>( ) )
		ReadContent( src->ReadObj( str_obj ), src ) ;

	// append individual stream objects
	else if ( str_obj.IsType<Stream>( ) )
		m_cstrs.push_back( str_obj.As<Stream>( ) ) ;

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

	if ( m_cstrs.size() == 1 )
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

void RealPage::Finish( )
{
	if ( !m_cstrs.empty() )
		m_cstrs.back().Flush( ) ;
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

Resources* RealPage::GetResource( )
{
	return &m_resources ;
}

const Resources* RealPage::GetResource( ) const
{
	return &m_resources ;
}

PageContent* RealPage::GetContent( )
{
	m_content.ops.clear( ) ;
	Decode( m_content.ops ) ;
	return &m_content ;
}

bool RealPage::Content::GetPaintOps( std::vector<PaintOp>& op )
{
	op = ops ;
	return true ;
}

void RealPage::Decode( std::vector<PaintOp>& ops )
{
	for ( std::vector<Stream>::iterator i = m_cstrs.begin( ) ;
	                                    i != m_cstrs.end( ) ; ++i )
	{
		// rewind to stream start for reading
		i->Rewind( ) ;
		
		std::istream s( i->InStreamBuf() ) ;
		TokenSrc src( s ) ;
		std::vector<Object> args ;
	
		while ( true )
		{
			Token  cmd ;
			Object obj ;
	
			if ( src >> obj )
			{
				// swapping is faster
				args.push_back( Object() ) ;
				obj.Swap( args.back() ) ;
			}
			else
			{
				src.ResetState( ) ;
				if ( src >> cmd )
				{
					ops.push_back(
						PaintOp(
							cmd.Get(),
							args.empty() ? 0 : &args[0],
							args.size(),
							&m_resources ) ) ;
					args.clear( ) ;
				}
				else
					break ;
			}
		}
	}
}

std::vector<Graphics*> RealPage::DecodeGraphic( )
{
    return std::vector<Graphics*>() ;
}

} // end of namespace
