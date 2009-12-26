/***************************************************************************\
 *   Copyright (C) 2009 by Nestal Wan                                      *
 *   me@nestal.net                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
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

/*!
	\file	StandardFont.hh
	\brief	definition the StandardFont class
	\date	Sun Mar 8 2009
	\author	Nestal Wan
*/

#ifndef __PDF_STANDARD_FONT_HEADER_INCLUDED__
#define __PDF_STANDARD_FONT_HEADER_INCLUDED__

#include "BaseFont.hh"
#include "core/Name.hh"

namespace pdf {

/*!	\brief	The standard 14 Type 1 fonts.

	this class represents
*/
class StandardFont : public BaseFont
{
public :
	StandardFont( const Name& name ) ;

	std::string BaseName( ) const ;

	Ref Write( IFile *file ) const ;
	
	static bool IsStandardFont( const Name& font_name ) ;

private :
	Name	m_font_name ;
} ;

} // end of namespace

#endif
