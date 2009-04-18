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
	\file	ElementSrc.cc
	\brief	implementation the ElementSrc class
	\date	Sun Mar 30 2008
	\author	Nestal Wan
*/

#include "ElementSrc.hh"

#include "IElement.hh"
#include "IFile.hh"

#include "core/TraverseObject.hh"
#include "util/Util.hh"
#include "util/Backtrace.hh"

#include <boost/bind.hpp>
#include <boost/format.hpp>

#include <algorithm>

namespace pdf {

ElementSrc::ElementSrc( IFile *file )
	: m_file( file )
{
}

Object ElementSrc::ReadObj( const Ref& obj, bool deref )
{
	Object r = m_file->ReadObj( obj ) ;
	if ( deref )
		ForEachObj( r, boost::bind( &ElementSrc::Dereference, this, _1 ) );
	return r ;
}

void ElementSrc::Dereference( Object& obj )
{
	if ( obj.Type( ) == Object::ref )
		obj = ReadObj( obj, true ) ;
}

void ElementSrc::Store( IElement *element, const Ref& link )
{
	m_map.insert( std::make_pair( link, element ) ) ;
}

IElement* ElementSrc::Find( const Ref& link )
{
	Map::iterator i = m_map.find( link ) ;
	return i != m_map.end( ) ? i->second : 0 ;
}

Object& ElementSrc::DeRef( Object& obj )
{
	// de-reference it if it is a reference
	if ( obj.IsType<Ref>( ) )
		obj = ReadObj( obj ) ;

    return obj ;
}

} // end of namespace
