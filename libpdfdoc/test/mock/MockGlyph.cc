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

/**	\file	MockGlyph.cc
	\brief	implementation of the MockGlyph class
	\date	Jan 31, 2010
	\author	Nestal Wan
*/

#include "MockGlyph.hh"

namespace pdfut {

/**	constructor
*/
MockGlyph::MockGlyph( )
{
}

unsigned MockGlyph::Width( ) const
{
	return 500 ;
}

unsigned MockGlyph::Height( ) const
{
	return 1024 ;
}

unsigned MockGlyph::AdvanceX( ) const
{
	return 1024 ;
}

unsigned MockGlyph::AdvanceY( ) const
{
	return 0 ;
}

bool MockGlyph::Decompose( pdf::Outline * ) const
{
	return false ;
}

bool MockGlyph::IsOutline( ) const
{
	return false ;
}

} // end of namespace
