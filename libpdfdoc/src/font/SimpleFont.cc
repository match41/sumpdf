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

#include "RealGlyph.hh"
#include "FontException.hh"
#include "FontDescriptor.hh"
#include "FontEncoding.hh"

// libpdfdoc headers
#include "core/Array.hh"
#include "core/Name.hh"
#include "core/Object.hh"
#include "core/Dictionary.hh"

#include "font/FontDb.hh"
#include "file/File.hh"
#include "file/DictReader.hh"
#include "file/ElementFactory.hh"

#include "util/Util.hh"
#include "util/Debug.hh"

// boost library
#include <boost/bind.hpp>
#include <boost/format.hpp>
#include <boost/tr1/unordered_map.hpp>

#ifdef HAVE_FONTCONFIG
#include <fontconfig/fontconfig.h>
#endif

// freetype headers
#include <ft2build.h>
#include FT_FREETYPE_H

// libstdc++
#include <iostream>
#include <algorithm>
#include <iterator>
#include <fstream>
#include <cstring>

namespace pdf {

struct SimpleFont::Impl
{
	FT_FaceRec_			*face ;

	Name				base_font ;
	font::Type			type ;
	int					first_char, last_char ;

	std::vector<double>	widths ;
	
	FontDescriptor		*descriptor ;
	Object				to_unicode ;
	FontEncoding		*encoding ;		//!< name or dictionary
	
	typedef std::tr1::unordered_map<wchar_t, RealGlyph*> GlyphMap ;
	GlyphMap	glyphs ;
	
