/***************************************************************************
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
 ***************************************************************************/

/*!
	\file	Font.hh
	\brief	definition the Font class
	\date	Fri Apr 11 2008
	\author	Nestal Wan
*/

#ifndef __PDF_FONT_HEADER_INCLUDED__
#define __PDF_FONT_HEADER_INCLUDED__

#include <string>

namespace pdf {

class Glyph ;
class FontEncoding ;

/**	\defgroup	font Fonts
	\brief		Font module

	libpdfdoc provides a number of classes for font handling. They provide
	access to important font metrics and functions for rendering the font
	glyphs.
	
	See the <a
	href="http://freetype.sourceforge.net/freetype2/docs/glyphs/index.html">
	Freetype page</a> for details on the background about fonts.
*/

///	The Font class.
/**	\ingroup	font
	
	This class represents a font in the PDF document. It can be created by
	reading a PDF document which contain some fonts, or by manually creating
	by a font file. It provides access to the Glyph in the font.
*/
class Font
{
public :
	virtual ~Font( ) ;
	
	/// Return the base name of a font.
	/**	This function returns the PostScript name of the font. For Type 1 fonts,
		this is usually the value of the FontName entry in the font program.
		For TrueType fonts, it is returned by the
		<a href="http://freetype.sourceforge.net/freetype2/docs/reference/ft2-base_interface.html#FT_Get_Postscript_Name">
		FT_Get_Postscript_Name()</a>
		function in FreeType. It will be used for displaying the font's name
		for client applications.
		\return	The base name of a font.
	*/
	virtual std::string 	BaseName( ) const = 0 ;
	
	///	Return the glyph for the corresponding character code.
	/**	This function will search the glyph for the given character code. If
		there is no glyph defined for this character in the font, it will return
		0.
		\param	ch	The unicode value of the character.
		\return	A point to the glyph, or 0 if the glyph in absent in the font.
	*/
	virtual const Glyph*	GetGlyph( wchar_t ch ) const = 0 ;
	
	///	Return the number of font unit in the EM square.
	/**	In creating the glyph outlines, a type designer uses an imaginary square
		called the EM square. Typically, the EM square can be thought of as a
		tablet on which the characters are drawn. The square's size, i.e., the
		number of grid units on its sides, is very important for two reasons:

		-	It is the reference used to scale the outlines to a given text
			dimension. For example, a size of 12pt at 300x300 dpi corresponds
			to 12x300/72 = 50 pixels. This is the size the EM square would
			appear on the output device if it was rendered directly. In other
			words, scaling from grid units to pixels uses the formula:

			  pixel_size = point_size * resolution / 72
			  pixel_coord = grid_coord * pixel_size / EM_size

		-	The greater the EM size is, the larger resolution the designer can
			use when digitizing outlines. For example, in the extreme example
			of an EM size of 4 units, there are only 25 point positions
			available within the EM square which is clearly not enough. Typical
			TrueType fonts use an EM size of 2048 units; Type 1 PostScript
			fonts have a fixed EM size of 1000 grid units but point coordinates
			can be expressed as floating values.
	*/
	virtual unsigned UnitsPerEM( ) const = 0 ;
	
	/// Return the width of the glyph in PDF glyph unit.
	/**	Call this function to convert the lengths in font unit to PDF glyph
		unit. The PDF glyph unit is 1/1000 of user space unit, which is 1/72 inch.
		I.e. if this function returns 72000, then it is one inch. This value
		must be further multiplied by the font size in points to get the real
		device-independent size.
		\param	val		The value in font unit to be convert to glyph unit.
		\return	The converted value in glyph unit.
	*/
	virtual double FromFontUnit( unsigned val ) const = 0 ;
	
	virtual FontEncoding* Encoding( ) = 0 ;
	
	/// Return the height of the font.
	/**	This field is simply used to compute a default line spacing (i.e., the
		baseline-to-baseline distance) when writing text with this font.
	*/
	virtual double Height( ) const = 0 ; 
} ;

} // end of namespace

#endif
