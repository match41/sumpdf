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
	\file	TraverseObject.hh
	\brief	definition the TraverseObject class
	\date	Tue Apr 15 2008
	\author	Nestal Wan
*/

#ifndef __PDF_TRAVERSE_OBJECT_HEADER_INCLUDED__
#define __PDF_TRAVERSE_OBJECT_HEADER_INCLUDED__

#include "Array.hh"
#include "Dictionary.hh"
#include "Object.hh"

#include <algorithm>
#include <boost/bind.hpp>

#include <iostream>

namespace pdf {

template <typename F>
void ForEachObj( Object& obj, F func )
{
	if ( obj.Type( ) == Object::dictionary )
	{
		Dictionary& dict = obj.As<Dictionary>( ) ;
		for ( Dictionary::iterator i  = dict.begin( ) ;
								   i != dict.end( ) ; ++i )
		{
			ForEachObj( i->second, func ) ;
		}
	}
	
	else if ( obj.Type( ) == Object::array )
	{
		Array& array = obj.As<Array>( ) ;
		for ( Array::iterator i  = array.begin( ) ;
							  i != array.end( ) ; ++i )
			ForEachObj( *i, func ) ;
	}
	else
		func( obj ) ;
}

template <typename F>
void ForEachObj( const Object& obj, F func )
{
	if ( obj.Type( ) == Object::dictionary )
	{
		const Dictionary& dict = obj.As<Dictionary>( ) ;
		for ( Dictionary::const_iterator i  = dict.begin( ) ;
								         i != dict.end( ) ; ++i )
		{
			ForEachObj( i->second, func ) ;
		}
	}
	
	else if ( obj.Type( ) == Object::array )
	{
		const Array& array = obj.As<Array>( ) ;
		for ( Array::const_iterator i  = array.begin( ) ;
							        i != array.end( ) ; ++i )
			ForEachObj( *i, func ) ;
	}
	else
		func( obj ) ;
}

} // end of namespace

#endif
