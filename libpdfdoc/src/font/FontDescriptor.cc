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

#include "util/Util.hh"
#include "util/Debug.hh"

namespace pdf {

const Name FontDescriptor::m_stretch_names[] =
{
	"UltraCondensed",
	"ExtraCondensed",
	"Condensed",
	"SemiCondensed",
	"Normal",
	"SemiExpanded",
	"Expanded",
	"ExtraExpanded",
	"UltraExpanded"
} ;

/**	constructor
*/
FontDescriptor::FontDescriptor( )
	: m_flags( 0 )
{
	m_italic_angle = m_ascent = m_descent = m_leading = 0.0 ;
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
	
	// optional font family name. normally empty for embedded font
	Detach( file, self, "FontFamily",	m_family ) ;
	
	Name stretch ;
	if ( Detach( file, self, "FontStretch", stretch ) )
		m_stretch = static_cast<Stretch>(std::find(
			Begin(m_stretch_names),
			End(m_stretch_names),
			stretch ) - Begin(m_stretch_names) ) ;

	Detach( file, self, "FontWeight",	m_weight ) ;
	Detach( file, self, "Flags",		m_flags ) ;
	
	Array bbox ;
	if ( Detach( file, self, "FontBBox", bbox ) )
		m_bbox = Rect( bbox.begin(), bbox.end() ) ;

	Detach( file, self, "ItalicAngle",	m_italic_angle ) ;
	Detach( file, self, "Ascent",		m_ascent ) ;
	Detach( file, self, "Descent",		m_descent ) ;
	Detach( file, self, "Leading",		m_leading ) ;
	Detach( file, self, "CapHeight",	m_cap_height ) ;
	Detach( file, self, "XHeight",		m_x_height ) ;
	Detach( file, self, "StemV",		m_stemv ) ;
	Detach( file, self, "StemH",		m_stemh ) ;
	Detach( file, self, "AvgWidth",		m_avg_width ) ;
	Detach( file, self, "MaxWidth",		m_max_width ) ;
	Detach( file, self, "MissingWidth",	m_miss_width ) ;
}

Stream FontDescriptor::FontFile( ) const
{
	return m_font_file ;
}

std::string FontDescriptor::Family( ) const
{
	return m_family ;
}

} // end of namespace
