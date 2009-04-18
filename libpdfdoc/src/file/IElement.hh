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

namespace pdf {

class Ref ;
// class IElementSrc ;
class IElementDest ;
class ElementList ;
class ElementReader ;
class Object ;

/*!	\brief	PDF file element.
	\internal
	
	This class represents an element of a PDF file. A so-called "element" is a
	PDF object that is sufficiently self-contain and represent a logical unit
	of a PDF file. Examples of element include pages, fonts, XObjects and 
	multimedia objects. Another features of element is that they can be referred
	by each others using references, i.e. indirect objects. It allows easy
	sharing and save memory and disk space.
	
	An element may contain child elements.
*/
class IElement
{
public :
	virtual ~IElement( ) ;
	
// 	virtual void Init( Object& obj, IElementSrc *src ) = 0 ;
	virtual void Init( Object& obj, ElementReader *src ) = 0 ;
	
	/*!	\brief	read an element
		\internal
	
		Read the element from disk. It will also read all children from the
		source file.
		\param	link	reference to the element
		\param	src		a IElementSrc for the element to read its child
						elements
	*/
// 	virtual void Read( const Ref& link, IElementSrc *src ) = 0 ;
	
	/*!	\brief	write an element
		\internal
	
		Write the element to disk. It will also write all children to the
		file if they are not yet written.
		\param	link	destination link to write to
		\param	dest	an IElementDest for it to write its child also
	*/
	virtual void Write( const Ref& link, IElementDest *dest ) const = 0 ;
	
	/*!	\brief	get the list of children element
		\internal
	*/
	virtual ElementList GetChildren( ) const = 0 ;
} ;

} // end of namespace

#endif
