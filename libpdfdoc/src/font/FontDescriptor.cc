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

/**	\file	FontDescriptor.cc
	\brief	implementation of the FontDescriptor class
	\date	Jan 16, 2010
	\author	Nestal Wan
*/

#include "FontDescriptor.hh"

#include "core/Array.hh"
#include "core/Dictionary.hh"
#include "file/IFile.hh"
#include "file/ObjectReader.hh"
#include "stream/Stream.hh"

#include "util/Util.hh"
#include "util/Debug.hh"

#include FT_TRUETYPE_IDS_H
#include FT_TRUETYPE_TABLES_H

#include <iostream>
#include <iterator>
#include <fstream>

namespace pdf {

const Name FontDescriptor::m_stretch_names[] =
{
	"UltraCondensed",	///< usWidthClass: 1
	"ExtraCondensed",	///< usWidthClass: 2
	"Condensed",		///< usWidthClass: 3
	"SemiCondensed",	///< usWidthClass: 4
	"Normal",			///< usWidthClass: 5
	"SemiExpanded",		///< usWidthClass: 6
	"Expanded",			///< usWidthClass: 7
	"ExtraExpanded",	///< usWidthClass: 8
	"UltraExpanded",	///< usWidthClass: 9
} ;

/**	constructor
*/
FontDescriptor::FontDescriptor( )
	: m_flags( 0 )
{
	m_italic_angle = m_ascent = m_descent = m_leading = 0.0 ;
}

FontDescriptor::FontDescriptor( FT_Face face, std::vector<unsigned char>& prog )
	: m_flags( 0 )
{
	PDF_ASSERT( face != 0 ) ;
	
	TT_OS2	*os2 = reinterpret_cast<TT_OS2*>(
		FT_Get_Sfnt_Table( face, ft_sfnt_os2 ) ) ;
	
	TT_Postscript	*post = reinterpret_cast<TT_Postscript*>(
		FT_Get_Sfnt_Table( face, ft_sfnt_post ) ) ;
	
	m_psname		= FT_Get_Postscript_Name( face ) ;

	m_ascent 		= FontUnit(face->ascender,	face) ;
	m_descent		= FontUnit(face->descender,	face) ;
	m_leading		= 0 ;					// TODO: see other implementation
	
	if ( os2 != 0 )
	{
		// prefer to use OS/2 table if present
		m_ascent 		= FontUnit(os2->sTypoAscender,	face) ;
		m_descent		= FontUnit(os2->sTypoDescender,	face) ;
		
		m_cap_height	= FontUnit(	os2->version == 0x0002
				? os2->sCapHeight
				: os2->sTypoAscender,
			face ) ;	// arbitrary 
		
		// optional
		m_x_height	= FontUnit(os2->version == 0x0002
				? os2->sxHeight
				: 0.0,
			face ) ;
		m_avg_width	= FontUnit(os2->xAvgCharWidth != 0
				? os2->xAvgCharWidth
				: 0.0,
			face ) ;
		
		// I don't know how to get this one
		m_stemv		= (os2->usWeightClass/65.0) * (os2->usWeightClass/65.0)+ 50;
		
		// we define the Stretch enum to suit the usWidthClass field
		m_stretch	= static_cast<Stretch>( os2->usWidthClass ) ;
	}
	
	m_x_min	= FontUnit( face->bbox.xMin, face ) ;
	m_x_max = FontUnit( face->bbox.xMax, face ) ;
	m_y_min = FontUnit( face->bbox.yMin, face ) ;
	m_y_max = FontUnit( face->bbox.yMax, face ) ;

	m_italic_angle	= (post != 0 ? FontUnit(post->italicAngle, face) : 0.0) ;
	m_leading = 0.0 ;
/*
	FT_ULong length = 0 ;
	FT_Load_Sfnt_Table( face, 0, 0, 0, &length ) ;
std::cout << "file length is: " << length << std::endl ;

	m_font_file.resize( length ) ;
	FT_Load_Sfnt_Table( face, 0, 0, &m_font_file[0], &length ) ;
std::cout << "file length is: " << m_font_file.size() << std::endl ;

//	FT_Load_Sfnt_Table( face, 0, 0, &m_font_file[0], &length ) ;
*/
	m_font_file.swap( prog ) ;
}

void FontDescriptor::Read( Dictionary& self, IFile *file )
{
	// font file can be in FontFile, FontFile2 or 3, depending on font type
	Stream prog ;
	bool r =
		Detach( file, self, "FontFile", 	prog ) ||
		Detach( file, self, "FontFile2", 	prog ) ||
		Detach( file, self, "FontFile3", 	prog ) ;
	if ( r )
		prog.CopyData( m_font_file ) ;
	
	// optional font family name. normally empty for embedded font
	Detach( file, self, "FontFamily",	m_family ) ;
	
	Name stretch ;
	if ( DetachConv( file, self, "FontStretch", stretch ) )
		m_stretch = static_cast<Stretch>(std::find(
			Begin(m_stretch_names),
			End(m_stretch_names),
			stretch ) - Begin(m_stretch_names) ) ;

	DetachConv( file, self, "FontWeight",	m_weight ) ;
	DetachConv( file, self, "Flags",		m_flags ) ;
	
	Array bbox ;
	if ( Detach( file, self, "FontBBox", bbox ) )
		m_bbox = Rect( bbox.begin(), bbox.end() ) ;

	DetachConv( file, self, "ItalicAngle",	m_italic_angle ) ;
	DetachConv( file, self, "Ascent",		m_ascent ) ;
	DetachConv( file, self, "Descent",		m_descent ) ;
	DetachConv( file, self, "Leading",		m_leading ) ;
	DetachConv( file, self, "CapHeight",	m_cap_height ) ;
	DetachConv( file, self, "XHeight",		m_x_height ) ;
	DetachConv( file, self, "StemV",		m_stemv ) ;
	DetachConv( file, self, "StemH",		m_stemh ) ;
	DetachConv( file, self, "AvgWidth",		m_avg_width ) ;
	DetachConv( file, self, "MaxWidth",		m_max_width ) ;
	DetachConv( file, self, "MissingWidth",	m_miss_width ) ;
}

Ref FontDescriptor::Write( IFile *file ) const
{
	Dictionary self ;
	self["FontName"]	= Name(m_psname) ;
	self["Ascent"]		= m_ascent ;
	self["Descent"]		= m_descent ;
	self["CapHeight"]	= m_cap_height ;
	self["StemV"]		= m_stemv ;
	self["Flags"]		= 4 ;
	self["Type"]		= Name("FontDescriptor") ;
	self["ItalicAngle"]	= 0 ;
	
	if ( m_x_height != 0.0 )
		self["XHeight"]		= m_x_height ;
	
	Rect bbox( m_x_min, m_y_min, m_x_max, m_y_max ) ;
	self["FontBBox"]	= Array( bbox.begin(), bbox.end() ) ;
	
	std::vector<unsigned char> prog = m_font_file ;
	
	// embedded font program also needs Length1 for the size of the stream
	Stream s( prog, Object::NullObj() ) ;
	s.AddDictionaryEntry( "Length1", m_font_file.size() ) ;
	
	// streams must be indirect objects
	self["FontFile2"]	= file->WriteObj(s) ;
	
	self["Stretch"]		= static_cast<int>( m_stretch ) ;

	return file->WriteObj( self ) ;
}

const std::vector<unsigned char>& FontDescriptor::FontFile( ) const
{
	return m_font_file ;
}

std::string FontDescriptor::Family( ) const
{
	return m_family ;
}

double FontDescriptor::FontUnit( double val, FT_Face face )
{
	return val * 1000.0 / face->units_per_EM ; 
}

} // end of namespace
