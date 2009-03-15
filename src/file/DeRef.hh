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
	\brief	definition the DeRef class
	\date	Sun Apr 6 2008
	\author	Nestal Wan
*/

#ifndef __PDF_DEREF_HEADER_INCLUDED__
#define __PDF_DEREF_HEADER_INCLUDED__

#include "IElementSrc.hh"

#include "core/Dictionary.hh"
#include "core/Object.hh"

namespace pdf {

/*!	\brief	brief description
	
	this class represents
*/
template <typename T>
T IElementSrc::DeRef( const Object& obj )
{
	return obj.Type( ) == Object::ref ? ReadObj( obj ).As<T>( )
									  : obj.As<T>( ) ;
}

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
