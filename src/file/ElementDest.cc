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
	\file	ElementDest.cc
	\brief	implementation the ElementDest class
	\date	Sun Apr 13 2008
	\author	Nestal Wan
*/

#include "ElementDest.hh"

#include "IFile.hh"
#include "IElement.hh"

#include "core/Dictionary.hh"
#include "core/Array.hh"
#include "core/TraverseObject.hh"

#include <boost/bind.hpp>

#include <algorithm>
#include <cassert>

namespace pdf {

ElementDest::ElementDest( IFile *file )
	: m_file( file )
{
	assert( m_file != 0 ) ;
}

Ref ElementDest::Write( const IElement *element )
{
	assert( element != 0 ) ;

	Map::iterator i = m_map.find( element ) ;
	if ( i == m_map.end( ) )
	{
		Ref link = m_file->AllocLink( ) ;
		i = m_map.insert( std::make_pair( element, link ) ).first ;
		
		element->Write( link, this ) ;
	}
	return i->second ;
}

Ref ElementDest::WriteObj( const Object& obj )
{
	// since we need to make a copy of the object, we only do so for dictionary
	// and arrays
	if ( obj.Type( ) == Object::dictionary || obj.Type( ) == Object::array )
	{
		// we can't modify the original object. we can only make a copy.
		Object copy = obj ;
		ForEachObj( copy, boost::bind( &ElementDest::Dereference, this, _1 ) ) ;
		
		return m_file->WriteObj( copy ) ;
	}
	else
		return m_file->WriteObj( obj ) ;
}

void ElementDest::Dereference( Object& obj )
{
	// special handling for stream objects.
	// stream objects must be indirect objects. so if we encounter them, we
	// need to write them separately and keep only their reference.
	if ( obj.Type( ) == Object::stream )
		obj = m_file->WriteObj( obj ) ;
}

Ref ElementDest::AllocLink( )
{
	return m_file->AllocLink( ) ;
}

void ElementDest::WriteObj( const Object& obj, const Ref& link )
{
	if ( obj.Type( ) == Object::dictionary || obj.Type( ) == Object::array )
	{
		Object copy = obj ;
		ForEachObj( copy, boost::bind( &ElementDest::Dereference, this, _1 ) ) ;
		return m_file->WriteObj( copy, link ) ;
	}
	else
		return m_file->WriteObj( obj, link ) ;
}

} // end of namespace
