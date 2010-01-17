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
		m_ft, 0, 0, 0, &FontPool::RequestFace, 0, &m_mgr ) ;
	if ( e != 0 )
		throw Exception( "cannot create FTC manager" ) ;
}

FontPool::~FontPool( )
{
	FTC_Manager_Done( m_mgr ) ;
}

FT_Face FontPool::GetFace( BaseFont *font )
{
	PDF_ASSERT( font != 0 ) ;

	FT_Face face ;
	FT_Error e = FTC_Manager_LookupFace(
		m_mgr,
		reinterpret_cast<FTC_FaceID>( font ),
		&face ) ;
	if ( e != 0 )
		throw Exception( "create load font face" ) ;
	return 0 ;
}

FT_Glyph FontPool::GetGlyph( FT_Face face, wchar_t ch )
{
	return 0 ;
}

FT_Error FontPool::RequestFace(
	FTC_FaceID	face_id,
	FT_Library	library,
	FT_Pointer	request_data,
	FT_Face		*aface )
{
	BaseFont *font = reinterpret_cast<BaseFont*>( face_id ) ;
	PDF_ASSERT( font != 0 ) ;
	return font != 0 ? 0 : -1 ;
}

} // end of namespace
