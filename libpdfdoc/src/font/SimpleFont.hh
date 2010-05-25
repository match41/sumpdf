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
#include "font/FontType.hh"

#include <vector>
#include <memory>

struct FT_FaceRec_ ;

namespace pdf {

class DictReader ;
class FontDb ;
class FontEncoding ;
class RealGlyph ;
class Glyph ;
class Name ;

///	general font for PDF files
/**	\internal

	This class represent a simple Type 1 or TrueType font. According to the
	PDF specification, a simple font has less than 256 characters.
*/
class SimpleFont : public BaseFont
{
public :
	SimpleFont( DictReader& self, FontDb *font_db ) ;
	SimpleFont( const std::string& path, FontDb *font_db ) ;
	SimpleFont( FT_FaceRec_ *face, std::vector<unsigned char>& prog ) ;
	~SimpleFont( ) ;

	std::string BaseName( ) const ;
	Ref Write( File *file, const FontSubsetInfo *subset ) const ;

	const Glyph*	GetGlyph( wchar_t ch ) const ;
	
	FontDescriptor* Descriptor( ) ;
	FontEncoding* Encoding( ) ;

	unsigned UnitsPerEM( ) const ;
	double FromFontUnit( unsigned val ) const ;
	double Height( ) const ;
	
	bool IsSubset( ) const ;
	
private :
	void LoadGlyphs( ) ;

	static const Name m_font_types[] ;

	static const Name&	SubType( font::Type t ) ;
	static font::Type	SubType( const Name& t ) ;

	static FT_FaceRec_* LoadFace(
		const unsigned char	*data,
		std::size_t 		size,
		FontDb 				*fontdb );

	void Init( FT_FaceRec_ *face, std::vector<unsigned char>& prog ) ;

	static std::vector<unsigned char> FindStdFont(
		const std::string&	name,
		FontDb 				*fontdb ) ;
	
	bool InitWithStdFont( const std::string& name, FontDb *fontdb ) ;

	bool LoadDescriptor( DictReader& reader, FontDb *font_db ) ;
	bool LoadFontProgram( FontDb *font_db ) ;
	void LoadEncoding( DictReader& reader ) ;


	static bool IsSubset( const std::string& basename ) ;

private :
	struct Impl ;
	std::auto_ptr<Impl>	m_impl ;
} ;

} // end of namespace

#endif
