/***************************************************************************\
 *   Copyright (C) 2006 by Nestal Wan                                      *
 *   me@nestal.net                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
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

#include "file/IElementDest.hh"
#include "file/ElementList.hh"
#include "file/DeRef.hh"

#include "util/Exception.hh"
#include "util/Util.hh"

#include "font/ftwrap/Face.hh"
#include "font/ftwrap/Glyph.hh"

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

SimpleFont::SimpleFont( freetype::Library *lib, const std::string& filename )
	: m_encoding( "WinAnsiEncoding" )
{
	freetype::Face face( lib, filename ) ;
	m_base_font = Name( face.PSName() ) ;
	m_type		= truetype ;
	
	// traverse all characters
	unsigned		glyph ;
	unsigned long 	char_code = face.GetFirstChar( glyph ), last_char ;
	m_first_char = static_cast<int>( char_code ) ;

	while ( glyph != 0 && char_code < 256 )
	{
		last_char = char_code ;
		char_code = face.GetNextChar( char_code, glyph ) ;
	}
	
	m_last_char	= static_cast<int>( last_char ) ;

	for ( int i = m_first_char ; i <= m_last_char ; i++ )
	{
		freetype::Glyph g( &face, face.GetGlyphCode( i ) ) ;
		m_widths.push_back( g.HoriAdvance( ) ) ;
	}
	assert( (int)m_widths.size() == m_last_char - m_first_char + 1 ) ;
}

void SimpleFont::Read( const Ref& link, IElementSrc *src )
{
	Dictionary dict = src->ReadObj( link ) ;
	
	try
	{
		m_type	= SubType( dict["Subtype"].As<Name>() ) ;
		
		dict.Extract( Name("BaseFont"),		m_base_font ) ;
		dict.Extract( Name("FirstChar"),	m_first_char ) ;
		dict.Extract( Name("LastChar"),		m_last_char ) ;
		
		// width is optional
		Array widths ;
		if ( dict.Extract( Name("Widths"), widths ) )
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

void SimpleFont::Write( const Ref& link, IElementDest *dest ) const
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
		dict["Widths"]			= Array( m_widths.begin( ), m_widths.end( ) ) ;
	
	if ( !m_descriptor.IsNull( ) )
		dict["FontDescriptor"]	= m_descriptor ;
	
	if ( !m_to_unicode.IsNull( ) )
		dict["ToUnitcode"]		= dest->WriteObj( m_to_unicode ) ;
	
	dest->WriteObj( dict, link ) ;
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

const SimpleFont& SimpleFont::TimesRoman( )
{
	static const SimpleFont font( Name("Times-Roman"), type1 ) ;
	return font ;
}

std::string SimpleFont::BaseName( ) const
{
	return m_base_font.Str( ) ;
}

ElementList SimpleFont::GetChildren( ) const
{
	return ElementList( ) ;
}

template <> SimpleFont* IElementSrc::Read( const Ref& link )
{
	// dynamic cast reference
	// it will throw bad_cast if failed
	IElement *temp = Find( link ) ;
	return temp != 0 ? &dynamic_cast<SimpleFont&>( *temp )
						: NewElement<SimpleFont>( link ) ;
}

} // end of namespace
