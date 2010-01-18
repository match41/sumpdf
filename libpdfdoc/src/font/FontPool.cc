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
#include "file/IFile.hh"
#include "font/BaseFont.hh"
#include "font/FontDescriptor.hh"
#include "stream/Stream.hh"

#include "util/Debug.hh"
#include "util/Exception.hh"

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
	
	// TODO: free the FaceID's
//	std::for_each( m_face_map.begin(), m_face_map.end(),
//		
}

FT_Face FontPool::GetFace( const Ref& ref, IFile *file )
{
	PDF_ASSERT( file != 0 ) ;

	FaceMap::iterator i = m_face_map.find( ref ) ;
	if ( i == m_face_map.end() )
	{
		// first create new entry in the map
		FaceID *fid = new FaceID ;
		Stream s = file->ReadObj( ref ).As<Stream>() ;
		s.CopyData( fid->data ) ;

		i = m_face_map.insert( std::make_pair( ref, fid ) ).first ; 
	}

	PDF_ASSERT( i != m_face_map.end() ) ;

	FT_Face face ;
	FT_Error e = FTC_Manager_LookupFace(
		m_mgr,
		reinterpret_cast<FTC_FaceID>( &i->second ),
		&face ) ;
	if ( e != 0 )
		throw Exception( "create load font face" ) ;
	
	return face ;
}

FT_Glyph FontPool::GetGlyph( FT_Face face, wchar_t ch )
{
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
