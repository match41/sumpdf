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

/**	\file	PageInfo.cc
	\brief	implementation of the PageInfo class
	\date	Feb 13, 2010
	\author	Nestal Wan
*/

#include "PageInfo.hh"

#include "PageTree.hh"
#include "RealResources.hh"

#include "core/Array.hh"
#include "core/Dictionary.hh"
#include "core/Ref.hh"
#include "file/DictReader.hh"
#include "file/ElementPool.hh"
#include "file/File.hh"
#include "util/Debug.hh"

namespace pdf {

namespace
{
	double default_page_size[4] = { 0, 0, 595.1, 842.1 } ;
}

/**	constructor
	
*/
PageInfo::PageInfo( PageTree *parent )
	: m_parent( parent )
	, m_res( parent == 0 ? 0 : parent->Resource() )
	, m_media_box( default_page_size )
	, m_rotate( 0 )
{
}

PageInfo::PageInfo( FontDb *fontdb )
	: m_parent( 0 )
	, m_res( new RealResources( fontdb ), false )
	, m_media_box( default_page_size )
	, m_crop_box( m_media_box )
	, m_rotate( 0 )
{
}

PageInfo::~PageInfo( )
{
}

void PageInfo::Read( DictReader& dict )
{
	PDF_ASSERT( dict.GetFile() != 0 ) ;
	PDF_ASSERT( m_res != 0 ) ;

	// media box
	if ( !dict.Detach( "MediaBox", m_media_box ) && m_parent != 0 )
		m_media_box = m_parent->MediaBox() ;

	// crop box
	if ( !dict.Detach( "CropBox", m_crop_box ) && m_parent != 0 )
		m_crop_box = m_parent->CropBox() ;

	ElementPool *pool = dict.GetFile()->Pool( ) ;
	PDF_ASSERT( pool != 0 ) ;
	
	// try reading the resources
	RealResources *res = dict.Create( "Resources",
		boost::bind( &PageInfo::ReadResource, this, _1 ) ) ;
	
	// if no resource, use parent or creates an empty one if no parent
	if ( res == 0 )
		res = m_parent != 0 ? m_parent->Resource() : CreateNewResource( ) ;
	
	// replace the old resources
	PDF_ASSERT( res != 0 ) ;
	m_res = res ;

	if ( !dict.Detach( "Rotate", m_rotate ) && m_parent != 0 )
		m_rotate = m_parent->Rotation( ) ;
}

RealResources* PageInfo::ReadResource( DictReader& dict )
{
	RealResources *res = CreateNewResource( ) ;
	res->Read( dict ) ;
	return res ;
}

void PageInfo::Write(
	Dictionary& 			dict,
	File 					*file,
	const FontSubsetInfo	*ss ) const
{
	PDF_ASSERT( file != 0 ) ;
	PDF_ASSERT( m_res != 0 ) ;
	
	ElementPool *pool = file->Pool() ;
	PDF_ASSERT( pool != 0 ) ;
	
	Ref ref = pool->Find( m_res.get() ) ;
	if ( ref == Ref() )
	{
		ref = m_res->Write( file, ss ) ;
		pool->Add( ref, m_res.get() ) ;
	}
	
	// write resources as an indirect reference
	PDF_ASSERT( ref != Ref() ) ;
	dict.insert( "Resources", ref ) ;

    if ( (m_parent == 0 || m_media_box != m_parent->MediaBox()) &&
	     !m_media_box.IsNull() )
    	dict.insert( "MediaBox", m_media_box ) ;

    if ( (m_parent == 0 || m_crop_box != m_parent->CropBox()) &&
	     !m_crop_box.IsNull() )
    	dict.insert( "CropBox", m_crop_box ) ;

    // default value is zero, so no need to write if zero.
    if ( m_rotate != 0 )
    	dict.insert( "Rotate", m_rotate ) ;
}

RealResources* PageInfo::Resource( )
{
	PDF_ASSERT( m_res != 0 ) ;
	return m_res.get() ;
}

const RealResources* PageInfo::Resource( ) const
{
	PDF_ASSERT( m_res != 0 ) ;
	return m_res.get() ;
}

RealResources* PageInfo::CreateNewResource( )
{
	PDF_ASSERT( m_parent != 0 || m_res != 0 ) ;
	return m_parent != 0 ? new RealResources( m_parent->Resource() )
		: new RealResources( m_res->FontDatabase( ) ) ;
}

void PageInfo::ReplaceResource( RealResources *res )
{
	PDF_ASSERT( m_res != 0 ) ;
	m_res.reset( res ) ;
}

PageTree* PageInfo::Parent( )
{
	return m_parent ;
}

Rect PageInfo::MediaBox() const
{
	return m_media_box != Rect() ? m_media_box :
		(m_parent != 0 ? m_parent->MediaBox() : m_media_box ) ;
}

/** According to PDF spec, the default value of the crop box is the same as
	the media box.
*/
Rect PageInfo::CropBox() const
{
	return m_crop_box != Rect() ? m_crop_box : MediaBox() ;
}

int PageInfo::Rotation( ) const
{
	return m_rotate ;
}

} // end of namespace
