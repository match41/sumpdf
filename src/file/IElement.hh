/***************************************************************************
 *   Copyright (C) 2006 by Nestal Wan                                      *
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
 ***************************************************************************/

/*!
	\file	IElement.hh
	\brief	definition the IElement interface
	\date	Fri Mar 21 2008
	\author	Nestal Wan
*/

#ifndef __PDF_IELEMENT_HEADER_INCLUDED__
#define __PDF_IELEMENT_HEADER_INCLUDED__

#include <vector>

namespace pdf {

class Ref ;
class IElementSrc ;
class IElementDest ;
class ElementList ;

/*!	\brief	PDF file element.
	
	This class represents an element of a PDF file. A so-called "element" is a
	PDF object that is sufficiently self-contain and represent a logical unit
	of a PDF file. Examples of element include pages, fonts, XObjects and 
	multimedia objects. Another features of element is that they can be referred
	by each others using references, i.e. indirect objects. It allows easy
	sharing and save memory and disk space.
*/
class IElement
{
public :
	virtual ~IElement( ) ;
	
	//!	Read the element from disk. It will also read all children from the
	//! source file.
	virtual void Read( const Ref& link, IElementSrc *src ) = 0 ;
	virtual void Write( const Ref& link, IElementDest *dest ) const = 0 ;
	
	virtual ElementList GetChildren( ) const = 0 ;
} ;

} // end of namespace

#endif
