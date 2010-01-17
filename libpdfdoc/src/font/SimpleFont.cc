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

#include "core/Array.hh"
#include "core/Dictionary.hh"

#include "file/IFile.hh"
#include "file/ObjectReader.hh"

#include "util/Exception.hh"
#include "util/Util.hh"

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

SimpleFont::SimpleFont( FT_Face face )
	: m_face( face ),
	  m_base_font( ::FT_Get_Postscript_Name( face ) ),
	  m_type( GetFontType( face ) )
{
	GetWidth(
		face,
		std::back_inserter(m_widths),
		m_widths.max_size(),
		m_first_char,
		m_last_char ) ;
	
	assert( m_widths.size() ==
		static_cast<std::size_t>(m_last_char - m_first_char + 1) ) ;
}

SimpleFont::SimpleFont( Dictionary& self, IFile *file, FT_Library ft_lib )
{
	try
	{
		Name subtype ;
		if ( self.Extract( "Subtype",	subtype ) )
			m_type	= SubType( subtype ) ;

		self.Extract( Name("BaseFont"),		m_base_font ) ;
		self.Extract( Name("FirstChar"),	m_first_char ) ;
		self.Extract( Name("LastChar"),		m_last_char ) ;

		// width is optional
		Array widths ;
		if ( Detach( file, self, "Widths", widths ) )
			m_widths.assign( widths.begin(), widths.end() ) ;

//		self.Extract( "Encoding",	m_encoding ) ;
//		self.Extract( "ToUnicode",	m_to_unicode ) ;
		
		Dictionary fd ;
		if ( Detach( file, self, "FontDescriptor", fd ) )
		{
			m_descriptor.Read( fd, file ) ;
			std::vector<unsigned char> font_file ;
			m_descriptor.FontFile().CopyData( font_file ) ;
//std::ofstream f( (m_base_font.Str()+".ttf").c_str() ) ;
//std::copy( font_file.begin(), font_file.end(),
//std::ostreambuf_iterator<char>( f ) ) ;
//f.close() ;
			FT_Error e = FT_New_Memory_Face(
				ft_lib,
				&font_file[0],
				font_file.size(),
				0,
				&m_face ) ;
			if ( e != 0 )
				throw -1 ;
		}
		
		m_self.Read( self, file ) ;
	}
	catch ( Exception& e )
	{
		std::ostringstream msg ;
		msg << "cannot read font:\n" << e.what( ) << "\n"
		    << "Font Dictionary: \n" << self << "\n" ;
		throw Exception( msg.str( ) ) ;
	}
}

SimpleFont::Type SimpleFont::GetFontType( FT_Face face )
{
	const char *format = ::FT_Get_X11_Font_Format( face ) ;
	if ( format == 0 )
		return unknown ;
	else if ( ::strcmp( format, "Truetype" ) == 0 )
		return truetype ;
	else if ( ::strcmp( format, "Type 1" ) == 0 )
		return type1 ;
	else
		return unknown ;
}

template <typename OutIt>
void SimpleFont::GetWidth(
	FT_Face		face,
	OutIt		out,
	std::size_t	space,
	int&		first_char,
	int&		last_char )
{
	// traverse all characters
	unsigned		glyph ;
	unsigned long 	char_code = ::FT_Get_First_Char( face, &glyph ) ;
	first_char = static_cast<int>( char_code ) ;

	while ( glyph != 0 && char_code < 256 )
	{
		last_char = static_cast<int>( char_code ) ;
		char_code = ::FT_Get_Next_Char( face, char_code, &glyph ) ;
	}

	for ( int i = first_char ; i <= last_char && space-- > 0 ; i++ )
	{
		// load the glyph to the glyph slot in the face
		FT_Error error = ::FT_Load_Glyph(
			face,
			::FT_Get_Char_Index( face, i ),
			FT_LOAD_DEFAULT ) ;
		
		if ( error != 0 )
			throw Exception( ) ;

		*out++ = face->glyph->metrics.horiAdvance ;
	}
}

double SimpleFont::Width( const std::wstring& text, double size ) const
{
	assert( m_widths.size() ==
		static_cast<std::size_t>(m_last_char - m_first_char + 1) ) ;
	
	double width = 0.0 ;
	for ( std::wstring::const_iterator
		i = text.begin() ; i < text.end() ; ++i )
	{
		if ( *i >= m_first_char && *i <= m_last_char )
			width += (m_widths[*i-m_first_char] * size );
	}
	return width ;
/*
	if ( ch >= m_first_char && ch <= m_last_char )
		return m_widths[ch-m_first_char] ;
	else
		return 0.0 ;
*/
}

Ref SimpleFont::Write( IFile *file ) const
{
	CompleteObj dict( m_self ) ;
	dict.Get()["Type"]		= Name( "Font" ) ;
	dict.Get()["Subtype"]	= SubType( m_type ) ;
	dict.Get()["BaseFont"]	= m_base_font ;
	dict.Get()["FirstChar"]	= m_first_char ;
	dict.Get()["LastChar"]	= m_last_char ;

	if ( dict.Get().find( "Encoding" ) == dict.Get().end() )
		dict.Get()["Encoding"]		= Name("WinAnsiEncoding") ;

//	if ( !m_encoding.IsNull() )
//		dict.Get()["Encoding"]		= m_encoding ;

	if ( !m_widths.empty( ) )
		dict.Get()["Widths"]		= Array( m_widths.begin(), m_widths.end() ) ;

//	if ( !m_descriptor.IsNull( ) )
//		dict["FontDescriptor"]	= m_descriptor ;

//	if ( !m_to_unicode.IsNull( ) )
//		dict.Get()["ToUnitcode"]	= file->WriteObj( m_to_unicode ) ;

	return dict.Write( file ) ;
}

const Name& SimpleFont::SubType( Type t )
{
	assert( t >= truetype && t <= type0 ) ;
	return m_font_types[t] ;
}

SimpleFont::Type SimpleFont::SubType( const Name& name )
{
	const Name *ptr = std::find( pdf::Begin( m_font_types ),
	                             pdf::End( m_font_types ), name ) ;
	if ( ptr == pdf::End( m_font_types ) )
	{
		std::ostringstream os ;
		os << "unknown font type: " << name ;
		throw Exception( os.str() ) ;
	}

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

BaseFont* CreateFont( Dictionary& obj, IFile *file, FT_Library ft_lib )
{
	return new SimpleFont( obj, file, ft_lib ) ;
}

} // end of namespace
