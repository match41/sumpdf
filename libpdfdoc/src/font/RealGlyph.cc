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

/**	\file	RealGlyph.cc
	\brief	implementation of the RealGlyph class
	\date	Feb 1, 2010
	\author	Nestal Wan
*/

#include "font/RealGlyph.hh"
#include "font/Outline.hh"

#include "FreeTypeWrappers.hh"
#include "FontException.hh"

#include "util/Debug.hh"

#include <boost/format.hpp>

#include FT_IMAGE_H
#include FT_OUTLINE_H

namespace pdf {

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

///	Constructor.
/**	\internal Constructor a glyph given a font face and the glyph index.
	This function will load the glyph from the font face.
*/
RealGlyph::RealGlyph( unsigned idx, const ft::Face& face )
{
	// load the glyph to the glyph slot in the face
	// we want to do the scaling in double instead of inside freetype
	// in small font we don't have hinting
	FT_Error error = FT_Load_Glyph( face.face, idx, FT_LOAD_NO_SCALE ) ;
	if ( error != 0 )
		throw FontException(
			boost::format( "cannot load glyph %1%" ) % idx ) ;

	error = FT_Get_Glyph( face.face->glyph, &m_glyph ) ;
	if ( error != 0 )
		throw FontException(
			boost::format( "cannot copy glyph %1%" ) % idx ) ;

	m_met = face.face->glyph->metrics ;
}

RealGlyph::RealGlyph( const RealGlyph& rhs )
	: m_met( rhs.m_met )
{
	FT_Glyph_Copy( rhs.m_glyph, &m_glyph ) ;
}

RealGlyph::~RealGlyph( )
{
	FT_Done_Glyph( m_glyph ) ;
}

/// Width in un-scaled Freetype font unit.
unsigned RealGlyph::Width( ) const
{
	return m_met.width ;
}

/// Height in un-scaled Freetype font unit.
unsigned RealGlyph::Height( ) const
{
	return m_met.height ;
}

/// Horizonal advance in un-scaled Freetype font unit.
unsigned RealGlyph::AdvanceX( ) const
{
	return m_met.horiAdvance ;
}

unsigned RealGlyph::AdvanceY( ) const
{
	return m_met.vertAdvance ;
}

bool RealGlyph::Decompose( Outline *outline ) const
{
	if ( m_glyph->format == FT_GLYPH_FORMAT_OUTLINE )
	{
		FT_Outline_Funcs f =
		{
			&MoveTo,
			&LineTo,
			&QuadTo,
			&CubicTo,
			0, 0
		} ;
		
		FT_OutlineGlyph og = reinterpret_cast<FT_OutlineGlyph>( m_glyph ); 
		return FT_Outline_Decompose( &og->outline, &f, outline ) == 0 ;
	}
	else
		return false ;
}

///	Return \c true if the glyph is an outline glyph.
bool RealGlyph::IsOutline( ) const
{
	return m_glyph->format == FT_GLYPH_FORMAT_OUTLINE ;
}

} // end of namespace
