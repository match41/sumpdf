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
	\file	Face.hh
	\brief	definition of the freetype face wrapper
	\date	2002-01-10
	\atuhor	Nestal Wan
*/

#ifndef __FREETYPE_FACE_HEADER_INCLUDED__
#define __FREETYPE_FACE_HEADER_INCLUDED__

// standard C++ headers
#include <memory>
#include <string>

/*!	\namespace	freetype
	\brief		freetype wrappers namespace
	
	this namespace will contain all freetype related classes and functions.
	the freetype wrapper classes provides a simple way to use freetype to
	manipulate fonts.

	the actual freetype data structure is hide behind a
	pointer-to-implementation. users of this class are not requried to have
	access to freetype headers when compiling this class.
*/
namespace ft {

class Library ;

/*!	\class	Face
	\brief	the font face class.
	
	a font face is a font with a specified font size. this class is used to
	wrap up freetype operations to get metrics of a font face, and create glyph
	objects.
	*/
class Face
{
public :
	struct Body ;
	
private :
	//! pointer to implementation. the actual freetype data structure
	//! is hided behind this pointer.
	const std::auto_ptr<Body> m_pimpl ;

public :
	Face( Library *lib, const std::string& filename, long index = 0 ) ;
	Face( Library *lib, const unsigned char *buffer, std::size_t size,
	      long index = 0 ) ;
	~Face( ) ;
	
	const std::string Family( ) const ;
	const std::string Style( ) const ;
	const std::string PSName( ) const ;

	int Ascent( ) const ;
	int Decent( ) const ;
	int UnitsPerEM( ) const ;
	std::size_t GlyphCount( ) const ;
	std::size_t FixedCount( ) const ;
	void BoundBox( int bbox[] ) const ;

	int XHeight( ) const ;
	int CapHeight( ) const ;

	double ItalicAngle( ) const ;

	bool IsFixed( ) const ;

	void SetSize( double width, double height, int hres, int vres ) ;
	unsigned int GetGlyphCode( unsigned long char_code ) const ;

	unsigned long GetFirstChar( unsigned& glyph_idx ) const ;
	unsigned long GetNextChar( unsigned long current,
	                           unsigned& glyph_idx ) const ;

public :
	// suppose only for Glyph
	Body* Pimpl( ) { return m_pimpl.get( ) ; }
	const Body* Pimpl( ) const { return m_pimpl.get( ) ; }

private :
#ifndef NDEBUG
	bool IsValid( ) const ;
#endif
} ;

} // end of namespace

#endif
