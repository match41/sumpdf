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

/**	constructor
	
*/
PageInfo::PageInfo( PageTree *parent )
	: m_parent( parent ),
	  m_res( new RealResources( parent == 0 ? 0 : parent->GetResource() ) ),
	  m_rotate( 0 )
{
}

PageInfo::PageInfo( FontDb *fontdb )
	: m_parent( 0 ),
	  m_res( new RealResources( fontdb ) ),
	  m_rotate( 0 )
{
}

PageInfo::~PageInfo( )
{
	PDF_ASSERT( m_res != 0 ) ;
	m_res->Release() ;
}

void PageInfo::Read( DictReader& dict )
{
	PDF_ASSERT( dict.GetFile() != 0 ) ;
	PDF_ASSERT( m_res != 0 ) ;

	// media box
	Array mbox ;
	if ( dict.Detach( "MediaBox", mbox ) )
		m_media_box = Rect( mbox.begin( ), mbox.end( ) ) ;
	else if ( m_parent != 0 )
		m_media_box = m_parent->MediaBox() ;

	// crop box
	Array cbox ;
	if ( dict.Detach( "CropBox", cbox ) )
		m_crop_box = Rect( cbox.begin( ), cbox.end( ) ) ;
	else if ( m_parent != 0 )
		m_crop_box = m_parent->CropBox() ;

	ElementPool *pool = dict.GetFile()->Pool( ) ;
	PDF_ASSERT( pool != 0 ) ;
	
	// reading the resources
	Ref link = dict["Resources"].To<Ref>( std::nothrow ) ;
	if ( !pool->Acquire( link, m_res ) )  
	{
		DictReader res_dict ;
		if ( dict.Detach( "Resources", res_dict ) )
		{
			m_res->Read( res_dict ) ;
			pool->Add( link, m_res ) ; 
		}
		else if ( m_parent != 0 )
		{
			// free our own first
			m_res->Release( ) ;
		
			m_res = m_parent->GetResource() ;
			m_res->AddRef( ) ;
		}
	}
	
	if ( !dict.Detach( "Rotate", m_rotate ) && m_parent != 0 )
		m_rotate = m_parent->Rotation( ) ;
}

void PageInfo::Write( Dictionary& dict, File *file ) const
{
	PDF_ASSERT( file != 0 ) ;
	PDF_ASSERT( m_res != 0 ) ;
	
	ElementPool *pool = file->Pool() ;
	PDF_ASSERT( pool != 0 ) ;
	
	Ref ref = pool->Find( m_res ) ;
	if ( ref == Ref() )
	{
		ref = m_res->Write( file ) ;
		pool->Add( ref, m_res ) ;
	}
	
	// write resources as an indirect reference
	PDF_ASSERT( ref != Ref() ) ;
	dict["Resources"]	= ref ;

    if ( (m_parent == 0 || m_media_box != m_parent->MediaBox()) &&
	     m_media_box != Rect() )
    	dict["MediaBox"] = Array( m_media_box.begin( ), m_media_box.end( ) ) ;

    if ( (m_parent == 0 || m_crop_box != m_parent->CropBox()) &&
	     m_crop_box != Rect() )
    	dict["CropBox"] = Array( m_crop_box.begin( ), m_crop_box.end( ) ) ;

    // default value is zero, so no need to write if zero.
    if ( m_rotate != 0 )
    	dict["Rotate"]	= m_rotate ;
}

RealResources* PageInfo::GetResource( )
{
	PDF_ASSERT( m_res != 0 ) ;
	return m_res ;
}

const RealResources* PageInfo::GetResource( ) const
{
	PDF_ASSERT( m_res != 0 ) ;
	return m_res ;
}

PageTree* PageInfo::Parent( )
{
	return m_parent ;
}

Rect PageInfo::MediaBox() const
{
	return m_media_box ;
}

Rect PageInfo::CropBox() const
{
	return m_crop_box ;
}

int PageInfo::Rotation( ) const
{
	return m_rotate ;
}

} // end of namespace
