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

///	brief description
/**	The FontDescriptor class represents
*/
class FontDescriptor
{
public :
	enum Stretch
	{
		ultra_condensed, extra_condensed, condensed, semi_condensed,
		normal,
		semi_expanded, expanded, extra_expanded, ultra_expanded
	} ;

public :
	FontDescriptor( ) ;
	
	void Read( Dictionary& self, IFile *file ) ;
	
	std::string Family( ) const ;

	Stretch GetStretch( ) const ;

	double	ItalicAngle( ) const ;

	Stream	FontFile( ) const ;

private :
	std::string	m_family ;
	Stretch		m_stretch ;
	int			m_flags ;
	Rect		m_bbox ;
	double		m_italic_angle ;
	double		m_ascent, m_descent, m_leading ;
	Stream		m_font_file ;
} ;

} // end of namespace

#endif // FONTDESCRIPTOR_HH_
