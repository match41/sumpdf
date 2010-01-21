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

#include "FontDescriptor.hh"

// libpdfdoc headers
#include "core/Name.hh"
#include "core/Object.hh"
#include "file/CompleteObj.hh"

#include <vector>

namespace pdf {

class Dictionary ;

/*!	\brief	general font for PDF files
	\internal

	This class represent a simple Type 1 or TrueType font. According to the
	PDF specification, a simple font has less than 256 characters.
*/
class SimpleFont : public BaseFont
{
public :
	enum Type { truetype, type1, mmtype1, type3, type0, unknown } ;

public :
	SimpleFont( ) ;
	SimpleFont( Dictionary& self, IFile *file, FT_Library ft_lib ) ;
	SimpleFont( const std::string& font_file, unsigned idx, FT_Library ft_lib );
	~SimpleFont( ) ;

#ifdef HAVE_FONTCONFIG
	SimpleFont( const std::string& name, FT_Library ft_lib ) ;
#endif

	std::string BaseName( ) const ;
	Ref Write( IFile *file ) const ;

	double Width( const std::wstring& text, double size ) const ;
	
	FT_Face Face( ) const ;
	
	FontDescriptor* Descriptor( ) ;
	
private :
	SimpleFont( const Name& base_font, Type type ) ;

	void Init( ) ;

	static const Name m_font_types[] ;
	static Type GetFontType( FT_Face face ) ;

	static const Name& SubType( Type t ) ;
	static Type        SubType( const Name& t ) ;

	template <typename OutIt>
	static void GetWidth(
		FT_Face		face,
		OutIt		out,
		std::size_t	space,
		int&		first_char,
		int&		last_char ) ;

	void ReadDescriptor( Dictionary& fd, FT_Library ft_lib, IFile *file ) ;

	static FT_Face LoadFace(
		const std::string& 	file,
		unsigned 			idx,
		FT_Library 			ft_lib );

#ifdef HAVE_FONTCONFIG
	static FT_Face FindFont( const std::string& font, FT_Library ft_lib ) ;
#endif

private :
	CompleteObj			m_self ;

	FT_Face	m_face ;

	Name	m_base_font ;
	Type	m_type ;
	int		m_first_char, m_last_char ;

	std::vector<int>	m_widths ;
	FontDescriptor		m_descriptor ;
//	Object				m_to_unicode ;
//	Object				m_encoding ;	//!< name or dictionary
} ;

} // end of namespace

#endif
