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
	\file	Glyph.cc
	\brief	implementation of the freetype glyph wrapper class
	\date	2002-07-06
	\auhtor	Nestal Wan
*/

// class header
#include "Glyph.hh"

// other class headers
#include "FaceBody.hh"
#include "Exception.hh"
#include "Types.hh"

// freetype headers
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

// standard headers
#include <cassert>

namespace ft {

struct Glyph::Body
{
	FT_Glyph         m_glyph ;
	FT_Glyph_Metrics m_metrics ;
} ;

Glyph::Glyph( const Face *face, unsigned int glyph_index )
	: m_body( new Body )
{
	assert( face != 0 ) ;
	assert( face->Pimpl( ) != 0 ) ;
	assert( m_body.get( ) != 0 ) ;

	// lazy shortcut
	FT_Face ftface = face->Pimpl( )->m_face ;

	// load the glyph to the glyph slot in the face
	FT_Error error = FT_Load_Glyph( ftface, glyph_index, FT_LOAD_DEFAULT ) ;
	if ( error != 0 )
		throw Exception( ) ;

	// copy the glyph metrics
	m_body->m_metrics = ftface->glyph->metrics ;

	// copy the glyph from the glyph slot to this
	error = FT_Get_Glyph( ftface->glyph, &m_body->m_glyph ) ;
	if ( error != 0 )
		throw Exception( ) ;
}

Glyph::~Glyph( )
{
	assert( m_body.get( ) != 0 ) ;
	FT_Done_Glyph( m_body->m_glyph ) ;
}

double Glyph::Width( ) const
{
	assert( m_body.get( ) != 0 ) ;
	return FtPos2Double( m_body->m_metrics.width ) ;
}

double Glyph::Height( ) const
{
	assert( m_body.get( ) != 0 ) ;
	return FtPos2Double( m_body->m_metrics.height ) ;
}

double Glyph::HoriAdvance( ) const
{
	assert( m_body.get( ) != 0 ) ;
	return FtPos2Double( m_body->m_metrics.horiAdvance ) ;
}

double Glyph::VertAdvance( ) const
{
	assert( m_body.get( ) != 0 ) ;
	return FtPos2Double( m_body->m_metrics.vertAdvance ) ;
}

} // end of namespace
