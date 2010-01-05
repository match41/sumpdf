/***************************************************************************
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
 ***************************************************************************/

/*!
	\file	Name.cc
	\brief	implementation the Name class
	\date	Thu Mar 13 2008
	\author	Nestal Wan
*/

#include "Name.hh"
#include "Object.hh"
#include "Token.hh"
#include "TokenSrc.hh"
#include "util/Exception.hh"

#include <iostream>

namespace pdf {

Name::Name( const char *name )
	: m_name( name )
{
}

Name::Name( const std::string& name )
	: m_name( name )
{
}

Name::Name( const Object& obj )
	: m_name( obj.Type() == Object::null ? std::string( )
	                                     : obj.As<Name>().m_name )
{
}

/**	\brief	Swapping two names

	Swapping two Name objects with no-throw exception guarantee.
	\param	name	another Name object to be swapped
*/
void Name::swap( Name& name )
{
	m_name.swap( name.m_name ) ;
}

std::istream& operator>>( std::istream& is, Name& name )
{
	TokenSrc src( is ) ;
	return (src >> name).Stream() ;
}

TokenSrc& operator>>( TokenSrc& src, Name& name )
{
	// search for
	Token bslash, name_text ;
	if ( src >> bslash >> name_text && bslash.Get() == "/" )
		name.m_name = name_text.Get( ) ;
	
	return src ;
}

std::ostream& operator<<( std::ostream& os, const Name& name )
{
	if ( name.m_name.empty( ) )
		throw ParseError( "cannot write empty name" ) ;

	return os << '/' << name.m_name ;
}

bool operator<( const Name& name1, const Name& name2 )
{
	return name1.m_name < name2.m_name ;
}

bool operator==( const Name& name1, const Name& name2 )
{
	return name1.m_name == name2.m_name ;
}

bool operator!=( const Name& name1, const Name& name2 )
{
	return name1.m_name != name2.m_name ;
}

bool operator==( const Name& name1, const char *name2 )
{
	return name1 == Name( name2 ) ;
}

bool operator!=( const Name& name1, const char *name2 )
{
	return name1 != Name( name2 ) ;
}

///	Returns the underlying string.
const std::string& Name::Str() const
{
	return m_name ;
}

bool Name::empty( ) const
{
	return m_name.empty( ) ;
}

} // end of namespace

namespace std
{
	void swap( pdf::Name& dict1, pdf::Name& dict2 )
	{
		dict1.swap( dict2 ) ;
	}
}
