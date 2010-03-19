/***************************************************************************\
 *   Copyright (C) 2006 by Nestal Wan                                      *
 *   me@nestal.net                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2.                              *
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

/**	\file	MockResources.cc
	\brief	implementation of the MockResources class
	\date	Jan 23, 2010
	\author	Nestal Wan
*/

#include "MockResources.hh"

#include "font/BaseFont.hh"

namespace pdf {

/**	constructor
*/
MockResources::MockResources( )
{
}

Name MockResources::AddFont( BaseFont *font )
{
	Name name( font->BaseName( ) ) ;
	m_font_map[name] = font ;
	return name ;
}

void MockResources::Add( const Name& name, BaseFont *font )
{
	m_font_map[name] = font ;
}

BaseFont* MockResources::FindFont( const Name& name ) const
{
	std::map<Name,BaseFont*>::const_iterator i = m_font_map.find( name ) ;
	return i != m_font_map.end() ? i->second : 0 ;
}

Name MockResources::FindFont( const BaseFont *font ) const
{
	return Name( font->BaseName( ) ) ;
}

} // end of namespace
