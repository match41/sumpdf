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
#include "FontSubsetInfo.hh"
#include "subset/Sfnt.hh"

#include "core/Array.hh"
#include "core/Dictionary.hh"
#include "file/File.hh"
#include "file/DictReader.hh"
#include "stream/Stream.hh"

#include "util/CArray.hh"
#include "util/Debug.hh"

// freetype headers
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_TRUETYPE_IDS_H
#include FT_TRUETYPE_TABLES_H

#include <boost/format.hpp>

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
	: m_stretch( font::normal_width )
	, m_weight( 400 )					// 400 means normal
	, m_flags( 0 )
{
	m_ascent	= m_descent	= m_leading		= m_cap_height	= m_x_height   =
	m_stemv		= m_stemh	= m_avg_width	= m_max_width	= m_miss_width = 0 ;
	m_length1	= m_length2	= m_length3		= 0 ;
}

FontDescriptor::FontDescriptor( DictReader& self )
	: m_stretch( font::normal_width )
	, m_weight( 400 )					// 400 means normal
	, m_flags( 0 )
{
	m_ascent	= m_descent	= m_leading		= m_cap_height	= m_x_height   =
	m_stemv		= m_stemh	= m_avg_width	= m_max_width	= m_miss_width = 0 ;
	m_length1	= m_length2	= m_length3		= 0 ;
	
	Read( self ) ;
}

FontDescriptor::FontDescriptor( FT_Face face, std::vector<unsigned char>& prog )
	: m_stretch( font::normal_width )
	, m_weight( 400 )					// 400 means normal
	, m_flags( GetFlag( face ) )
{
	m_ascent	= m_descent	= m_leading		= m_cap_height	= m_x_height   =
	m_stemv		= m_stemh	= m_avg_width	= m_max_width	= m_miss_width = 0 ;
	m_length1	= m_length2	= m_length3		= 0 ;

	PDF_ASSERT( face != 0 ) ;
	m_length1 = m_length2 = m_length3 = 0 ;
	
	TT_OS2	*os2 = reinterpret_cast<TT_OS2*>(
		FT_Get_Sfnt_Table( face, ft_sfnt_os2 ) ) ;
	
	TT_Postscript	*post = reinterpret_cast<TT_Postscript*>(
		FT_Get_Sfnt_Table( face, ft_sfnt_post ) ) ;
	
	const char *psname = FT_Get_Postscript_Name( face ) ;
	m_psname		= psname ? psname : "" ;

	m_ascent 		= FontUnit(face->ascender,	face) ;
	m_descent		= FontUnit(face->descender,	face) ;
	m_leading		= 0 ;					// TODO: see other implementation
	m_stemv			= 70.0 ;				// TODO: verify?
	
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
		
		// we define the Width enum to suit the usWidthClass field
		m_stretch	= static_cast<font::Width>( os2->usWidthClass ) ;
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
	
	font::Type type = font::GetType( face ) ;
	if ( type == font::type1 )
	{
		InitType1Lengths( ) ;
	}
}

void FontDescriptor::InitType1Lengths( )
{
	unsigned char eexec[] = "eexec" ;
	std::size_t eexec_len = Count( eexec ) - 1 ;
	
	std::vector<unsigned char>::const_iterator len1 =
		std::search(
			m_font_file.begin(),
			m_font_file.end(),
			eexec,
			eexec + eexec_len ) ;

	if ( len1 != m_font_file.end() )
		m_length1 = len1 - m_font_file.begin() + eexec_len ;

	unsigned char cleartomark[] = "cleartomark" ;
	std::size_t cleartomark_len = Count( cleartomark ) - 1 ;
	
	std::vector<unsigned char>::const_iterator len2 =
		std::search(
			m_font_file.begin(),
			m_font_file.end(),
			cleartomark,
			cleartomark + cleartomark_len ) ;
	
	if ( m_length1 > 0 && len2 != m_font_file.end() )
	{
		m_length2 = len2 - m_font_file.begin( ) - 520 - m_length1 ;
		m_length3 = 520 ;
	}
}

std::bitset<32> FontDescriptor::GetFlag( FT_FaceRec_ *face )
{
	// TODO: use some real flags
	std::bitset<32> r ;
	r[nonsymbolic] = 1 ;
	return r ;
}

