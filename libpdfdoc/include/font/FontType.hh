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

/**	\file	FontType.hh
    \brief	definition the FontType class
    \date	Jan 28, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_FONTTYPE_HEADER_INCLUDED__
#define __PDF_FONTTYPE_HEADER_INCLUDED__

struct FT_FaceRec_ ;

namespace pdf {

namespace font
{
	///	font type enum
	/**	The Type enum represents an enumeration of font types.
	*/
	enum Type
	{
		///	Truetype fonts
		/**	Truetype fonts are SFNT base fonts with a "glyp" table. They are
			the standard font format for MS Windows systems.
		*/
		truetype,
		
		/// Composite fonts
		/**	Type 0 fonts are composite fonts. They aren't really real fonts.
			They contain other fonts as their descendant. In PDF they can only
			contain one single descendant which should be a CID font.
		*/
		type0,
		
		/// Type 1 fonts
		/**	Very commonly called "postscript fonts". They are developed by
			adobe. They use postscript language to describe glyph shapes.
		*/
		type1,
		
		/// CFF/Type 2 fonts
		/**	Type 2 fonts are called CFF (compact font format). They are just
			type 1 fonts with a binary representation.
		*/
		type2,
		
		/// Type 3 fonts
		/**	Type 3 fonts are fake fonts which the glyph are described by PDF
			rendering operators.
		*/
		type3,
		
		/// Multiple Master Type 1 fonts
		mmtype1,
		
		/// OpenType CFF fonts
		/**	OpenType font (i.e. SFNT based font) with a "CFF " table instead
			of a "glyp" table. The "CFF " table contains a CFF font.
		*/
		opentype_cff,
		
		unknown
	} ;
	
	Type GetType( FT_FaceRec_ *face ) ;
	
	/// font weight enum
	enum Weight
	{
		thin,
		extra_light,
		ultra_light,
		light,
		book,
		regular_weight,
		normal_weight,
		medium_weight,
		demi_bold,
		semi_bold,
		bold,
		extra_bold,
		ultra_bold,
		black,
		heavy,
		extra_black,
		ultra_black,
	} ;
	
	Weight ParseWeight( const char *str ) ;
	
	enum Width
	{
		padding,	///< to match the OS/2 table in truetype font enum
		
		ultra_condensed,
		extra_condensed,
		condensed,
		semi_condensed,
		normal_width,
		semi_expanded,
		expanded,
		extra_expanded,
		ultra_expanded,
		unknown_width,
	} ;

	enum Slant
	{
		roman,
		italic,
		oblique,
	} ;
	
	Slant ParseSlant( const char *str ) ;
}

} // end of namespace

#endif // FONTTYPE_HH_
