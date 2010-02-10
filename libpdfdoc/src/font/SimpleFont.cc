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

/*!
	\file	SimpleFont.cc
	\brief	implementation the SimpleFont class
	\date	Sat Mar 22 2008
	\author	Nestal Wan
*/

#include "SimpleFont.hh"
#include "FontType.hh"
#include "RealGlyph.hh"

#include "FontException.hh"
#include "FontDescriptor.hh"

#include "core/Dictionary.hh"

#include "file/File.hh"
#include "file/DictReader.hh"

#include "util/Util.hh"
#include "util/Debug.hh"

#include <boost/bind.hpp>
#include <boost/format.hpp>

#ifdef HAVE_FONTCONFIG
#include <fontconfig/fontconfig.h>
#endif

// freetype headers
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_XFREE86_H

// libstdc++
#include <iostream>
#include <algorithm>
#include <iterator>
#include <fstream>
#include <cstring>

namespace pdf {

const Name SimpleFont::m_font_types[] =
{
	Name("TrueType"), Name("Type1"), Name("MMType1"), Name("Type3"),
	Name("Type0" ),
	
	Name("Type1"),	// CFF opentype font is treated as type1
} ;

SimpleFont::SimpleFont(
	const std::string& 	font_file,
	unsigned 			idx,
	FT_Library 			ft_lib )
	: m_first_char( -1 ),
	  m_last_char( -1 )
{
	std::vector<unsigned char> prog = LoadFile( font_file ) ;
	Init( prog, ft_lib ) ;
}

SimpleFont::SimpleFont( const std::string& name, FT_Library ft_lib )
	: m_face( 0 ),
	  m_type( font::unknown ),
	  m_first_char( -1 ),
	  m_last_char( -1 )
{
	std::string path = FindFont( name ) ; 
	std::vector<unsigned char> prog = LoadFile( path ) ;
	Init( prog, ft_lib ) ;
}

SimpleFont::SimpleFont( DictReader& reader, FT_Library ft_lib )
	: m_face( 0 ),
	  m_type( font::unknown ),
	  m_first_char( -1 ),
	  m_last_char( -1 ),
	  m_descriptor( new FontDescriptor )
{
	PDF_ASSERT( ft_lib != 0 ) ;
	
	try
	{
		Name subtype ;
		if ( reader.Detach( "Subtype",	subtype ) )
			m_type	= SubType( subtype ) ;
		
		// base font is absent in type 3 fonts
		if ( m_type != font::type3 )
			reader.Detach( "BaseFont",	m_base_font ) ;
		
		reader.Detach( "FirstChar",	m_first_char ) ;
		reader.Detach( "LastChar",	m_last_char ) ;
		
		// width is optional
		reader.Detach( "Widths", 	m_widths ) ;

		reader.Detach( "Encoding",	m_encoding ) ;
		reader.Detach( "ToUnicode",	m_to_unicode ) ;
		
		DictReader fd ;
		if ( reader.Detach( "FontDescriptor", fd ) )
		{
			m_descriptor->Read( m_type, fd ) ;

			const std::vector<unsigned char>& font_file
				= m_descriptor->FontFile( ) ;
			
			if ( !font_file.empty() )
			{
				m_face = LoadFace( &font_file[0], font_file.size(), ft_lib ) ;
				LoadGlyphs( ) ;
			}
			else if ( !m_base_font.empty() )
			{
				std::string path = FindStdFont( m_base_font.Str() ) ;

				std::vector<unsigned char> prog = LoadFile( path ) ;
				Init( prog, ft_lib ) ;
			}
			else
				throw Exception( "no font name. can't load font" ) ;

		}
		
		// font descriptor is absent. it may be a standard 14 fonts.
		// try to search for them instead.
		else if ( m_type != font::type3 )
		{
			if ( !m_base_font.empty() )
			{
				std::string path = FindStdFont( m_base_font.Str() ) ;

				std::vector<unsigned char> prog = LoadFile( path ) ;
				Init( prog, ft_lib ) ;
			}
			else
				throw Exception( "no font name. can't load font" ) ;
		}
		else
			throw Exception( "no descriptor?" ) ;
	}
	catch ( Exception& e )
	{
		throw FontException(
			boost::format(
				"cannot read font:\n%1%\n"
			    "Font Dictionary: %2%\n" ) % e.what() % *reader ) ;
	}
}

SimpleFont::~SimpleFont( )
{
	// destroy all glyph before the face
	std::for_each(
		m_glyphs.begin(),
		m_glyphs.end(),
		boost::bind<void>( DeletePtr(),
			boost::bind( &GlyphMap::value_type::second, _1 ) ) ) ;
	
	FT_Done_Face( m_face ) ;
}

void SimpleFont::Init( std::vector<unsigned char>& prog, FT_Library ft_lib )
{
	FT_Error e = FT_New_Memory_Face(
		ft_lib,
		&prog[0],
		prog.size(),
		0,
		&m_face ) ;
	
	if ( e != 0 )
		throw FontException( "cannot create font face" ) ;
	
	const char *psname = ::FT_Get_Postscript_Name( m_face ) ;
	m_base_font = (psname != 0 ? psname : "" ) ;

	m_type = font::GetType( m_face ) ;
	m_descriptor.reset( new FontDescriptor( m_face, prog ) ) ;
	LoadGlyphs( ) ;
}

std::vector<unsigned char> SimpleFont::LoadFile( const std::string& filename )
{
	using boost::format ;
	std::ifstream fs( filename.c_str(), std::ios::in | std::ios::binary ) ;
	if ( !fs )
		throw FontException( format("cannot open font file: %1%") % filename ) ;
	
	std::vector<unsigned char> bytes(
		(std::istreambuf_iterator<char>(fs)),
		(std::istreambuf_iterator<char>()) ) ;
	if ( bytes.empty() )
		throw FontException( format("font file %1% is empty") % filename ) ;
	
	return bytes ;
}

FT_FaceRec_* SimpleFont::LoadFace(
	const unsigned char	*data,
	std::size_t 		size,
	FT_LibraryRec_		*ft_lib )
{
	FT_Face face = 0 ;
	FT_Error e = FT_New_Memory_Face( ft_lib, data, size, 0, &face ) ;

	using boost::format ;
	if ( e != 0 )
		throw FontException( format("cannot create font face: %1%") % e ) ;
	
	return face ;
}

#ifdef HAVE_FONTCONFIG
std::string SimpleFont::FindFont(
	const std::string& font,
	const std::string& style )
{
	FcPattern *sans = FcPatternBuild( NULL,
		FC_FAMILY,		FcTypeString, 	font.c_str(),
		FC_WEIGHT,		FcTypeInteger, 	FC_WEIGHT_NORMAL,
		FC_STYLE,		FcTypeString, 	style.c_str(),
		FC_WIDTH,		FcTypeInteger,	FC_WIDTH_NORMAL,
		FC_SCALABLE,	FcTypeBool,		true,
	    NULL ) ;
	if ( sans == 0 )
		throw FontException( "cannot create font pattern" ) ;

	FcResult result ;
	FcPattern *matched = FcFontMatch( 0, sans, &result ) ;

	FcChar8 *filename ;
	if ( FcPatternGetString(matched, FC_FILE, 0, &filename ) != FcResultMatch )
		throw FontException( "cannot find font " + font ) ;

	const char *file = reinterpret_cast<const char*>( filename ) ;
	if ( file == 0 )
		throw FontException( "cannot find font " + font ) ;
	
	int idx ;
	if ( FcPatternGetInteger( matched, FC_INDEX, 0, &idx ) != FcResultMatch )
		throw FontException( "cannot find font " + font ) ;

	// TODO: how to embed font with index != 0?
	if ( idx != 0 )
		throw FontException( "font collection is not supported yet" ) ;

	return file ;
}
#else
std::string SimpleFont::FindFont( const std::string&, const std::string& )
{
	// TODO: implement fond searching without fontconfig
	return "" ;
}
#endif

std::string SimpleFont::FindStdFont( const std::string& base_name )
{
	std::size_t embed = base_name.find_first_of( "+" ) ;
	std::size_t pos = base_name.find_first_of( "-" ) ;
	
	if ( embed != std::string::npos )
		embed = embed + 1 ;
	else
		embed = 0 ;
	
	std::string name = base_name.substr( embed, pos - embed ), style ;
	if ( pos != std::string::npos && pos + 1 < base_name.size() )
		style = base_name.substr( pos+1 ) ;

	// simple font mappings for the standard fonts
	if ( name == "Helvetica" )
//#ifndef WIN32
//		name = "Liberation Sans" ;
//#else
		name = "Arial" ;
//#endif

	else if ( name == "Times" )
//#ifndef WIN32
//		name = "Liberation Serif" ;
//#else
		name = "TimesNewRoman" ;
//#endif

	else if ( name == "Courier" )
//#ifndef WIN32
//		name = "Liberation Mono" ;
//#else
		name = "Courier New" ;
//#endif

#ifndef WIN32
	else if ( name == "Symbol" )
		name = "Standard Symbols L" ;
#endif

	return FindFont( name, style ) ;
}

/// Return the size of the EM square in font units.
unsigned SimpleFont::UnitsPerEM( ) const
{
	PDF_ASSERT( m_face != 0 ) ;
	return m_face->units_per_EM ;
}

///	Common initialization procedure for constructor that comes with an FT_Face.
void SimpleFont::LoadGlyphs( )
{
	PDF_ASSERT( m_face != 0 ) ;

	// by default, freetype will select the unicode charmap, but there are
	// some cases that the unicode charmap is not present. we need to select
	// whatever charmap we have.
	if ( m_face->charmap == 0 && m_face->num_charmaps > 0 )
		FT_Select_Charmap( m_face, m_face->charmaps[0]->encoding ) ;

	// traverse all characters
	unsigned		gindex ;
	unsigned long 	char_code = FT_Get_First_Char( m_face, &gindex ) ;
	int first_char = static_cast<int>( char_code ), last_char = -1 ;

//std::cout << "font: " << BaseName() << std::endl ;
	while ( gindex != 0 && char_code < 256 )
	{
//std::cout << "gindex = " << gindex << " char code = " << char_code << std::endl ;
		// load the glyph to the glyph slot in the face
		// we want to do the scaling in double instead of inside freetype
		// in small font we don't have hinting
		FT_Error error = FT_Load_Glyph( m_face, gindex, FT_LOAD_NO_SCALE ) ;
		if ( error != 0 )
			throw FontException(
				boost::format( "cannot load glyph %2% from %1%" )
				% BaseName() % char_code ) ;

		FT_Glyph glyph ;
		error = FT_Get_Glyph( m_face->glyph, &glyph ) ;
		if ( error != 0 )
			throw FontException(
				boost::format( "cannot copy glyph %2% from %1%" )
				% BaseName() % char_code ) ;

//		if ( glyph->format == FT_GLYPH_FORMAT_OUTLINE )
		{
			m_glyphs.insert( std::make_pair(
				static_cast<wchar_t>(char_code),
				new RealGlyph( gindex, m_face ) ) ) ;
		}
//		else
//			throw FontException(
//				boost::format( "font %1% glyph %2% is not outline" )
//							% BaseName() % char_code ) ;
		
		last_char = static_cast<int>( char_code ) ;
		char_code = ::FT_Get_Next_Char( m_face, char_code, &gindex ) ;
	}
	
	if ( m_first_char == -1 )
		m_first_char = first_char ;
	if ( m_last_char == -1 )
		m_last_char = last_char ;
}

/// Convert Freetype font unit to PDF glyph unit
double SimpleFont::FromFontUnit( unsigned val ) const
{
	return val * 1000.0 / m_face->units_per_EM ;
}

Ref SimpleFont::Write( File *file ) const
{
	PDF_ASSERT( file != 0 ) ;

	Dictionary dict ;
	dict["Type"]		= Name( "Font" ) ;
	dict["Subtype"]		= SubType( m_type ) ;
	
	// BaseFont is optional for type 3 fonts
	if ( m_type != font::type3 )
		dict["BaseFont"]	= m_base_font ;
	
	dict["FirstChar"]	= m_first_char ;
	dict["LastChar"]	= m_last_char ;
	
	if ( !m_encoding.Is<void>() )
		dict["Encoding"]		= m_encoding ;
	
	if ( m_widths.empty() )
	{
		Array widths ;
		for ( int i = m_first_char ; i <= m_last_char ; ++i )
		{
			const Glyph *g = GetGlyph( i ) ;
			widths.push_back(
				g != 0
					? (g->AdvanceX() * 1000.0 / m_face->units_per_EM)
					: 0.0 ) ;
		}
		dict["Widths"]			= widths ;
	}
	else
		dict["Widths"]			= m_widths ;

	dict["FontDescriptor"]	= m_descriptor->Write( file ) ;

	if ( !m_to_unicode.Is<void>( ) )
		dict["ToUnitcode"]	= file->WriteObj( m_to_unicode ) ;

	return file->WriteObj( dict ) ;
}

const Name& SimpleFont::SubType( font::Type t )
{
	PDF_ASSERT( t >= font::truetype && t <= font::type0 ) ;
	return m_font_types[t] ;
}

font::Type SimpleFont::SubType( const Name& name )
{
	const Name *ptr = std::find(
		pdf::Begin( m_font_types ),
	    pdf::End( m_font_types ),
	    name ) ;
	    
	if ( ptr == pdf::End( m_font_types ) )
		throw FontException( boost::format("unknown font type: %1%") % name) ;

	return static_cast<font::Type>( ptr - pdf::Begin( m_font_types ) ) ;
}

std::string SimpleFont::BaseName( ) const
{
	return m_base_font.Str( ) ;
}

const Glyph* SimpleFont::GetGlyph( wchar_t ch ) const
{
	GlyphMap::const_iterator it = m_glyphs.find( ch ) ;
	return it != m_glyphs.end() ? it->second : 0 ;
}

FontDescriptor* SimpleFont::Descriptor( )
{
	return m_descriptor.get() ;
}

BaseFont* CreateFont( DictReader& obj, FT_LibraryRec_ *ft )
{
	return new SimpleFont( obj, ft ) ;
}

} // end of namespace
