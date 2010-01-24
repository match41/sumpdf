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

#include "FreeTypeWrappers.hh"
#include "FontException.hh"

#include <boost/format.hpp>

namespace pdf {

struct Glyph::Impl
{
	FT_Glyph			glyph ;
	FT_Glyph_Metrics	met ;
} ;


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

} // end of namespace
