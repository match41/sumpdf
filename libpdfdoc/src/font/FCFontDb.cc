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

/**	\file	FCFontDb.cc
	\brief	implementation of the FCFontDb class
	\date	Feb 11, 2010
	\author	Nestal Wan
*/

#include "FCFontDb.hh"

#include "FontException.hh"

#include <boost/format.hpp>

#include "util/Debug.hh"

// fontconfig headers
#ifndef HAVE_FONTCONFIG
	#error This file requires fontconfig to compile.
#else
#include <fontconfig/fontconfig.h>
#include <fontconfig/fcfreetype.h>
#endif

#include <fstream>
#include <iterator>
#include <iostream>

namespace pdf {

namespace
{
	int weight_map[] =
	{
		FC_WEIGHT_THIN,	 		// thin,
		FC_WEIGHT_EXTRALIGHT,	// extra_light,
		FC_WEIGHT_ULTRALIGHT,	// ultra_light,
		FC_WEIGHT_LIGHT,	 	// light,
		FC_WEIGHT_BOOK,	 		// book,
		FC_WEIGHT_REGULAR,	 	// regular_weight,
		FC_WEIGHT_NORMAL,	 	// normal_weight,
		FC_WEIGHT_MEDIUM,	 	// medium_weight,
		FC_WEIGHT_DEMIBOLD,	 	// demi_bold,
		FC_WEIGHT_SEMIBOLD,	 	// semi_bold,
		FC_WEIGHT_BOLD,	 		// bold,
		FC_WEIGHT_EXTRABOLD,	// extra_bold,
		FC_WEIGHT_ULTRABOLD,	// ultra_bold,
		FC_WEIGHT_BLACK,	 	// black,
		FC_WEIGHT_HEAVY,	 	// heavy,
		FC_WEIGHT_EXTRABLACK,	// extra_black,
		FC_WEIGHT_ULTRABLACK,	// ultra_black,
	} ;

	int width_map[] =
	{
		0,							// padding,
		FC_WIDTH_ULTRACONDENSED,	// ultra_condensed,
		FC_WIDTH_EXTRACONDENSED,	// extra_condensed,
		FC_WIDTH_CONDENSED,	 		// condensed,
		FC_WIDTH_SEMICONDENSED,		// semi_condensed,
		FC_WIDTH_NORMAL,	 		// normal_width,
		FC_WIDTH_SEMIEXPANDED,	 	// semi_expanded,
		FC_WIDTH_EXPANDED,			// expanded,
		FC_WIDTH_EXTRAEXPANDED,		// extra_expanded,
		FC_WIDTH_ULTRAEXPANDED,		// ultra_expanded,
	} ;

	int slant_map[] =
	{
		FC_SLANT_ROMAN,		// roman
		FC_SLANT_ITALIC,	// italic
		FC_SLANT_OBLIQUE,	// oblique
	} ;
	
	std::vector<unsigned char> MatchFont( FcPattern *sans, const std::string& base_name )
	{
		FcConfigSubstitute( 0, sans, FcMatchPattern ) ;

		FcResult result ;
		FcPattern *matched = FcFontMatch( 0, sans, &result ) ;

		FcChar8 *filename ;
		if ( FcPatternGetString(matched, FC_FILE, 0, &filename ) != FcResultMatch )
			throw FontException() << expt::ErrMsg( "cannot find font " + base_name ) ;

		const char *file = reinterpret_cast<const char*>( filename ) ;
		if ( file == 0 )
			throw FontException() << expt::ErrMsg( "cannot find font " + base_name ) ;

		int idx ;
		if ( FcPatternGetInteger( matched, FC_INDEX, 0, &idx ) != FcResultMatch )
			throw FontException() << expt::ErrMsg( "cannot find font " + base_name ) ;

		// TODO: how to embed font with index != 0?
		if ( idx != 0 )
			throw Unsupported() << expt::ErrMsg( "font collection is not supported yet" ) ;

		std::ifstream fs( file, std::ios::binary | std::ios::in ) ;
		std::vector<unsigned char> prog(
			(std::istreambuf_iterator<char>( fs )),
			(std::istreambuf_iterator<char>()) ) ;
		
		using boost::format ;
		if ( prog.empty() )
			throw FontException() << expt::FormattedMsg( format("font file %1% is empty") % file ) ;
		
		return prog ;
	}
}

/**	constructor
	
*/
FCFontDb::FCFontDb( )
{
}

std::vector<unsigned char> FCFontDb::FindFont(
	const std::string&	base_name,
	font::Weight		weight,
	font::Slant			slant,
	font::Width			width )
{
	PDF_ASSERT( weight >= font::thin && weight <= font::ultra_black ) ;
	PDF_ASSERT(
		width >= font::ultra_condensed &&
		width <= font::ultra_expanded ) ;
	PDF_ASSERT( slant >= font::roman && slant <= font::oblique ) ;

	FcPattern *sans = FcPatternBuild( NULL,
		FC_FAMILY,		FcTypeString, 	base_name.c_str(),
		FC_WEIGHT,		FcTypeInteger, 	weight_map[weight],
		FC_WIDTH,		FcTypeInteger,	width_map[width],
		FC_SLANT,		FcTypeInteger,	slant_map[slant],
		FC_SCALABLE,	FcTypeBool,		true,
	    NULL ) ;
	if ( sans == 0 )
		throw FontException() << expt::ErrMsg( "cannot create font pattern" ) ;

	return MatchFont( sans, base_name ) ;
}

std::string FCFontDb::FindFontPath( FT_FaceRec_ *ref )
{
	PDF_ASSERT( ref != 0 ) ;

	// something non-null
	FcChar8 abc[1] ;

	FcBlanks *b = FcBlanksCreate() ;

	FcPattern *pat = FcFreeTypeQueryFace( ref, abc, 0, b ) ;
	PDF_ASSERT( pat != 0 ) ;

	FcResult result ;
	FcPattern *matched = FcFontMatch( 0, pat, &result ) ;

	FcChar8 *filename ;
	if ( FcPatternGetString(matched, FC_FILE, 0, &filename ) != FcResultMatch )
		throw FontException() << expt::ErrMsg( "cannot find font" ) ;

	const char *file = reinterpret_cast<const char*>( filename ) ;
	if ( file == 0 )
		throw FontException() << expt::ErrMsg( "cannot find font" ) ;

	return file ;
}

} // end of namespace
