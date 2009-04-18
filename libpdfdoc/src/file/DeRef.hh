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
	\file	DeRef.hh
	\brief	Definition of the de-referencing functions for IElementSrc
	\date	Sun Apr 6 2008
	\author	Nestal Wan
*/

#ifndef __PDF_DEREF_HEADER_INCLUDED__
#define __PDF_DEREF_HEADER_INCLUDED__

#include "IElementSrc.hh"

#include "core/Dictionary.hh"
#include "core/Object.hh"

namespace pdf {

/*!	\brief	de-reference objects

	If \a obj is a Ref, de-reference it, i.e look-up the object it is
	referring to by caller IElementSrc::ReadObj() and replace it. In any case,
	a reference to \a T is returned.
	
	\param	obj	object to be de-referenced. If it is a Ref, it will be replaced
				by the object it is referring to.
	\return	a reference to the de-reference'd \a T
	\throw	BadType	if \a obj or the object it refers to is not of type \a T
	\sa	Object::As(), ReadObj(), Detach()
*/
/*template <typename T>
T& IElementSrc::DeRef( Object& obj )
{
	// de-reference it if it is a reference
	if ( obj.Type( ) == Object::ref )
		obj = ReadObj( obj ) ;
	
	return obj.As<T>( ) ;
}
*/
/*!	\brief	detach and de-reference object in dictionary

	This function looks up the \a name in \a dict , and put it in \a result .
	If \a result is a Ref, it will de-reference it and put the de-reference'd
	object in \a result instead. It will then be erased from the dictionary.
	This function will use std::swap() on the objects to reduce deep copying.
	
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
bool IElementSrc::Detach( Dictionary& dict, const Name& name, T& result )
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

} // end of namespace

#endif
