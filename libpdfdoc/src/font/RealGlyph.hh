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

/**	\file	RealGlyph.hh
    \brief	definition the RealGlyph class
    \date	Feb 1, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_REALGLYPH_HEADER_INCLUDED__
#define __PDF_REALGLYPH_HEADER_INCLUDED__

#include "font/Glyph.hh"
#include <boost/shared_ptr.hpp>

namespace pdf {

// freetype wrappers
namespace ft
{
	struct Face ;
}

class Outline ;

///	A glyph class.
/**	The Glyph class represents a glyph in an PDF document. A glyph is an image
	representation of a character in a font face. This class provides access
	to many information about a glyph, e.g. its metrics and outline rendering.
	The unit of the metrics provided by this class is always in font unit.
	The actual size of the font unit is defined by Font::UnitsPerEM().
	
	\sa	Font::UnitsPerEM().  
*/
class RealGlyph : public Glyph
{
public :
	RealGlyph( unsigned idx, const ft::Face& face ) ;
	~RealGlyph( ) ;

	unsigned Width( ) const ;
	unsigned Height( ) const ;
	
	unsigned AdvanceX( ) const ;
	unsigned AdvanceY( ) const ;

	bool Decompose( Outline *outline ) const ;
	
	bool IsOutline( ) const ;

private :
	struct Impl ;
	boost::shared_ptr<Impl>	m_impl ;
	
	struct OutlineDecomposer ;
} ;

} // end of namespace

#endif // GLYPH_HH_
