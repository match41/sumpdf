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

/**	\file	Glyph.hh
    \brief	definition the Glyph class
    \date	Jan 24, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_GLYPH_HEADER_INCLUDED__
#define __PDF_GLYPH_HEADER_INCLUDED__

struct FT_GlyphRec_ ;

namespace pdf {

class Outline ;

///	A glyph class.
/**	\ingroup font
	The Glyph class represents a glyph in an PDF document. A glyph is an image
	representation of a character in a font face. This class provides access
	to many information about a glyph, e.g. its metrics and outline rendering.
	The unit of the metrics provided by this class is always in font unit.
	The actual size of the font unit is defined by Font::UnitsPerEM().
	
	\sa	Font::UnitsPerEM().  
*/
class Glyph
{
protected :
	/// protected destructor to ensure no polymorphic deletion.
	~Glyph( ) ;

public :
	/// Width in un-scaled Freetype font unit.
	virtual unsigned Width( ) const = 0 ;
	
	/// Height in un-scaled Freetype font unit.
	virtual unsigned Height( ) const = 0 ;
	
	/// Horizonal advance in un-scaled Freetype font unit.
	virtual unsigned AdvanceX( ) const = 0 ;

	/// Vertical advance in un-scaled Freetype font unit.
	virtual unsigned AdvanceY( ) const = 0 ;

	///	Decompose the glyph outline.
	/**	This function walks through the glyph outline and decompose it into
		individual segment and Bezier arcs. The caller must implement the
		Outline interface and the corresponding callback function in the Outline
		interface will be called for each line segment and Bezier arcs in the
		glyph.
		
		\param	outline		A callback interface to each segment and arcs.
		\return	\c true if walk through successfully, otherwise \c false.
				It fails when the glyph is not an outline glyph.
	*/
	virtual bool Decompose( Outline *outline ) const = 0 ;
	
	///	Return true if the glyph is outline glyph, otherwise false.
	virtual bool IsOutline( ) const = 0 ;
} ;

} // end of namespace

#endif // GLYPH_HH_
