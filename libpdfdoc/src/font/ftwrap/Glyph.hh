/***************************************************************************\
 *   Copyright (C) 2002 by Nestal Wan                                      *
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
/*!
	\file	Glyph.hh
	\brief	the freetype glyph wrapper
	\date	2002-06-16
	\author	Nestal Wan
*/

#ifndef __FREETYPE_GLYPH_HEADER_INCLUDED__
#define __FREETYPE_GLYPH_HEADER_INCLUDED__

#include <memory>

namespace ft {

class Face ;

/*!	\brief	Freetype Glyph class
	
	This class represents a glyph in a font. A glyph is the graphical
	representation of a character is a font. This class provides an interface
	to retrieve the metrics and render the glyph.
*/
class Glyph
{
private :
	struct Body ;
	const std::auto_ptr<Body> m_body ;

public :
	Glyph( const Face *face, unsigned int glyph_index ) ;
	~Glyph( ) ;

	// metrics	
	double Width( ) const ;
	double Height( ) const ;
	double HoriAdvance( ) const ;
	double VertAdvance( ) const ;
} ;

} // end of namespace

#endif
