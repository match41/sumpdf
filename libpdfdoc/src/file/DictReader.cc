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

/**	\file	DictReader.cc
	\brief	implementation of the DictReader class
	\date	Feb 7, 2010
	\author	Nestal Wan
*/

#include "DictReader.hh"

#include "stream/Stream.hh"
#include "core/Array.hh"

namespace pdf {

DictReader::DictReader( )
	: m_file( 0 )
{
}

/**	constructor
	
*/
DictReader::DictReader( Dictionary& dict, File *file )
	: m_file( file )
{
	// grand theft auto
	m_dict.swap( dict ) ;
}

DictReader::DictReader( Object& obj, File *file )
	: m_file( file )
{
	if ( obj.Is<Ref>() )
		m_dict = file->ReadObj( obj.As<Ref>() ).As<Dictionary>() ;
	else
		m_dict.swap( obj.As<Dictionary>() ) ;
}

template <typename ObjType>
bool DictReader::Detach( const Name& name, ObjType& result )
{
	Dictionary::iterator i = m_dict.find( name ) ;
	if ( i != m_dict.end( ) )
	{
		if ( i->second.Is<Ref>() )
			result = m_file->ReadObj( i->second ).As<ObjType>() ;
		else
			std::swap( i->second.As<ObjType>(), result ) ;

		m_dict.erase( i ) ;
		return true ;
	}
	return false ;
}

template bool DictReader::Detach( const Name& name, Dictionary& result ) ;
template bool DictReader::Detach( const Name& name, Array& result ) ;
template bool DictReader::Detach( const Name& name, Name& result ) ;
template bool DictReader::Detach( const Name& name, std::string& result ) ;
template bool DictReader::Detach( const Name& name, bool& result ) ;
template bool DictReader::Detach( const Name& name, Stream& result ) ;
template bool DictReader::Detach( const Name& name, Ref& result ) ;
template bool DictReader::Detach( const Name& name, Object& result ) ;

namespace
{
	template <typename ObjType>
	bool DetachTo( File *file, Dictionary& dict, const Name& name, ObjType& result )
	{
		Dictionary::iterator i = dict.find( name ) ;
		if ( i != dict.end( ) )
		{
			result = ( i->second.Is<Ref>() ) ?
				file->ReadObj( i->second ).To<ObjType>() :
				i->second.To<ObjType>() ;
		
			dict.erase( i ) ;
			return true ;
		}
		return false ;
	}
}

template <>
bool DictReader::Detach<int>( const Name& name, int& result )
{
	return DetachTo( m_file, m_dict, name, result ) ;
}

template <>
bool DictReader::Detach<double>( const Name& name, double& result )
{
	return DetachTo( m_file, m_dict, name, result ) ;
}

template <>
bool DictReader::Detach<DictReader>( const Name& name, DictReader& result )
{
	Dictionary dict ;
	if ( Detach( name, dict ) )
	{
		result.m_dict.swap( dict ) ;
		result.m_file = m_file ;
		return true ;
	}
	else
		return false ;
}

Dictionary* DictReader::operator->()
{
	return &m_dict ;
}

const Dictionary* DictReader::operator->() const
{
	return &m_dict ;
}

Dictionary& DictReader::operator*()
{
	return m_dict ;
}

const Dictionary& DictReader::operator*() const
{
	return m_dict ;
}

Object& DictReader::operator[]( const Name& name )
{
	return m_dict[name] ;
}

const Object& DictReader::operator[]( const Name& name ) const
{
	return m_dict[name] ;
}

} // end of namespace
