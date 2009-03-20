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
	\file	BaseFont.hh
	\brief	definition the BaseFont class
	\date	Sun Mar 8 2009
	\author	Nestal Wan
*/

#ifndef __PDF_BASE_FONT_HEADER_INCLUDED__
#define __PDF_BASE_FONT_HEADER_INCLUDED__

#include "Font.hh"
#include "file/IElement.hh"
#include "file/IElementSrc.hh"

namespace pdf {

/*!	\brief	base class for all fonts
	\internal
	
	This class is the base class of all font classes in libpdfdoc.
*/
class BaseFont : public Font, public IElement
{
public :
	BaseFont( ) ;

	virtual std::string BaseName( ) const = 0 ;

	ElementList GetChildren( ) const ;
} ;

template <> BaseFont* CreateNewElement( const Ref& link, IElementSrc * ) ;

} // end of namespace

#endif
