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
	\file	TextFormat.cc
	\brief	implementation of the TextFormat class
	\date	Jan 4, 2010
	\author	Nestal Wan
*/

#include "TextFormat.hh"

namespace pdf {

/**	constructor
*/
TextFormat::TextFormat( )
	: m_leading( 0 )
{
}

/**	\brief	Returns the leading.

	The leading parameter is the vertical distance between the baselines
	of adjacent lines of text. It always applies to the vertical coordinate
	in text space, regardless of the writing mode.
	
	\return	The leading of current text state.
	\sa SetLeading()
*/
double TextFormat::Leading( ) const
{
	return m_leading ;
}

void TextFormat::SetLeading( double val )
{
	m_leading = val ;
}

} // end of namespace