///	Read the font descriptor from file.
void FontDescriptor::Read( DictReader& reader )
{
	// font file can be in FontFile, FontFile2 or 3, depending on font type
	Stream prog ;
	
	// type1 font program is embbeded in the "FontFile" stream.
	if ( reader.Detach( "FontFile", 	prog ) )
	{
		Dictionary prog_dict = prog.Self() ;
		DictReader prog_reader( prog_dict, reader.GetFile() ) ;
		
		// it has has 3 different lengths
		if ( !prog_reader.Detach( "Length1", m_length1 ) ||
			 !prog_reader.Detach( "Length2", m_length2 ) ||
			 !prog_reader.Detach( "Length3", m_length3 ) )
			throw FontException()
				<< expt::ErrMsg( "missing length for type 1 font" ) ; 
	}

	// truetype font program is embbeded in the "FontFile2" stream
	else if ( reader.Detach( "FontFile2", prog ) )
	{
	}
	
	// CFF and open type are in "FontFile3"
	else if ( reader.Detach( "FontFile3", prog ) )
	{
	}
	
	prog.CopyData( m_font_file ) ;
	
	// optional font family name. normally empty for embedded font
	reader.Detach( "FontFamily",	m_family ) ;
	
	Name stretch ;
	if ( reader.Detach( "FontStretch", stretch ) )
		m_stretch = static_cast<font::Width>(std::find(
			Begin(m_stretch_names),
			End(m_stretch_names),
			stretch ) - Begin(m_stretch_names) ) ;

	reader.Detach( "FontWeight",	m_weight ) ;
	
	int flags ;
	if ( reader.Detach( "Flags",	flags ) )
		m_flags = flags ;
		
	reader.Detach( "FontBBox",		m_bbox ) ;
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

Stream FontDescriptor::WriteTrueTypeFont(
	FT_FaceRec_ 				*face,
	const std::vector<long>&	glyphs ) const
{
	PDF_ASSERT( FT_IS_SFNT( face ) ) ;

	std::size_t size = m_font_file.size() ;
	
	Stream s( Stream::deflate ) ;
	
	// write the whole font
	if ( glyphs.empty() )
		s.Append( &m_font_file[0], m_font_file.size() ) ;
		
	// write font subset
	else
	{
		Sfnt sfnt( face ) ;
		std::vector<uchar> subset = sfnt.CreateSubset(
			&glyphs[0], glyphs.size() ) ;
		s.Append( &subset[0], subset.size() ) ;
		
		size = subset.size( ) ;
	}
	s.Flush( ) ;
	
	// Length1 indicates original uncompressed size
	Dictionary& sdict = s.Self() ;
	sdict.insert( std::make_pair( "Length1", size ) ) ;
	return s ;
}

Ref FontDescriptor::Write(
	File 						*file,
	const std::vector<long>&	glyphs,
	FT_FaceRec_ 				*face ) const
{
	PDF_ASSERT( file != 0 ) ;

	Dictionary self ;
	if ( !m_psname.empty() )
		self.insert( "FontName", Name(m_psname) ) ;
	
	if ( !m_family.empty() )
		self.insert( "Family", m_family ) ;

	self.insert( "Ascent",		m_ascent ) ;
	self.insert( "Descent",		m_descent ) ;
	self.insert( "CapHeight",	m_cap_height ) ;
	self.insert( "StemV",		m_stemv ) ;
	self.insert( "Flags",		static_cast<int>( m_flags.to_ulong() ) ) ;
	self.insert( "Type",		Name("FontDescriptor") ) ;
	self.insert( "ItalicAngle",	0 ) ;
	
	if ( m_x_height != 0.0 )
		self.insert( "XHeight", m_x_height ) ;
	
	self.insert( "FontBBox",	m_bbox ) ;
	
	if ( !m_font_file.empty() )
	{
		font::Type type = font::GetType( face ) ;
		if ( type == font::truetype )
		{
			Stream ttf = WriteTrueTypeFont( face, glyphs ) ;
			self.insert( "FontFile2", file->WriteObj( ttf ) ) ;
		}
		else
		{
			Stream s( Stream::deflate ) ;
			s.Append( &m_font_file[0], m_font_file.size() ) ;
			s.Flush( ) ;
			
			Name prog_key ;
			
			Dictionary& sdict = s.Self() ;
			
			if ( type == font::type1 )
			{
				sdict.insert( std::make_pair( "Length1", m_length1 ) ) ;
				sdict.insert( std::make_pair( "Length2", m_length2 ) ) ;
				sdict.insert( std::make_pair( "Length3", m_length3 ) ) ;
				prog_key = "FontFile" ;
			}
			else if ( type == font::opentype_cff )
			{
				sdict.insert( std::make_pair( "Subtype", Name("OpenType") ) ) ;
				prog_key = "FontFile3" ;
			}
			else if ( type == font::type2 )
			{
				sdict.insert( std::make_pair( "Subtype", Name("Type1C") ) ) ;
				prog_key = "FontFile3" ;
			}
			else
				throw Unsupported( )
					<< expt::ErrMsg( "unsupported font type" )
					<< expt::Index( type ) ;
			
			self.insert( prog_key, file->WriteObj( s ) ) ;
		}
	}
	
	if ( m_stretch >= font::ultra_condensed &&
	     m_stretch <= font::ultra_expanded )  
		self.insert( "Stretch", m_stretch_names[m_stretch] ) ;

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
	PDF_ASSERT( face != 0 ) ;
	return val * 1000.0 / face->units_per_EM ; 
}

bool FontDescriptor::IsSymbolic() const
{
	return m_flags[symbolic] ;
}

} // end of namespace
