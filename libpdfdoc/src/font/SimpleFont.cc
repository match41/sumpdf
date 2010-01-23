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

#include "FontException.hh"

#include "FontDescriptor.hh"

#include "core/Array.hh"
#include "core/Dictionary.hh"

#include "file/IFile.hh"
#include "file/ObjectReader.hh"

#include "util/Util.hh"
#include "util/Debug.hh"

#include <boost/bind.hpp>
#include <boost/format.hpp>

#ifdef HAVE_FONTCONFIG
#include <fontconfig/fontconfig.h>
#endif

#include <iostream>
#include <algorithm>
#include <iterator>
#include <fstream>

#include FT_XFREE86_H

namespace pdf {

const Name SimpleFont::m_font_types[] =
{
	Name("TrueType"), Name("Type1"), Name("MMType1"), Name("Type3"),
	Name("Type0" ),
} ;

/*
SimpleFont::SimpleFont( )
	: m_base_font( "Helvetica-Bold" ),
	  m_type( type1 )
{
	m_first_char = m_last_char = 0 ;
}

SimpleFont::SimpleFont( const Name& base_font, Type type )
	: m_base_font( base_font ),
	  m_type( type )
{
	m_first_char = m_last_char = 0 ;
}
*/

SimpleFont::SimpleFont(
	const std::string& 	font_file,
	unsigned 			idx,
	FT_Library 			ft_lib )
//	: m_face( LoadFace( font_file, idx, ft_lib ) ),
//	  m_base_font( ::FT_Get_Postscript_Name( m_face ) ),
//	  m_type( GetFontType( m_face ) ),
//	  m_descriptor( new FontDesciptor( m_face ) )
{
	std::vector<unsigned char> prog = LoadFile( font_file ) ;
	Init( prog, ft_lib ) ;
}

SimpleFont::SimpleFont( const std::string& name, FT_Library ft_lib )
	: m_face( 0 ),
//	  m_base_font( ::FT_Get_Postscript_Name( m_face ) ),
	  m_type( unknown )
//	  m_descriptor( m_face )
{
	std::string path = FindFont( name ) ; 
	std::vector<unsigned char> prog = LoadFile( path ) ;
	Init( prog, ft_lib ) ;
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
	
	m_base_font = ::FT_Get_Postscript_Name( m_face ) ;
	m_type = GetFontType( m_face ) ;
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

#ifdef HAVE_FONTCONFIG
std::string SimpleFont::FindFont( const std::string& font )
{
	FcPattern *sans = FcPatternBuild( NULL,
		FC_FAMILY,	FcTypeString, 	font.c_str(),
		FC_WEIGHT,	FcTypeInteger, 	FC_WEIGHT_MEDIUM,
	    NULL ) ;
	if ( sans == 0 )
		throw FontException( "cannot create font pattern" ) ;

	FcResult result ;
	FcPattern *matched = FcFontMatch( 0, sans, &result ) ;

	FcChar8 *filename ;
	if ( FcPatternGetString(matched, FC_FILE, 0, &filename ) != FcResultMatch )
		throw FontException( "cannot find font " + font ) ;

	const char *file = reinterpret_cast<const char*>( filename ) ;

	int idx ;
	if ( FcPatternGetInteger( matched, FC_INDEX, 0, &idx ) != FcResultMatch )
		throw FontException( "cannot find font " + font ) ;

	// TODO: how to embed font with index !+ 0?
	if ( idx != 0 )
		throw FontException( "font collection is not supported yet" ) ;

//	FT_Face face ;
//	if ( FT_New_Face( ft_lib, file, idx, &face ) != 0 )
//		throw FontException( "cannot load font file " + std::string(file) ) ;

	return file ;
}
#endif

SimpleFont::SimpleFont( Dictionary& self, IFile *file, FT_Library ft_lib )
	: m_face( 0 ),
	  m_type( unknown )
{
	PDF_ASSERT( file != 0 ) ;
	PDF_ASSERT( ft_lib != 0 ) ;
	
	try
	{
		Name subtype ;
		if ( self.Extract( "Subtype",	subtype ) )
			m_type	= SubType( subtype ) ;

		self.Extract( Name("BaseFont"),		m_base_font ) ;
		self.Extract( Name("FirstChar"),	m_first_char ) ;
		self.Extract( Name("LastChar"),		m_last_char ) ;

		// width is optional
//		Array widths ;
//		if ( Detach( file, self, "Widths", widths ) )
//			m_widths.assign( widths.begin(), widths.end() ) ;

//		self.Extract( "Encoding",	m_encoding ) ;
//		self.Extract( "ToUnicode",	m_to_unicode ) ;
		
		Dictionary fd ;
		if ( Detach( file, self, "FontDescriptor", fd ) )
		{
			if ( ReadDescriptor( fd, ft_lib, file ) )
				LoadGlyphs( ) ;
		}
		
		m_self.Read( self, file ) ;
	}
	catch ( Exception& e )
	{
		std::ostringstream msg ;
		msg << "cannot read font:\n" << e.what( ) << "\n"
		    << "Font Dictionary: \n" << self << "\n" ;
		throw FontException( msg.str( ) ) ;
	}
}

SimpleFont::~SimpleFont( )
{
	std::for_each(
		m_glyphs.begin(),
		m_glyphs.end(),
		boost::bind(
			&FT_Done_Glyph,
			boost::bind( &GlyphData::glyph,
				boost::bind( &GlyphMap::value_type::second, _1 ) ) ) ) ;

	FT_Done_Face( m_face ) ;
}

bool SimpleFont::ReadDescriptor( Dictionary& fd, FT_Library ft_lib, IFile *file )
{
	m_descriptor->Read( fd, file ) ;

	const std::vector<unsigned char>& font_file = m_descriptor->FontFile( ) ;

//std::ofstream f( (m_base_font.Str()+".ttf").c_str() ) ;
//std::copy( font_file.begin(), font_file.end(),
//std::ostreambuf_iterator<char>( f ) ) ;
//f.close() ;
	if ( !font_file.empty() )
	{
		FT_Error e = FT_New_Memory_Face(
			ft_lib,
			&font_file[0],
			font_file.size(),
			0,
			&m_face ) ;
		
		if ( e != 0 )
			throw FontException( "cannot create font face" ) ;
		
		return true ;
	}
	else
		return false ;
}

double SimpleFont::Width( wchar_t ch ) const
{
	GlyphMap::const_iterator it = m_glyphs.find( ch ) ;
	return it != m_glyphs.end()
		? it->second.metrics.horiAdvance * 1000.0 / m_face->units_per_EM
		: 0.0 ;
}

double SimpleFont::Width( const std::wstring& text, double size ) const
{
	double width = 0.0 ;
	for ( std::wstring::const_iterator i = text.begin() ; i < text.end() ; ++i )
	{
		if ( *i >= m_first_char && *i <= m_last_char )
			width += (Width(*i) * size ) ;
	}
	return width ;
}

SimpleFont::Type SimpleFont::GetFontType( FT_Face face )
{
	const char *format = ::FT_Get_X11_Font_Format( face ) ;

	if ( format == 0 )
		return unknown ;
	else if ( ::strcasecmp( format, "Truetype" ) == 0 )
		return truetype ;
	else if ( ::strcasecmp( format, "Type 1" ) == 0 )
		return type1 ;
	else
		throw FontException( "unknown font type: " + std::string(format) ) ;
}

///	Common initialization procedure for constructor that comes with an FT_Face.
void SimpleFont::LoadGlyphs( )
{
	PDF_ASSERT( m_face != 0 ) ;

	// traverse all characters
	unsigned		gindex ;
	unsigned long 	char_code = FT_Get_First_Char( m_face, &gindex ) ;
	m_first_char = static_cast<int>( char_code ) ;

	while ( gindex != 0 && char_code < 256 )
	{
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

		if ( glyph->format == FT_GLYPH_FORMAT_OUTLINE )
		{
			GlyphData g =
			{
				glyph,
				m_face->glyph->metrics,
			} ;
			m_glyphs[char_code] = g ;
		}
		
		m_last_char = static_cast<int>( char_code ) ;
		char_code = ::FT_Get_Next_Char( m_face, char_code, &gindex ) ;
	}
}

Ref SimpleFont::Write( IFile *file ) const
{
	PDF_ASSERT( file != 0 ) ;

	CompleteObj dict( m_self ) ;
	dict.Get()["Type"]		= Name( "Font" ) ;
	dict.Get()["Subtype"]	= SubType( m_type ) ;
	dict.Get()["BaseFont"]	= m_base_font ;
	dict.Get()["FirstChar"]	= m_first_char ;
	dict.Get()["LastChar"]	= m_last_char ;
	
//	double mat[] = { 0.001, 0, 0, 0.001, 0, 0 } ;
//	dict.Get()["FontMatrix"]	= Array( Begin(mat), End(mat) ) ;

	if ( dict.Get().find( "Encoding" ) == dict.Get().end() )
		dict.Get()["Encoding"]		= Name("WinAnsiEncoding") ;

//	if ( !m_encoding.IsNull() )
//		dict.Get()["Encoding"]		= m_encoding ;

	Array widths ;
	for ( int i = m_first_char ; i <= m_last_char ; ++i )
		widths.push_back( Width(i) ) ;
	
	dict.Get()["Widths"]			= widths ;

	dict.Get()["FontDescriptor"]	= m_descriptor->Write( file ) ;

//	if ( !m_to_unicode.IsNull( ) )
//		dict.Get()["ToUnitcode"]	= file->WriteObj( m_to_unicode ) ;

	return dict.Write( file ) ;
}

const Name& SimpleFont::SubType( Type t )
{
	PDF_ASSERT( t >= truetype && t <= type0 ) ;
	return m_font_types[t] ;
}

SimpleFont::Type SimpleFont::SubType( const Name& name )
{
	const Name *ptr = std::find( pdf::Begin( m_font_types ),
	                             pdf::End( m_font_types ), name ) ;
	if ( ptr == pdf::End( m_font_types ) )
		throw FontException( boost::format("unknown font type: %1%") % name) ;

	return static_cast<Type>( ptr - pdf::Begin( m_font_types ) ) ;
}

std::string SimpleFont::BaseName( ) const
{
	return m_base_font.Str( ) ;
}

FT_Face SimpleFont::Face( ) const
{
	return m_face ;
}

FT_Glyph SimpleFont::Glyph( wchar_t ch, FT_Glyph_Metrics *met ) const
{
	GlyphMap::const_iterator it = m_glyphs.find( ch ) ;
	if ( it != m_glyphs.end() )
	{
		if ( met != 0 )
			*met = it->second.metrics ;
		
		PDF_ASSERT( it->second.glyph != 0 ) ;
		return it->second.glyph ;
	}
	else
		return 0 ;
}

FontDescriptor* SimpleFont::Descriptor( )
{
	return m_descriptor.get() ;
}

BaseFont* CreateFont( Dictionary& obj, IFile *file, FT_Library ft_lib )
{
	return new SimpleFont( obj, file, ft_lib ) ;
}

} // end of namespace
