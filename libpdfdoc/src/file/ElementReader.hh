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
	\file	ElementReader.hh
	\brief	definition the ElementReader class
	\date	Sat Apr 18 2009
	\author	Nestal Wan
*/

#ifndef __PDF_ELEMENT_READER_INCLUDED__
#define __PDF_ELEMENT_READER_INCLUDED__

#include "core/Ref.hh"
#include "core/Object.hh"
#include "core/Dictionary.hh"

#include "ElementFactory.hh"

#include <map>

namespace pdf {

class IFile ;
class IElement ;

/*!	\brief	brief description
	
	this class represents
*/
class ElementReader
{
public :
	ElementReader( IFile *file ) ;

	IElement* Find( const Ref& link ) ;
    Object& DeRef( Object& obj ) ;
	
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

			// store before Read(). this is important!
			// there may be lookups for "link" inside Read(). if not insert
			// before Read(), these lookups will fail and re-create again,
			// causing infinite recursion.
			Store( element, link ) ;
			InitElement( element, obj ) ;
			
			return element ;
		}
	}

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
			InitElement( element, i->second ) ;
			dict.erase( i ) ;
			
			return element ;
		}
	}

	/*!	\brief	detach and de-reference object in dictionary
	
		This function looks up the \a name in \a dict , and put it in
		\a result. If \a result is a Ref, it will de-reference it and put the
		de-reference'd object in \a result instead. It will then be erased from
		the dictionary.	This function will use std::swap() on the objects to
		reduce deep	copying.
		
		\param	dict	the dictionary to look up and detach
		\param	name	the key to the value in \a dict to detach
		\param	result	reference to output
		\return	true if the \a name is found in \a dict . otherwise false.
		\throw	BadType	if \a name is found but its type is not \a T
		
		\sa		Dictionary::Extract(), Object::Swap(), ReadObj(), DeRef()
		\note	This template function is defined in DeRef.hh. Callers must
				include this header when user or else a link error will be
				generated.
	*/
	template <typename T>
	bool Detach( Dictionary& dict, const Name& name, T& result )
	{
		Dictionary::iterator i = dict.find( name ) ;
		if ( i != dict.end( ) )
		{
			if ( i->second.Type( ) == Object::ref )
				result = ReadObj( i->second ) ;
			else
				std::swap( i->second.As<T>(), result ) ;
			
			dict.erase( i ) ;
			return true ;
		}
		return false ;
	}

private :
	Object ReadObj( const Ref& obj ) ;
	
	void Store( IElement *element, const Ref& link ) ;
	void InitElement( IElement *element, Object& obj ) ;

private :
	typedef std::map<Ref, IElement*> Map ;
	Map	m_map ;
	
	IFile *m_file ;
} ;

} // end of namespace

#endif
