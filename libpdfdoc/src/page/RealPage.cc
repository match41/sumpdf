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

#include "ContentStream.hh"
#include "PageTree.hh"
#include "RealResources.hh"

// core object headers
#include "core/Array.hh"
#include "core/Object.hh"
#include "core/String.hh"

// other libpdfdoc headers
#include "file/File.hh"
#include "file/DictReader.hh"
#include "file/ElementPool.hh"
#include "graphics/Graphics.hh"
#include "util/Debug.hh"
#include "util/Rect.hh"
#include "util/Util.hh"

#include <boost/bind.hpp>

#include <algorithm>
#include <iostream>
#include <stdexcept>

namespace pdf {

RealPage::RealPage( PageTree *parent )
	: m_pinfo( parent )
{
	PDF_ASSERT( parent != 0 ) ;
	parent->AppendLeaf( this ) ;
}

void RealPage::Read( DictReader& dict )
{
	PDF_ASSERT( dict.GetFile() != 0 ) ;

	// read content
	Object contents ;
	if ( dict.Detach( "Contents", contents ) )
	    ReadContent( contents, dict.GetFile() ) ;

	m_pinfo.Read( dict ) ;
}

RealPage::~RealPage( )
{
}

Rect RealPage::MediaBox( ) const
{
	return m_pinfo.MediaBox() ;
}

Rect RealPage::CropBox( ) const
{
	return m_pinfo.CropBox() ;
}

void RealPage::ReadContent( const Object& str_obj, File *src )
{
	PDF_ASSERT( src != 0 ) ;
	
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

void RealPage::Write( const Ref& link, File *file, const Ref& parent ) const
{
	PDF_ASSERT( file != 0 ) ;
	
	Dictionary self ;
	self["Type"]		= Name( "Page" ) ;
 	self["Contents"]	= WriteContent( file ) ;
	self["Parent"]		= parent ;
	
	m_pinfo.Write( self, file ) ;

	file->WriteObj( self, link ) ;
}

Object RealPage::WriteContent( File *file ) const
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
		    boost::bind( &File::WriteObj, file, _1 ) ) ;
		return strs ;
	}
}

void RealPage::Clear( )
{
	m_cstrs.clear( ) ;
}

std::size_t RealPage::Count( ) const
{
	return 1 ;
}

PageTree* RealPage::Parent( )
{
	return m_pinfo.Parent( ) ;
}

RealPage* RealPage::GetLeaf( std::size_t index )
{
	// we are a leaf node. we have no children but ourselves.
	return index == 0 ? this : 0 ;
}

RealResources* RealPage::GetResource( )
{
	return m_pinfo.GetResource() ;
}

const RealResources* RealPage::GetResource( ) const
{
	return m_pinfo.GetResource() ;
}

int RealPage::Rotation( ) const
{
	return m_pinfo.Rotation( ) ;
}

void RealPage::VisitGraphics( GraphicsVisitor *visitor ) const
{
	ContentStream cs(
		m_cstrs.begin(),
		m_cstrs.end(),
		m_pinfo.GetResource(),
		visitor ) ;
	cs.Decode( ) ;
}

void RealPage::SetContent( const std::vector<Graphics*>& gfx )
{
	Stream str ;
	std::ostream os( str.OutStreamBuf() ) ;

	using namespace boost ;
	std::for_each(
		gfx.begin(),
		gfx.end(),
		bind( &Graphics::Print, _1, ref(os), m_pinfo.GetResource() ) ) ;

	os.flush() ;
	
	m_cstrs.clear( ) ;
	m_cstrs.push_back( str ) ;
}

void RealPage::GetRawContent( std::vector<unsigned char>& out ) const
{
	if ( m_cstrs.size() == 1 )
		m_cstrs.front().CopyData( out ) ;
}

} // end of namespace