	Impl()
	: face( 0 )
	, type( font::unknown )
	, first_char( -1 )
	, last_char( -1 )
	, descriptor( 0 )
	, encoding( 0 )
	{
	}
} ;

const Name SimpleFont::m_font_types[] =
{
	Name("TrueType"), Name("Type1"), Name("MMType1"), Name("Type3"),
	Name("Type0" ),
	
	Name("Type1"),	// CFF opentype font is treated as type1
} ;

SimpleFont::SimpleFont( const std::string& name, FontDb *font_db )
	: m_impl( new Impl )
{
	PDF_ASSERT( font_db != 0 ) ;
	
	InitWithStdFont( name, font_db ) ;
	
	PDF_ASSERT( m_impl->face != 0 ) ;
}

SimpleFont::SimpleFont( DictReader& reader, FontDb *font_db )
	: m_impl( new Impl )
{
	PDF_ASSERT( font_db != 0 ) ;
	
	try
	{
		Name subtype ;
		if ( reader.Detach( "Subtype",	subtype ) )
			m_impl->type	= SubType( subtype ) ;
		
		// base font is absent in type 3 fonts
		if ( m_impl->type == font::type0 )
			throw FontException( "Type0 font is not supported yet" ) ;
			
		if ( m_impl->type != font::type3 )
		{
			if ( !reader.Detach( "BaseFont", m_impl->base_font ) )
				throw FontException( "no BaseFont for font" ) ;
		}
		
		reader.Detach( "FirstChar",	m_impl->first_char ) ;
		reader.Detach( "LastChar",	m_impl->last_char ) ;
		
		// width is optional
		reader.Detach( "Widths", 	m_impl->widths ) ;

		try
		{
			ElementFactory<> f( reader ) ;
			FontEncoding *enc = f.Create<FontEncoding>(
				"Encoding",
				NewPtr<FontEncoding>() ) ;
			
			if ( enc != 0 )
			{
				if ( m_impl->encoding != 0 )
					m_impl->encoding->Release() ;
				
				m_impl->encoding = enc ;
			}
		}
		catch ( std::exception& )
		{
			Name name ;
			if ( reader.Detach( "Encoding", name ) )
				;
		}
		
		reader.Detach( "ToUnicode",	m_impl->to_unicode ) ;
		
		if ( LoadDescriptor( reader, font_db ) )
		{
		}
		
		// font descriptor is absent. it may be a standard 14 fonts.
		// try to search for them instead.
		else if ( m_impl->type != font::type3 )
		{
			if ( !InitWithStdFont( m_impl->base_font.Str(), font_db )  )
				throw FontException( "no font name. can't load font" ) ;
		}
		else
			throw FontException( "no descriptor?" ) ;
	}
	catch ( Exception& e )
	{
		e.Add( boost::format(
			"cannot read font:\n%1%\n"
			"Font Dictionary: %2%\n" ) % e.what() % *reader ) ;
		throw ;
	}
	
	PDF_ASSERT( m_impl->face != 0 ) ;
}

SimpleFont::~SimpleFont( )
{
	// destroy all glyph before the face
	std::for_each(
		m_impl->glyphs.begin(),
		m_impl->glyphs.end(),
		boost::bind<void>( DeletePtr(),
			boost::bind( &Impl::GlyphMap::value_type::second, _1 ) ) ) ;
	
	PDF_ASSERT( m_impl->face != 0 ) ;
	FT_Done_Face( m_impl->face ) ;
	
	if ( m_impl->encoding != 0 )
		m_impl->encoding->Release() ;
	
	if ( m_impl->descriptor != 0 )
		m_impl->descriptor->Release() ;
}

bool SimpleFont::LoadDescriptor( DictReader& reader, FontDb *font_db )
{
	ElementFactory<> f( reader ) ;
	FontDescriptor *fd = f.Create<FontDescriptor>(
		"FontDescriptor",
		boost::bind( NewPtr<FontDescriptor>(), m_impl->type, _1 ) ) ;

	if ( fd != 0 )
	{
		if ( m_impl->descriptor != 0 )
			m_impl->descriptor->Release() ;
		
		m_impl->descriptor = fd ;

		const std::vector<unsigned char>& font_file
			= m_impl->descriptor->FontFile( ) ;
		
		if ( !font_file.empty() )
		{
			m_impl->face = font_db->LoadFont( &font_file[0], font_file.size() );
			
			PDF_ASSERT( m_impl->face != 0 ) ;
			
			LoadGlyphs( ) ;
		}
		else if ( !InitWithStdFont( m_impl->base_font.Str(), font_db )  )
			throw Exception( "can't load font " + m_impl->base_font.Str() );
		
		return true ;
	}
	else
		return false ;
}

bool SimpleFont::InitWithStdFont( const std::string& name, FontDb *font_db )
{
	if ( !name.empty() )
	{
		PDF_ASSERT( font_db != 0 ) ;
		std::vector<unsigned char> prog = FindStdFont( name, font_db ) ;
		Init( prog, font_db ) ;
		return true ;
	}
	else
		return false ;
}
	
void SimpleFont::Init( std::vector<unsigned char>& prog, FontDb *font_db )
{
	PDF_ASSERT( font_db != 0 ) ;
	m_impl->face = font_db->LoadFont( &prog[0], prog.size() ) ;
	
	if ( m_impl->base_font.empty() )
	{
		const char *psname = ::FT_Get_Postscript_Name( m_impl->face ) ;
		m_impl->base_font = (psname != 0 ? psname : "" ) ;
	}

	if ( m_impl->type == font::unknown )
		m_impl->type 		= font::GetType( m_impl->face ) ;
	
	if ( m_impl->descriptor == 0 )
		m_impl->descriptor = new FontDescriptor( m_impl->face, prog ) ;
	
	LoadGlyphs( ) ;
}

std::vector<unsigned char> SimpleFont::FindStdFont(
	const std::string&	base_name,
	FontDb				*fdb )
{
	std::size_t embed = base_name.find_first_of( "+" ) ;
	std::size_t pos = base_name.find_first_of( "-," ) ;
	
	if ( embed != std::string::npos )
		embed = embed + 1 ;
	else
		embed = 0 ;
	
	std::string name = base_name.substr( embed, pos - embed ), style ;
	if ( pos != std::string::npos && pos + 1 < base_name.size() )
		style = base_name.substr( pos+1 ) ;

	// simple font mappings for the standard fonts
	if ( name == "Helvetica" )
		name = "Arial" ;

	else if ( name == "Times" )
		name = "TimesNewRoman" ;

	else if ( name == "Courier" )
		name = "Courier New" ;

#ifndef WIN32
	else if ( name == "Symbol" )
		name = "Standard Symbols L" ;
#endif
	font::Weight	weight	= font::ParseWeight( style.c_str() ) ;
	font::Slant		slant	= font::ParseSlant( style.c_str() ) ;

	return fdb->FindFont( name, weight, slant, font::normal_width ) ;
}

/// Return the size of the EM square in font units.
unsigned SimpleFont::UnitsPerEM( ) const
{
	PDF_ASSERT( m_impl->face != 0 ) ;
	return m_impl->face->units_per_EM ;
}

///	Common initialization procedure for constructor that comes with an FT_Face.
void SimpleFont::LoadGlyphs( )
{
	PDF_ASSERT( m_impl->face != 0 ) ;

	// by default, freetype will select the unicode charmap, but there are
	// some cases that the unicode charmap is not present. we need to select
	// whatever charmap we have.
	if ( m_impl->face->charmap == 0 && m_impl->face->num_charmaps > 0 )
		FT_Select_Charmap( m_impl->face, m_impl->face->charmaps[0]->encoding ) ;

	// traverse all characters
	unsigned		gindex ;
	unsigned long 	char_code = FT_Get_First_Char( m_impl->face, &gindex ) ;
	int first_char = static_cast<int>( char_code ), last_char = -1 ;

	while ( gindex != 0 )
	{
		// load the glyph to the glyph slot in the face
		// we want to do the scaling in double instead of inside freetype
		// in small font we don't have hinting
		FT_Error error = FT_Load_Glyph( m_impl->face, gindex, FT_LOAD_NO_SCALE ) ;
		if ( error != 0 )
			throw FontException(
				boost::format( "cannot load glyph %2% from %1%" )
				% BaseName() % char_code ) ;

		FT_Glyph glyph ;
		error = FT_Get_Glyph( m_impl->face->glyph, &glyph ) ;
		if ( error != 0 )
			throw FontException(
				boost::format( "cannot copy glyph %2% from %1%" )
				% BaseName() % char_code ) ;

		if ( glyph->format != FT_GLYPH_FORMAT_OUTLINE )
			std::cerr << "font " << BaseName() << " has a non-outline glyph" << std::endl ;

		m_impl->glyphs.insert( std::make_pair(
			static_cast<wchar_t>(char_code),
			new RealGlyph( gindex, m_impl->face ) ) ) ;
		
		last_char = static_cast<int>( char_code ) ;
		char_code = ::FT_Get_Next_Char( m_impl->face, char_code, &gindex ) ;
	}
	
	if ( m_impl->first_char == -1 )
		m_impl->first_char = first_char ;
	if ( m_impl->last_char == -1 )
		m_impl->last_char = last_char ;
}

/// Convert Freetype font unit to PDF glyph unit
double SimpleFont::FromFontUnit( unsigned val ) const
{
	return val * 1000.0 / m_impl->face->units_per_EM ;
}

Ref SimpleFont::Write( File *file ) const
{
	PDF_ASSERT( file != 0 ) ;

	Dictionary dict ;
	dict.insert( "Type", 		Name( "Font" ) ) ;
	dict.insert( "Subtype", 	SubType( m_impl->type ) ) ;
	
	// BaseFont is optional for type 3 fonts
	if ( m_impl->type != font::type3 && !m_impl->base_font.empty() )
		dict.insert( "BaseFont", 	m_impl->base_font ) ;
	
	dict.insert( "FirstChar", 	m_impl->first_char ) ;
	dict.insert( "LastChar", 	m_impl->last_char ) ;
	
	// TODO: write the font encoding
//	dict.insert( "Encoding", 	m_encoding ) ;
	
	if ( m_impl->widths.empty() )
	{
		std::vector<double> widths ;
		for ( int i = m_impl->first_char ; i <= m_impl->last_char ; ++i )
		{
			const Glyph *g = GetGlyph( i ) ;
			widths.push_back(
				g != 0
					? (g->AdvanceX() * 1000.0 / m_impl->face->units_per_EM)
					: 0.0 ) ;
		}
		dict.insert( "Widths", 		widths ) ;
	}
	else
		dict.insert( "Widths", 		m_impl->widths ) ;

	dict.insert( "FontDescriptor", 	m_impl->descriptor->Write( file ) ) ;

	if ( !m_impl->to_unicode.Is<void>( ) )
		dict.insert( "ToUnitcode", 	file->WriteObj( m_impl->to_unicode ) ) ;

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
	return m_impl->base_font.Str( ) ;
}

const Glyph* SimpleFont::GetGlyph( wchar_t ch ) const
{
	if ( m_impl->encoding != 0 )
		ch = m_impl->encoding->LookUp( ch ) ;
//		std::cout << "char " << ch << "\'" << (char)ch << "\' mapped to " << m_impl->encoding->LookUp( ch ) << std::endl ;

	Impl::GlyphMap::const_iterator it = m_impl->glyphs.find( ch ) ;
	return it != m_impl->glyphs.end() ? it->second : 0 ;
}

FontDescriptor* SimpleFont::Descriptor( )
{
	return m_impl->descriptor ;
}

} // end of namespace
