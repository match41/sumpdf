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
	enum Type { truetype, type1, mmtype1, type3, type0, openType, unknown } ;
	
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
