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

/**	\file	FontDescriptor.hh
    \brief	definition the FontDescriptor class
    \date	Jan 16, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_FONTDESCRIPTOR_HH_EADER_INCLUDED__
#define __PDF_FONTDESCRIPTOR_HH_EADER_INCLUDED__

#include "util/RefCounter.hh"

#include "font/FontType.hh"
#include "core/Name.hh"
#include "util/Rect.hh"

#include <string>
#include <vector>
#include <bitset>

struct FT_FaceRec_ ;

namespace pdf {

class DictReader ;
class File ;
class FontSubsetInfo ;
class Ref ;
class Stream ;

/**	\internal	\brief	A font descriptor
	The FontDescriptor class represents a font descriptor in a PDF
	file. It contains information about a font face. It is used to guess the
	appearance of a font without knowing its font program.
*/
class FontDescriptor : public RefCounter
{
public :
	FontDescriptor( ) ;
	FontDescriptor( DictReader& self ) ;
	FontDescriptor( FT_FaceRec_ *face, std::vector<unsigned char>& prog ) ;
	
	void Read( DictReader& self ) ;
	Ref Write( 
		File 						*file,
		const std::vector<long>&	glyphs,
		FT_FaceRec_ 				*face ) const ;
	
	std::string Family( ) const ;

	font::Width GetStretch( ) const ;

	double	ItalicAngle( ) const ;

	const std::vector<unsigned char>&	FontFile( ) const ;
	
	bool IsSymbolic() const ;

private :
	static const Name m_stretch_names[] ; 

	double FontUnit( double val, FT_FaceRec_ *face ) ;

	bool DecodeFontFile3( DictReader& reader, Stream& prog ) ;

	static std::bitset<32> GetFlag( FT_FaceRec_ *face ) ;

	void InitType1Lengths( ) ;

	Stream WriteTrueTypeFont(
		FT_FaceRec_ 				*face,
		const std::vector<long>&	glyphs ) const ;

private :
	std::string	m_family ;
	std::string	m_psname ;
	font::Width	m_stretch ;
	int			m_weight ;
	Rect		m_bbox ;
	double		m_italic_angle ;
	double		m_ascent, m_descent, m_leading, m_cap_height, m_x_height,
				m_stemv, m_stemh, m_avg_width, m_max_width, m_miss_width ;

	// for reading type1 font only
	int	m_length1, m_length2, m_length3 ;

	// TODO: add font flags and style flags
	enum Flags
	{
		fixed_pitch,
		serif,
		symbolic,
		script,
		nonsymbolic	= 5,
		italic,
		all_cap		= 16,
		small_cap,
		force_bold,
	} ;
	
	std::bitset<32>	m_flags ;

	std::vector<unsigned char>		m_font_file ;
} ;

} // end of namespace

#endif // FONTDESCRIPTOR_HH_
