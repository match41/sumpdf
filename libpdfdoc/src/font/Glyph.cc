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

/**	\file	Glyph.cc
	\brief	implementation of the Glyph class
	\date	Jan 24, 2010
	\author	Nestal Wan
*/

#include "font/Glyph.hh"
#include "font/Outline.hh"

#include "FreeTypeWrappers.hh"
#include "FontException.hh"

#include "util/Debug.hh"

#include <boost/format.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_IMAGE_H
#include FT_OUTLINE_H

namespace pdf {

struct Glyph::Impl
{
	FT_Glyph			glyph ;
	FT_Glyph_Metrics	met ;
} ;

namespace {

int MoveTo( const FT_Vector* to, void *user )
{
	Outline *out = reinterpret_cast<Outline*>( user ) ;
	PDF_ASSERT( out != 0 ) ;
	out->MoveTo( to->x, to->y ) ;
	return 0 ;
}

int LineTo( const FT_Vector* to, void *user )
{
	Outline *out = reinterpret_cast<Outline*>( user ) ;
	PDF_ASSERT( out != 0 ) ;
	out->LineTo( to->x, to->y ) ;
	return 0 ;
}

int QuadTo(
	const FT_Vector	*control,
	const FT_Vector	*to,
	void 			*user )
{
	Outline *out = reinterpret_cast<Outline*>( user ) ;
	PDF_ASSERT( out != 0 ) ;
	out->QuadTo( control->x, control->y, to->x, to->y ) ;
	return 0 ;
}

int CubicTo(
	const FT_Vector	*c1,
	const FT_Vector	*c2,
	const FT_Vector	*to,
	void 			*user )
{
	Outline *out = reinterpret_cast<Outline*>( user ) ;
	PDF_ASSERT( out != 0 ) ;
	out->CubicTo( c1->x, c1->y, c2->x, c2->y, to->x, to->y ) ;
	return 0 ;
}

} // end of anonymous namespace

/**	constructor
*/
Glyph::Glyph( unsigned idx, const ft::Face& face )
	: m_impl( new Impl )
{
	// load the glyph to the glyph slot in the face
	// we want to do the scaling in double instead of inside freetype
	// in small font we don't have hinting
	FT_Error error = FT_Load_Glyph( face.face, idx, FT_LOAD_NO_SCALE ) ;
	if ( error != 0 )
		throw FontException(
			boost::format( "cannot load glyph %1%" ) % idx ) ;

	error = FT_Get_Glyph( face.face->glyph, &m_impl->glyph ) ;
	if ( error != 0 )
		throw FontException(
			boost::format( "cannot copy glyph %1%" ) % idx ) ;

	m_impl->met = face.face->glyph->metrics ;
}

Glyph::~Glyph( )
{
	/// TODO: not thread-safe here
	if ( m_impl.unique() )
		FT_Done_Glyph( m_impl->glyph ) ;
}

/// Width in un-scaled font unit.
unsigned Glyph::Width( ) const
{
	return m_impl->met.width ;
}

/// Height in un-scaled font unit.
unsigned Glyph::Height( ) const
{
	return m_impl->met.height ;
}

/// Horizonal advance in un-scaled font unit.
unsigned Glyph::AdvanceX( ) const
{
	return m_impl->met.horiAdvance ;
}

/// Vertical advance in un-scaled font unit.
unsigned Glyph::AdvanceY( ) const
{
	return m_impl->met.vertAdvance ;
}

bool Glyph::Decompose( Outline *outline ) const
{
	if ( m_impl->glyph->format == FT_GLYPH_FORMAT_OUTLINE )
	{
		FT_Outline_Funcs f =
		{
			&MoveTo,
			&LineTo,
			&QuadTo,
			&CubicTo,
			0, 0
		} ;
		
		FT_OutlineGlyph og = reinterpret_cast<FT_OutlineGlyph>( m_impl->glyph ); 
		return FT_Outline_Decompose( &og->outline, &f, outline ) == 0 ;
	}
	else
		return false ;
}

bool Glyph::IsOutline( ) const
{
	return m_impl->glyph->format == FT_GLYPH_FORMAT_OUTLINE ;
}

} // end of namespace
