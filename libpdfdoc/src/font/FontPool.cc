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

/**	\file	FontPool.cc
	\brief	implementation of the FontPool class
	\date	Jan 17, 2010
	\author	Nestal Wan
*/

#include "FontPool.hh"

#include "core/Ref.hh"
#include "core/Object.hh"
#include "stream/Stream.hh"
#include "file/IFile.hh"

#include "util/Util.hh"
#include "util/Debug.hh"
#include "util/Exception.hh"

#include <boost/bind.hpp>

#include <algorithm>

namespace pdf {

/**	constructor
*/
FontPool::FontPool( FT_Library lib )
	: m_ft( lib )
{
	PDF_ASSERT( m_ft != 0 ) ;

	FT_Error e = FTC_Manager_New(
		m_ft, 0, 0, 0, &FontPool::RequestFace, this, &m_mgr ) ;
	if ( e != 0 )
		throw Exception( "cannot create FTC manager" ) ;
}

FontPool::~FontPool( )
{
	FTC_Manager_Done( m_mgr ) ;
	
    std::for_each(
        m_ref_map.begin(),
        m_ref_map.end(),
        boost::bind( DeletePtr(),
            boost::bind( &RefFaceMap::value_type::second, _1 ) ) ) ;

    std::for_each(
        m_name_map.begin(),
        m_name_map.end(),
        boost::bind( DeletePtr(),
            boost::bind( &NameFaceMap::value_type::second, _1 ) ) ) ;
}

FT_Face FontPool::GetFace( const Ref& ref, IFile *file )
{
	PDF_ASSERT( file != 0 ) ;

	RefFaceMap::iterator i = m_ref_map.find( ref ) ;
	if ( i == m_ref_map.end() )
	{
		// first create new entry in the map
		FaceID *fid = new FaceID ;
		Stream s = file->ReadObj( ref ).As<Stream>() ;
		s.CopyData( fid->data ) ;

		i = m_ref_map.insert( std::make_pair( ref, fid ) ).first ; 
	}

	PDF_ASSERT( i != m_ref_map.end() ) ;
	PDF_ASSERT( i->second != 0 ) ;
	
	return LookUpFace( i->second ) ;
}

FT_Face FontPool::LookUpFace( FaceID *face_id )
{
	PDF_ASSERT( face_id != 0 ) ;

	FT_Face face ;
	FT_Error e = FTC_Manager_LookupFace(
		m_mgr,
		reinterpret_cast<FTC_FaceID>( face_id ),
		&face ) ;
	if ( e != 0 )
		throw Exception( "create load font face" ) ;
	
	return face ;
}

FT_Face FontPool::GetFace(
	const std::string& font_name,
	const unsigned char *data,
	std::size_t size )
{
	PDF_ASSERT( !font_name.empty() ) ;
	PDF_ASSERT( size == 0 || data != 0 ) ;

	NameFaceMap::iterator i = m_name_map.find( font_name ) ;
	if ( i == m_name_map.end() )
	{
		// first create new entry in the map
		FaceID *fid = new FaceID ;
		fid->data.assign( data, data + size ) ;

		i = m_name_map.insert( std::make_pair( font_name, fid ) ).first ; 
	}

	PDF_ASSERT( i != m_name_map.end() ) ;
	PDF_ASSERT( i->second != 0 ) ;
	
	return LookUpFace( i->second ) ;
}

FT_Glyph FontPool::GetGlyph( FT_Face face, wchar_t ch )
{
	int idx = FTC_CMapCache_Lookup( m_cmap, 0, 0, ch ) ;
	
	return 0 ;
}

FT_Error FontPool::RequestFace(
	FTC_FaceID	id,
	FT_Library	library,
	FT_Pointer	request_data,
	FT_Face		*face )
{
	FaceID *face_id = reinterpret_cast<FaceID*>( id ) ;
	PDF_ASSERT( face_id != 0 ) ;
	
	const std::vector<unsigned char>& font_file = face_id->data ;

	return FT_New_Memory_Face(
		library,
		&font_file[0],
		font_file.size(),
		0,
		face ) ;
}

} // end of namespace
