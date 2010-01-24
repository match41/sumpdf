/***************************************************************************
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
 ***************************************************************************/

/*!
	\file	SimpleFont.hh
	\brief	definition the SimpleFont class
	\date	Sat Mar 22 2008
	\author	Nestal Wan
*/

#ifndef __PDF_REAL_FONT_HEADER_INCLUDED__
#define __PDF_REAL_FONT_HEADER_INCLUDED__

#include "BaseFont.hh"

#include "font/Glyph.hh"

// libpdfdoc headers
#include "core/Name.hh"
#include "core/Object.hh"
#include "file/CompleteObj.hh"

// boost library
#include <boost/tr1/unordered_map.hpp>


// freetype headers
#include <ft2build.h>
#include FT_FREETYPE_H

#include <vector>
#include <memory>

namespace pdf {

class Dictionary ;

///	general font for PDF files
/**	\internal

	This class represent a simple Type 1 or TrueType font. According to the
	PDF specification, a simple font has less than 256 characters.
*/
class SimpleFont : public BaseFont
{
public :
	enum Type { truetype, type1, mmtype1, type3, type0, unknown } ;

public :
	SimpleFont( Dictionary& self, IFile *file, FT_Library ft_lib ) ;
	SimpleFont( const std::string& font_file, unsigned idx, FT_Library ft_lib );
	SimpleFont( const std::string& name, FT_Library ft_lib ) ;
	~SimpleFont( ) ;

	std::string BaseName( ) const ;
	Ref Write( IFile *file ) const ;

	const Glyph*	GetGlyph( wchar_t ch ) const ;
	
	FontDescriptor* Descriptor( ) ;

	double Width( const std::wstring& text, double size ) const ;
	double Width( const Glyph& glyph ) const ;	
	unsigned UnitsPerEM( ) const ;
	double FromFontUnit( unsigned val ) const ;
	
private :
	void LoadGlyphs( ) ;

	static const Name m_font_types[] ;
	static Type GetFontType( FT_Face face ) ;

	static const Name& SubType( Type t ) ;
	static Type        SubType( const Name& t ) ;

	static FT_Face LoadFace(
		const unsigned char	*data,
		std::size_t 		size,
		FT_Library 			ft_lib );

	void Init( std::vector<unsigned char>& prog, FT_Library ft_lib ) ; 

	static std::string FindFont(
		const std::string& font_name,
		const std::string& style = "normal" ) ;
	static std::vector<unsigned char> LoadFile( const std::string& filename ) ;

	static std::string FindStdFont( const std::string& name ) ;

private :
	CompleteObj	m_self ;

	FT_Face	m_face ;

	Name	m_base_font ;
	Type	m_type ;
	int		m_first_char, m_last_char ;

	std::auto_ptr<FontDescriptor>		m_descriptor ;
//	Object				m_to_unicode ;
//	Object				m_encoding ;	//!< name or dictionary
	
	typedef std::tr1::unordered_map<wchar_t, Glyph> GlyphMap ;
	GlyphMap	m_glyphs ;
} ;

} // end of namespace

#endif
