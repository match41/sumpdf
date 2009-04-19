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
	\file	ElementReader.cc
	\brief	implementation the ElementReader class
	\date	Sat Apr 18 2009
	\author	Nestal Wan
*/

#include "ElementReader.hh"

#include "IFile.hh"
#include "IElement.hh"

#include "RawElement.hh"

namespace pdf {

ElementReader::ElementReader( IFile *file )
	: m_file( file )
{
}

void ElementReader::Store( IElement *element, const Ref& link )
{
	m_map.insert( std::make_pair( link, element ) ) ;
}

IElement* ElementReader::Find( const Ref& link )
{
	Map::iterator i = m_map.find( link ) ;
	return i != m_map.end( ) ? i->second : 0 ;
}

Object ElementReader::ReadObj( const Ref& link )
{
	return m_file->ReadObj( link ) ;
}

void ElementReader::InitElement( IElement *element, Object& obj )
{
	element->Init( obj, this ) ;
}

Object& ElementReader::DeRef( Object& obj )
{
	if ( obj.IsType<Ref>( ) )
		obj = m_file->ReadObj( obj ) ;
	return obj ;
}

/*!	general read element function. This function can be used instead of
	its templated brother when the caller does not need to know the exact
	type of element returned. It will first look up its cache to find an
	existing one, and return it if found. Otheriwse, it will create a
	RawElement, add it to the cache and returns it.
*/
IElement* ElementReader::Read( const Ref& link )
{
	IElement *temp = Find( link ) ;
	
	return temp != 0 ? temp : NewElement<RawElement>( link ) ;
}

} // end of namespace
