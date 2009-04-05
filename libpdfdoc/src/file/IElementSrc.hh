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

#include "IElement.hh"
#include "ElementFactory.hh"

#include "core/Object.hh"
#include "core/Dictionary.hh"

#include <typeinfo>
#include <vector>

namespace pdf {

class Dictionary ;
class IElementSrc ;
class Name ;
class Ref ;

/*!	\brief	somewhere that can provide IElement's
	
	This class represents an abstraction on something that can Read() IElement
	objects. It provides the ReadObj() function to provide 
*/
class IElementSrc
{
protected :
	virtual ~IElementSrc( ) ;

public :
	/*!	\brief	reading objects
	*/
	virtual Object ReadObj( const Ref& obj, bool deref = false ) = 0 ;

private :
	// override only
	virtual void Store( IElement *element, const Ref& link ) = 0 ;
	virtual IElement* Find( const Ref& link ) = 0 ;

public :
	// defined in DeRef.hh
	template <typename T>
	T& DeRef( Object& obj ) ;

	template <typename T>
	bool Detach( Dictionary& dict, const Name& name, T& result ) ;

	template <class Element>
	Element* DetachElement( Dictionary& dict, const Name& name )
	{
		Dictionary::iterator i = dict.find( name ) ;
		if ( i == dict.end() )
			return 0 ;
		
		if ( i->second.IsType<Ref>() )
			return Read<Element>( i->second ) ;
		else
		{
			Element *element = CreateNewElement<Element>( i->second, this ) ;
			element->Init( i->second, this ) ;
			return element ;
		}
	}

	/*!	helper function to create elements. This function is the primary
		function for users of this class to read elements.
	*/
	template <class Element>
	Element* Read( const Ref& link )
	{
		IElement *temp = Find( link ) ;
		
		// element found in cache, try to re-use it
		if ( temp != 0 )
			// dynamic cast reference
			// it will throw bad_cast if failed
			return &dynamic_cast<Element&>( *temp ) ;

		else
		{
			Object obj = ReadObj( link ) ;
			Element *element = CreateNewElement<Element>( obj, this ) ;

			// insert before Read(). this is important!
			// there may be lookups for "link" inside Read(). if not insert
			// before Read(), these lookups will fail and re-create again,
			// causing infinite recursion.
			Store( element, link ) ;
			element->Init( obj, this ) ;
			return element ;
		}
	}
} ;

} // end of namespace

#endif
