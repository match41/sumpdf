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
#include "FontSubsetInfo.hh"

#include "encoding/AdobeCMap.hh"
#include "encoding/SimpleEncoding.hh"
#include "encoding/BuildInEncoding.hh"
#include "encoding/Type1Encoding.hh"

// libpdfdoc headers
#include "core/Array.hh"
#include "core/Name.hh"
#include "core/Object.hh"
#include "core/Dictionary.hh"

#include "stream/Stream.hh"

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
	BaseEncoding		*encoding ;		//!< name or dictionary
	
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

// the "Subtype" field of the font dictionary 
const Name SimpleFont::m_font_types[] =
{
	Name("TrueType"),
	Name("Type0" ),
	Name("Type1" ),
	Name("Type1" ),  	// CFF/type2 font is treated as type1
	Name("Type3"),
	Name("MMType1"),	
	
	Name("Type1"),		// CFF opentype font is treated as type1
	Name("TrueType"),	// opentype font with glyf table is treated as truetype
} ;

SimpleFont::SimpleFont( const std::string& path, FontDb *font_db )
	: m_impl( new Impl )
{
	PDF_ASSERT( font_db != 0 ) ;
	
	// load font data from file
	std::ifstream file( path.c_str(), std::ios::binary | std::ios::in ) ;
	std::vector<unsigned char> prog(
		(std::istreambuf_iterator<char>( file )),
		(std::istreambuf_iterator<char>()) ) ;
	
	Init( font_db->LoadFont( &prog[0], prog.size() ), prog ) ;
	
	PDF_ASSERT( m_impl->face != 0 ) ;
}

