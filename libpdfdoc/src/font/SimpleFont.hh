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
#include "FontType.hh"

// libpdfdoc headers
#include "core/Array.hh"
#include "core/Name.hh"
#include "core/Object.hh"
//#include "file/CompleteObj.hh"

// boost library
#include <boost/tr1/unordered_map.hpp>

#include <vector>
#include <memory>

struct FT_FaceRec_ ;

namespace pdf {

class DictReader ;
class FontDb ;
class RealGlyph ;
class Glyph ;

///	general font for PDF files
/**	\internal

	This class represent a simple Type 1 or TrueType font. According to the
	PDF specification, a simple font has less than 256 characters.
*/
class SimpleFont : public BaseFont
{
public :
	SimpleFont( DictReader& self, FontDb *fontdb ) ;
	SimpleFont( const std::string& name, FontDb *fontdb ) ;
	~SimpleFont( ) ;

	std::string BaseName( ) const ;
	Ref Write( File *file ) const ;

	const Glyph*	GetGlyph( wchar_t ch ) const ;
	
	FontDescriptor* Descriptor( ) ;

	unsigned UnitsPerEM( ) const ;
	double FromFontUnit( unsigned val ) const ;
	
private :
	void LoadGlyphs( ) ;

	static const Name m_font_types[] ;

	static const Name&	SubType( font::Type t ) ;
	static font::Type	SubType( const Name& t ) ;

	static FT_FaceRec_* LoadFace(
		const unsigned char	*data,
		std::size_t 		size,
		FontDb 				*fontdb );

	void Init( std::vector<unsigned char>& prog, FontDb *fontdb ) ; 

	static std::vector<unsigned char> FindStdFont(
		const std::string&	name,
		FontDb 				*fontdb ) ;
	
	bool InitWithStdFont( const std::string& name, FontDb *fontdb ) ;

private :
	FT_FaceRec_	*m_face ;

	Name		m_base_font ;
	font::Type	m_type ;
	int			m_first_char, m_last_char ;

	Array 		m_widths ;
	
	std::auto_ptr<FontDescriptor>		m_descriptor ;
	Object				m_to_unicode ;
	Object				m_encoding ;	//!< name or dictionary
	
	typedef std::tr1::unordered_map<wchar_t, RealGlyph*> GlyphMap ;
	GlyphMap	m_glyphs ;
} ;

} // end of namespace

#endif
