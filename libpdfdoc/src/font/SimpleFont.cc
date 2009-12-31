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

//#include "ftwrap/Face.hh"
//#include "ftwrap/Glyph.hh"

#include FT_XFREE86_H

namespace pdf {

const Name SimpleFont::m_font_types[] =
{
	Name("TrueType"), Name("Type1"), Name("MMType1"), Name("Type3"),
	Name("Type0" ),
} ;

SimpleFont::SimpleFont( )
	: m_base_font( "Helvetica-Bold" ),
	  m_type( type1 ),
	  m_encoding( "WinAnsiEncoding" )
{
	m_first_char = m_last_char = 0 ;
}

SimpleFont::SimpleFont( const Name& base_font, Type type )
	: m_base_font( base_font ),
	  m_type( type ),
	  m_encoding( "WinAnsiEncoding" )
{
	m_first_char = m_last_char = 0 ;
}

SimpleFont::SimpleFont( FT_Face face )
	: m_face( face ),
	  m_base_font( ::FT_Get_Postscript_Name( face ) ),
	  m_type( GetFontType( face ) ),
	  m_encoding( "WinAnsiEncoding" )
{
	GetWidth( face, m_widths, m_first_char, m_last_char ) ;
	assert( (int)m_widths.size() == m_last_char - m_first_char + 1 ) ;
}

SimpleFont::SimpleFont( const Object& self, IFile *file )
{
	Dictionary dict = DeRefObj<Dictionary>( file, self ) ;

	try
	{
		m_type	= SubType( dict["Subtype"].As<Name>() ) ;

		dict.Extract( Name("BaseFont"),		m_base_font ) ;
		dict.Extract( Name("FirstChar"),	m_first_char ) ;
		dict.Extract( Name("LastChar"),		m_last_char ) ;

		// width is optional
		Array widths ;
		if ( Detach( file, dict, "Widths", widths ) )
			m_widths.assign( widths.begin(), widths.end() ) ;

		m_encoding		= dict["Encoding"] ;
		m_descriptor	= dict["FontDescriptor"] ;
		m_to_unicode	= dict["ToUnicode"] ;
	}
	catch ( Exception& e )
	{
		std::ostringstream msg ;
		msg << "cannot read font:\n" << e.what( ) << "\n"
		    << "Font Dictionary: \n" << dict << "\n" ;
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

void SimpleFont::GetWidth(
	FT_Face	face,
	std::vector<int>& width,
	int&	first_char,
	int&	last_char )
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

	for ( int i = first_char ; i <= last_char ; i++ )
	{
		// load the glyph to the glyph slot in the face
		FT_Error error = ::FT_Load_Glyph(
			face,
			::FT_Get_Char_Index( face, i ),
			FT_LOAD_DEFAULT ) ;
		
		if ( error != 0 )
			throw Exception( ) ;

		width.push_back( face->glyph->metrics.horiAdvance ) ;
	}
//	assert( (int)m_widths.size() == m_last_char - m_first_char + 1 ) ;
}

Ref SimpleFont::Write( IFile *file ) const
{
	Dictionary dict ;
	dict["Type"]		= Name( "Font" ) ;
	dict["Subtype"]		= SubType( m_type ) ;
	dict["BaseFont"]	= m_base_font ;
	dict["FirstChar"]	= m_first_char ;
	dict["LastChar"]	= m_last_char ;

	if ( !m_encoding.IsNull() )
		dict["Encoding"]		= m_encoding ;

	if ( !m_widths.empty( ) )
		dict["Widths"]			= Array( m_widths.begin(), m_widths.end() ) ;

	if ( !m_descriptor.IsNull( ) )
		dict["FontDescriptor"]	= m_descriptor ;

	if ( !m_to_unicode.IsNull( ) )
		dict["ToUnitcode"]		= file->WriteObj( m_to_unicode ) ;

	return file->WriteObj( dict ) ;
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

BaseFont* CreateFont( const Object& obj, IFile *file )
{
	return new SimpleFont( obj, file ) ;
}

} // end of namespace
