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

#include <string>

#include "util/Rect.hh"
#include "stream/Stream.hh"

namespace pdf {

class Dictionary ;
class IFile ;

///	A font descriptor
/**	The FontDescriptor class represents a font descriptor in a PDF file. It
	contains information about a font face. It is used to guess the appearance
	of a font without knowing its font program.
*/
class FontDescriptor
{
public :
	enum Stretch
	{
		ultra_condensed, extra_condensed, condensed, semi_condensed,
		normal,
		semi_expanded, expanded, extra_expanded, ultra_expanded,
		unknown
	} ;

public :
	FontDescriptor( ) ;
	
	void Read( Dictionary& self, IFile *file ) ;
	
	std::string Family( ) const ;

	Stretch GetStretch( ) const ;

	double	ItalicAngle( ) const ;

	const std::vector<unsigned char>&	FontFile( ) const ;

private :
	static const Name m_stretch_names[] ; 

private :
	std::string	m_family ;
	Stretch		m_stretch ;
	int			m_weight ;
	int			m_flags ;
	Rect		m_bbox ;
	double		m_italic_angle ;
	double		m_ascent, m_descent, m_leading, m_cap_height, m_x_height,
				m_stemv, m_stemh, m_avg_width, m_max_width, m_miss_width ;
	
	std::vector<unsigned char>		m_font_file ;
} ;

} // end of namespace

#endif // FONTDESCRIPTOR_HH_