SimpleFont::SimpleFont( FT_FaceRec_ *face, std::vector<unsigned char>& prog )
	: m_impl( new Impl )
{
	PDF_ASSERT( face != 0 ) ;
	Init( face, prog ) ;
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

		if ( LoadDescriptor( reader, font_db ) && LoadFontProgram( font_db ) )
		{
			LoadEncoding( reader ) ;
			LoadGlyphs( ) ;
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
		
		if ( reader.Detach( "ToUnicode",	m_impl->to_unicode ) )
		{
			std::istream is(m_impl->to_unicode.As<Stream>().InStreamBuf() ) ;
			AdobeCMap cmp( is ) ;
		}
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

void SimpleFont::LoadEncoding( DictReader& reader )
{
	try
	{
		BaseEncoding *enc = 0 ;
	
		Dictionary::iterator it = reader->find( "Encoding" ) ;
		if ( it != reader->end() )
		{
			if ( it->second.Is<Ref>() )
			{
				Object obj = reader.GetFile()->ReadObj( it->second ) ;
				if ( obj.Is<Name>() )
					enc = new BuildInEncoding( obj.As<Name>() ) ;
				else
				{
					ElementFactory<> f( reader ) ;
					enc = f.Create<SimpleEncoding>(
						"Encoding",
						NewPtr<SimpleEncoding>() ) ;
				}
			}
			else if ( it->second.Is<Name>() )
				enc = new BuildInEncoding( it->second.As<Name>() ) ;
		}
		
		if ( enc == 0 && m_impl->type == font::type1 )
			enc = new Type1Encoding( m_impl->face ) ;
		
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
		{
		}
	}
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
	m_impl->descriptor = f.Create<FontDescriptor>(
		"FontDescriptor", NewPtr<FontDescriptor>(), m_impl->descriptor ) ;

	return m_impl->descriptor != 0 ;
}

bool SimpleFont::LoadFontProgram( FontDb *font_db )
{
	if ( m_impl->descriptor != 0 )
	{
		const std::vector<unsigned char>& font_file
			= m_impl->descriptor->FontFile( ) ;
		
		if ( !font_file.empty() )
		{
			m_impl->face = font_db->LoadFont( &font_file[0], font_file.size() );
			
			PDF_ASSERT( m_impl->face != 0 ) ;
			m_impl->type = font::GetType( m_impl->face ) ;
			
			return true ;
		}
	}
	return false ;
}

bool SimpleFont::InitWithStdFont( const std::string& name, FontDb *font_db )
{
	if ( !name.empty() )
	{
		PDF_ASSERT( font_db != 0 ) ;
		std::vector<unsigned char> prog = FindStdFont( name, font_db ) ;
		Init( font_db->LoadFont( &prog[0], prog.size() ), prog ) ;
		return true ;
	}
	else
		return false ;
}
	
void SimpleFont::Init( FT_FaceRec_ *face, std::vector<unsigned char>& prog )
{
	m_impl->face = face ;

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

bool SimpleFont::IsSubset( const std::string& basename )
{
	return basename.size() > 7 && basename[6] == '+' &&
		std::find_if( basename.begin(), basename.begin() + 6,
			islower ) == basename.begin() + 6 ;
}

bool SimpleFont::IsSubset( ) const
{
	return IsSubset( m_impl->base_font.Str() ) ;
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
		FT_Error error = FT_Load_Glyph(
			m_impl->face,
			gindex,
			FT_LOAD_NO_SCALE ) ;
		
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
			std::cerr	<< "font " << BaseName() << " has a non-outline glyph"
						<< std::endl ;

		// use encoding to lookup the character code to unicode.
		// if the selected charmap for the font is unicode, then char_code is
		// already unicode, no need to lookup.
		wchar_t unicode =
			m_impl->encoding != 0                                  &&
			m_impl->face->charmap->encoding != FT_ENCODING_UNICODE ?
				m_impl->encoding->ToUnicode(
					static_cast<unsigned short>(char_code) ) :
				static_cast<wchar_t>( char_code ) ;

		m_impl->glyphs.insert( std::make_pair(
			unicode,
			new RealGlyph( gindex, m_impl->face ) ) ) ;

		last_char = static_cast<int>( char_code ) ;
		char_code = ::FT_Get_Next_Char( m_impl->face, char_code, &gindex ) ;
	}
	
	if ( m_impl->first_char == -1 )
		m_impl->first_char = std::min( first_char, 255 ) ;
	if ( m_impl->last_char == -1 )
		m_impl->last_char = std::min( last_char, 255 ) ;
}

/// Convert Freetype font unit to PDF glyph unit
double SimpleFont::FromFontUnit( unsigned val ) const
{
	return val * 1000.0 / m_impl->face->units_per_EM ;
}

Ref SimpleFont::Write( File *file, const FontSubsetInfo *subset ) const
{
	PDF_ASSERT( file != 0 ) ;

	Dictionary dict ;
	dict.insert( "Type", 		Name( "Font" ) ) ;
	dict.insert( "Subtype", 	SubType( m_impl->type ) ) ;
	
	int first_char	= m_impl->first_char ;
	int last_char	= m_impl->last_char ;

	Name base_font = m_impl->base_font ;

	// TODO: only truetype support subset for now
	std::vector<long> glyphs ;
	if ( subset != 0 && !IsSubset( ) && m_impl->type == font::truetype )
	{
		std::vector<wchar_t> ch = subset->GetUsedChars( this ) ;
		
		for ( std::vector<wchar_t>::iterator i = ch.begin() ; i != ch.end() ;
			++i )
			glyphs.push_back( FT_Get_Char_Index( m_impl->face, *i ) ) ;
		
		// used chars are sorted accending
		if ( !ch.empty() )
		{
			first_char	= ch.front() ;
			last_char	= ch.back() ;
		}

		base_font = Name( "AAAAAA+" + base_font.Str() ) ;
	}

	// BaseFont is optional for type 3 fonts
	if ( m_impl->type != font::type3 && !m_impl->base_font.empty() )
		dict.insert( "BaseFont", 	base_font ) ;

	dict.insert( "FirstChar", 	first_char ) ;
	dict.insert( "LastChar", 	last_char ) ;
	
	// write the font encoding
	if ( m_impl->encoding != 0 )
		dict.insert( "Encoding", m_impl->encoding->Write( file ) ) ;
	
	if ( m_impl->widths.empty() )
	{
		std::vector<double> widths ;
		for ( int i = first_char ; i <= last_char ; ++i )
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

	dict.insert( "FontDescriptor", 
		m_impl->descriptor->Write( file, glyphs, m_impl->face ) ) ;

	if ( !m_impl->to_unicode.Is<void>( ) )
		dict.insert( "ToUnitcode", 	file->WriteObj( m_impl->to_unicode ) ) ;

	return file->WriteObj( dict ) ;
}

const Name& SimpleFont::SubType( font::Type t )
{
	PDF_ASSERT( t >= font::truetype && t < font::unknown ) ;
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
//	char encbuf[] = {
//		(char)(m_impl->face->charmap->encoding>>24),
//		(char)(m_impl->face->charmap->encoding>>16),
//		(char)(m_impl->face->charmap->encoding>>8),
//		(char)(m_impl->face->charmap->encoding),
//		0
//	} ;
//	
//	std::cout << BaseName() << " " << encbuf << " char " << ch << "\'" << (char)ch << "\' mapped to " << (char)m_impl->encoding->LookUp( ch ) << std::endl ;
	
//	if ( m_impl->encoding != 0 )
//		ch = m_impl->encoding->LookUp( ch ) ;
	
	Impl::GlyphMap::const_iterator it = m_impl->glyphs.find( ch ) ;
	return it != m_impl->glyphs.end() ? it->second : 0 ;
}

FontDescriptor* SimpleFont::Descriptor( )
{
	return m_impl->descriptor ;
}

FontEncoding* SimpleFont::Encoding( )
{
	return m_impl->encoding ;
}

} // end of namespace
