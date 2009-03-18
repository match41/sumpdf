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
	\file	IElementSrc.hh
	\brief	definition the IElementSrc class
	\date	Sat Mar 22 2008
	\author	Nestal Wan
*/

#ifndef __PDF_IDOC_BODY_HEADER_INCLUDED__
#define __PDF_IDOC_BODY_HEADER_INCLUDED__

#include <typeinfo>
#include <vector>

namespace pdf {

class Dictionary ;
class IElement ;
class Name ;
class Ref ;
class Object ;

template <class Element>
Element* CreateNewElement( const Ref& )
{
	return new Element ;
}

/*!	\brief	brief description
	
	this class represents
*/
class IElementSrc
{
protected :
	virtual ~IElementSrc( ) ;

public :
	virtual Object ReadObj( const Ref& obj, bool deref = false ) = 0 ;

private :
	// override only
	virtual IElement* Init( IElement *element, const Ref& link ) = 0 ;
	virtual IElement* Find( const Ref& link ) = 0 ;

public :
	// define elsewhere
	template <typename T>
	T DeRef( const Object& obj ) ;

	template <typename T>
	bool Detach( Dictionary& dict, const Name& name, T& result ) ;

	// helper function to create objects
	template <class Element>
	Element* Read( const Ref& link )
	{
		// dynamic cast reference
		// it will throw bad_cast if failed
		IElement *temp = Find( link ) ;
		return temp != 0 ? &dynamic_cast<Element&>( *temp )
		                 : NewElement<Element>( link ) ;
	}

private :
	template <class Element>
	Element* NewElement( const Ref& link )
	{
		Element *element = CreateNewElement<Element>( link ) ;
		Init( element, link ) ;
		return element ;
	}
} ;

} // end of namespace

#endif
