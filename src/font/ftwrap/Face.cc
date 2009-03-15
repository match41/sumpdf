/***************************************************************************\
 *   Copyright (C) 2002 by Nestal Wan                                      *
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
\***************************************************************************/
/*!
	\file	Face.cc
	\brief	implementation of the FreeType wrappers
	\date	2002-01-11
	\author	Nestal Wan
*/

// class headers
#include "Face.hh"
#include "FaceBody.hh"

// other class headers
#include "LibraryBody.hh"
#include "Exception.hh"
#include "Types.hh"

// freetype headers
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_TRUETYPE_TABLES_H

// standard headers
#include <cassert>

// local functions
namespace {

inline TT_PCLT* PcltTable( FT_Face face )
{
	TT_PCLT *table = reinterpret_cast<TT_PCLT*>(
		FT_Get_Sfnt_Table( face, ft_sfnt_pclt ) ) ;

	if ( table == 0 )
		throw freetype::Exception( ) ;
	else
		return table ;
}

inline TT_Postscript* PostTable( FT_Face face )
{
	TT_Postscript *table = reinterpret_cast<TT_Postscript*>(
		FT_Get_Sfnt_Table( face, ft_sfnt_post ) ) ;

	if ( table == 0 )
		throw freetype::Exception( ) ;
	else
		return table ;
}

}

namespace freetype {

/*!	construct a font face.
	\param	lib		a pointer to the freetype library object. when the
					library object is destroyed, the font face object
					is destroyed by freetype too.
	\param	filename	the filename of the font file
	\param	index		the index of the font in the font file. one single
						font file can contain many font faces.
*/
Face::Face( Library *lib, const std::string& filename, long index )
	: m_pimpl( new Body )
{
	// assert parameters
	assert( lib != 0 ) ;
	assert( lib->Pimpl( ) != 0 ) ;
	assert( lib->Pimpl( )->GetBody( ) != 0 ) ;
	assert( !filename.empty( ) ) ;
	assert( index >= 0 ) ;
	
	FT_Error error = ::FT_New_Face( lib->Pimpl( )->GetBody( ),
		                            filename.c_str( ),
		                            index, &m_pimpl->m_face ) ;

	// should throw something better later
	if ( error )
		throw Exception( ) ;
	
	assert( IsValid( ) ) ;
}

Face::Face( Library *lib, const unsigned char *buffer, std::size_t size,
	        long index )
	: m_pimpl( new Body )
{
	// assert parameters
	assert( lib != 0 ) ;
	assert( lib->Pimpl( ) != 0 ) ;
	assert( lib->Pimpl( )->GetBody( ) != 0 ) ;
	assert( buffer != 0 ) ;
	assert( size > 0 ) ;
	assert( index >= 0 ) ;
	
	FT_Error error = ::FT_New_Memory_Face( lib->Pimpl( )->GetBody( ),
		                                   buffer, size, index,
		                                   &m_pimpl->m_face ) ;

	// should throw something better later
	if ( error )
		throw Exception( ) ;
	
	assert( IsValid( ) ) ;
}

Face::~Face( )
{
	assert( m_pimpl.get( ) != 0 ) ;
	if ( m_pimpl->m_face != 0 )
		::FT_Done_Face( m_pimpl->m_face ) ;
}

#ifndef NDEBUG
bool Face::IsValid( ) const
{
	return m_pimpl.get( ) != 0 && m_pimpl->m_face != 0 ;
}
#endif

const std::string Face::Family( ) const
{
	assert( IsValid( ) ) ;

	return m_pimpl->m_face->family_name ; ;
}

const std::string Face::Style( ) const
{
	assert( IsValid( ) ) ;

	return m_pimpl->m_face->style_name ;
}

const std::string Face::PSName( ) const
{
	assert( IsValid( ) ) ;

	return FT_Get_Postscript_Name( m_pimpl->m_face ) ;
}

int Face::Ascent( ) const
{
	assert( IsValid( ) ) ;

	return m_pimpl->m_face->ascender ;
}

int Face::Decent( ) const
{
	assert( IsValid( ) ) ;

	return m_pimpl->m_face->descender ;
}

int Face::UnitsPerEM( ) const
{
	assert( IsValid( ) ) ;

	return m_pimpl->m_face->units_per_EM ;
}

std::size_t Face::GlyphCount( ) const
{
	assert( IsValid( ) ) ;

	return m_pimpl->m_face->num_glyphs ;
}

std::size_t Face::FixedCount( ) const
{
	assert( IsValid( ) ) ;

	return m_pimpl->m_face->num_fixed_sizes ;
}

void Face::BoundBox( int bbox[] ) const
{
	assert( IsValid( ) ) ;

	bbox[0] = m_pimpl->m_face->bbox.xMin ;
	bbox[1] = m_pimpl->m_face->bbox.yMin ;
	bbox[2] = m_pimpl->m_face->bbox.xMax ;
	bbox[3] = m_pimpl->m_face->bbox.yMax ;
}

int Face::XHeight( ) const
{
	assert( IsValid( ) ) ;

	return PcltTable( m_pimpl->m_face )->xHeight ;
}

int Face::CapHeight( ) const
{
	assert( IsValid( ) ) ;

	return PcltTable( m_pimpl->m_face )->CapHeight ;
}

double Face::ItalicAngle( ) const
{
	assert( IsValid( ) ) ;

	return FtFixed2Double( PostTable( m_pimpl->m_face )->italicAngle ) ;
}

bool Face::IsFixed( ) const
{
	assert( IsValid( ) ) ;
	return FT_IS_FIXED_WIDTH( m_pimpl->m_face ) ;
}

/*!	set font face size. see FT_Set_Char_Size() for details.

	\param	width	char width in points. zero means same as height.
	\param	height	char height in points. zero means same as width.
	\param	hres	horizonal resolution in DPI.
	\param	vres	vertical resolution in DPI.
*/
void Face::SetSize( double width, double height, int hres, int vres )
{
	assert( IsValid( ) ) ;
	
	FT_Error result = FT_Set_Char_Size( m_pimpl->m_face,
	                                    static_cast<FT_F26Dot6>( width * 64.0 ),
										static_cast<FT_F26Dot6>( height * 64.0),
										hres, vres ) ;
	if ( result != 0 )
		throw Exception( ) ;
}

unsigned int Face::GetGlyphCode( unsigned long char_code ) const
{
	assert( IsValid( ) ) ;
	
	return FT_Get_Char_Index( m_pimpl->m_face, char_code ) ;
}

unsigned long Face::GetFirstChar( unsigned& glyph_idx ) const
{
	assert( IsValid( ) ) ;
	return ::FT_Get_First_Char( m_pimpl->m_face, &glyph_idx ) ;
}

unsigned long Face::GetNextChar( unsigned long current, 
					             unsigned& glyph_idx ) const
{
	assert( IsValid( ) ) ;
	return ::FT_Get_Next_Char( m_pimpl->m_face, current, &glyph_idx ) ;
}

} // end of namespace
