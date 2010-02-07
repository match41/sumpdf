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

#include "FontException.hh"

#include "core/Array.hh"
#include "core/Dictionary.hh"
#include "file/File.hh"
#include "file/DictReader.hh"
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
	"padding",			///< to match the OS/2 table in truetype font enum
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

/**	\internal	constructor
*/
FontDescriptor::FontDescriptor( )
	: m_type( font::unknown ),
	  m_flags( 0 )
{
	m_length1 = m_length2 = m_length3 = 0 ;
	m_italic_angle = m_ascent = m_descent = m_leading = 0.0 ;
}

FontDescriptor::FontDescriptor( FT_Face face, std::vector<unsigned char>& prog )
	: m_type( font::GetType( face ) ),
	  m_flags( 0 )
{
	PDF_ASSERT( face != 0 ) ;
	m_length1 = m_length2 = m_length3 = 0 ;
	
	TT_OS2	*os2 = reinterpret_cast<TT_OS2*>(
		FT_Get_Sfnt_Table( face, ft_sfnt_os2 ) ) ;
	
	TT_Postscript	*post = reinterpret_cast<TT_Postscript*>(
		FT_Get_Sfnt_Table( face, ft_sfnt_post ) ) ;
	
	const char *psname = FT_Get_Postscript_Name( face ) ;
	m_psname		= psname ? psname : "" ;
//	PDF_ASSERT( !m_psname.empty() ) ;

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
	
	double x_min = FontUnit( face->bbox.xMin, face ) ;
	double x_max = FontUnit( face->bbox.xMax, face ) ;
	double y_min = FontUnit( face->bbox.yMin, face ) ;
	double y_max = FontUnit( face->bbox.yMax, face ) ;
	m_bbox = Rect( x_min, y_min, x_max, y_max ) ; 

	m_italic_angle	= (post != 0 ? FontUnit(post->italicAngle, face) : 0.0) ;
	m_leading = 0.0 ;
	
	// steal the buffer. if we don't steal it, then it will be destroyed later.
	// then the FT_Face will become invalid.
	m_font_file.swap( prog ) ;
}

///	Read the font descriptor from file.
void FontDescriptor::Read( font::Type type, DictReader& reader, File *file )
{
	m_type = type ;
	
	// font file can be in FontFile, FontFile2 or 3, depending on font type
	Stream prog ;
	if ( m_type == font::type1 )
	{
		// type1 font has 3 different lengths
		if ( reader.Detach( "FontFile", 	prog ) )
		{
			Dictionary prog_dict = prog.Self() ;
			DictReader prog_reader( prog_dict, file ) ;
			if ( !prog_reader.Detach( "Length1", m_length1 ) ||
				 !prog_reader.Detach( "Length2", m_length2 ) ||
				 !prog_reader.Detach( "Length3", m_length3 ) )
				throw FontException( "missing length for type 1 font" ) ; 
		}
	}
	else if ( m_type == font::truetype )
		reader.Detach( "FontFile2", 	prog ) ;
	
	// TODO: confirm FontFile3 type
	else if ( m_type == font::type3 )
		reader.Detach( "FontFile3", 	prog ) ;
	
	prog.CopyData( m_font_file ) ;
	
	// optional font family name. normally empty for embedded font
	reader.Detach( "FontFamily",	m_family ) ;
	
	Name stretch ;
	if ( reader.Detach( "FontStretch", stretch ) )
		m_stretch = static_cast<Stretch>(std::find(
			Begin(m_stretch_names),
			End(m_stretch_names),
			stretch ) - Begin(m_stretch_names) ) ;

	reader.Detach( "FontWeight",	m_weight ) ;
	
	int flags ;
	if ( reader.Detach( "Flags",	flags ) )
		m_flags = flags ;
	
	Array bbox ;
	if ( reader.Detach( "FontBBox", bbox ) )
		m_bbox = Rect( bbox.begin(), bbox.end() ) ;

	reader.Detach( "ItalicAngle",	m_italic_angle ) ;
	reader.Detach( "Ascent",		m_ascent ) ;
	reader.Detach( "Descent",		m_descent ) ;
	reader.Detach( "Leading",		m_leading ) ;
	reader.Detach( "CapHeight",		m_cap_height ) ;
	reader.Detach( "XHeight",		m_x_height ) ;
	reader.Detach( "StemV",			m_stemv ) ;
	reader.Detach( "StemH",			m_stemh ) ;
	reader.Detach( "AvgWidth",		m_avg_width ) ;
	reader.Detach( "MaxWidth",		m_max_width ) ;
	reader.Detach( "MissingWidth",	m_miss_width ) ;
	
	Name	psname ;
	if ( reader.Detach( "FontName",	psname) )
		m_psname = psname.Str() ;
}

Ref FontDescriptor::Write( File *file ) const
{
	PDF_ASSERT( file != 0 ) ;

	Dictionary self ;
	if ( !m_psname.empty() )
		self["FontName"]	= Name(m_psname) ;
	
	if ( !m_family.empty() )
		self["Family"]		= m_family ;
	
	self["Ascent"]		= m_ascent ;
	self["Descent"]		= m_descent ;
	self["CapHeight"]	= m_cap_height ;
	self["StemV"]		= m_stemv ;
	self["Flags"]		= static_cast<int>( m_flags.to_ulong() ) ;
	self["Type"]		= Name("FontDescriptor") ;
	self["ItalicAngle"]	= 0 ;
	
	if ( m_x_height != 0.0 )
		self["XHeight"]		= m_x_height ;
	
//	Rect bbox( m_x_min, m_y_min, m_x_max, m_y_max ) ;
	self["FontBBox"]	= Array( m_bbox.begin(), m_bbox.end() ) ;
	
	// embedded font program also needs Length1 for the size of the stream
	if ( !m_font_file.empty() )
	{
		Stream s( Stream::deflate ) ;
		s.Append( &m_font_file[0], m_font_file.size() ) ;
		s.Flush( ) ;
		
		// it seems the OpenType can work for both truetype and type1 fonts
		// we add it anyway
//		s.AddDictionaryEntry( "Subtype", Name("OpenType" ) ) ;
		
		if ( m_type == font::truetype )
		{
			s.AddDictionaryEntry( "Length1", m_font_file.size() ) ;
			self["FontFile2"]	= file->WriteObj( s ) ;
		}
		else if ( m_type == font::type1 )
		{
			s.AddDictionaryEntry( "Length1", m_length1 ) ;
			s.AddDictionaryEntry( "Length2", m_length2 ) ;
			s.AddDictionaryEntry( "Length3", m_length3 ) ;
			self["FontFile"]	= file->WriteObj( s ) ;
		}
		else
			self["FontFile3"]	= file->WriteObj( s ) ;
	}
	
	if ( m_stretch >= ultra_condensed && m_stretch <= ultra_expanded )  
		self["Stretch"]	= m_stretch_names[m_stretch] ;

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
