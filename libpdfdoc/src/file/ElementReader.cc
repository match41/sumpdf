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

namespace pdf {

ElementReader::ElementReader( IFile *file )
	: m_file( file )
{
}

void ElementReader::Store( IElement *element, Object& obj, const Ref& link )
{
	// insert before Read(). this is important!
	// there may be lookups for "link" inside Read(). if not insert
	// before Read(), these lookups will fail and re-create again,
	// causing infinite recursion.
	m_map.insert( std::make_pair( link, element ) ) ;
	element->Init( obj, this ) ;
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

} // end of namespace
