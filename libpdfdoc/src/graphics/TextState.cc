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

/**
	\file	TextState.cc
	\brief	implementation of the TextFormat class
	\date	Jan 4, 2010
	\author	Nestal Wan
*/

#include "graphics/TextState.hh"

// TODO: resolve cyclic dependency
#include "page/Resources.hh"

namespace pdf {

/**	Constructor.
*/
TextState::TextState( )
	: m_leading( 0 )
{
}

///	Returns the leading.
/**	The leading parameter is the vertical distance between the baselines
	of adjacent lines of text. It always applies to the vertical coordinate
	in text space, regardless of the writing mode.
	
	\return	The leading of current text state.
	\sa SetLeading()
*/
double TextState::Leading( ) const
{
	return m_leading ;
}

void TextState::SetLeading( double val )
{
	m_leading = val ;
}

void TextState::SetFont( double size, Font *font )
{
	m_font_size	= size ;
	m_font		= font ;
}

} // end of namespace
