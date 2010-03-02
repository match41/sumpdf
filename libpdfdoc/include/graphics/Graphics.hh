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
    \file	Graphics.hh
    \brief	definition the Graphics class
    \date	Jan 4, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_GRAPHICS_HH_EADER_INCLUDED__
#define __PDF_GRAPHICS_HH_EADER_INCLUDED__

#include <iosfwd>
#include <cstddef>

namespace pdf {

class ContentOp ;
class GraphicsVisitor ;
class GraphicsState ;
class ResourcesDict ;

/**	\defgroup	graphics Graphics
	\brief		Graphics objects module
	
	The graphics objects represent visual elements that appears in the page.
	These classes provide access to their content.
	
	\section 	gfx_coord	Coordinate Systems
	
	There are a number of coordinate system defined by the PDF specifications.
	
	\subsection	user_coord	User Space
	
	All graphics items in a PDF document uses the default coordinate called
	"User Space". It is a devices independent unit. The default unit in users
	space is point, which is 1/72 inch. In other words, the coordinate (72,72)
	is corresponding to one inch right and inch up from the origin.
	
	\subsection	text_coord	Text Space
	
	The Text Space is the coordinate system for text rendering. The
	transformation from user space to text space is defined a text matrix and
	a few text states parameters. There are a number of operators to manipulate
	the text matrix.
	
	\subsection	glyph_coord	Glyph Space
	
	Typically equals to 1/1000 units of text space, except for type 3 fonts.
	
	\subsection font_unit	Font Unit
	
	This one is not defined by the PDF specification. It is the unit used in
	TrueType font and Type 1 fonts. Actually, the physical length of a font
	unit depends on the font itself. Different fonts uses different units.
*/

///	brief description
/**	\ingroup graphics
	The Graphics class represent base class for anything that appears in the
	page.
*/
class Graphics
{
public :
	virtual ~Graphics( ) ;
	
	virtual void OnCommand( ContentOp& op, const ResourcesDict *res ) = 0 ;

	virtual void Print( std::ostream& os, const ResourcesDict *res ) const = 0 ;

	virtual void Visit( GraphicsVisitor *visitor ) = 0 ;
	
	virtual GraphicsState GetState( ) const = 0 ;
} ;

} // end of namespace

#endif // GRAPHICS_HH_
