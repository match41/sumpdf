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

#include "core/Dictionary.hh"
#include "file/IFile.hh"
#include "file/ObjectReader.hh"

namespace pdf {

/**	constructor
*/
FontDescriptor::FontDescriptor( )
	: m_flags( 0 )
{
	m_italic_angle = m_ascent = m_descent = m_leading = 0.0 ;
}

void FontDescriptor::Read( Dictionary& self, IFile *file )
{
	if ( !Detach( file, self, "FontFile", 	m_font_file ) )
		if ( !Detach( file, self, "FontFile2", 	m_font_file ) )
			Detach( file, self, "FontFile3", 	m_font_file ) ;
	
	Detach( file, self, "FontFamily",	m_family ) ;
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
