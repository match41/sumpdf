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

#include "ArrayReader.hh"
#include "File.hh"

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

void DictReader::Swap( DictReader& dict )
{
	m_dict.swap( dict.m_dict ) ;
	std::swap( m_file, dict.m_file ) ;
}

template <typename T>
bool DictReader::Detach( Dictionary::iterator i, T& result )
{
	if ( i != m_dict.end( ) )
	{
		Object& obj = i->second ;
		if ( obj.Is<Ref>() )
			result = m_file->ReadObj( obj ).As<T>() ;
		else
			std::swap( obj.As<T>(), result ) ;

		m_dict.erase( i ) ;
		return true ;
	}
	return false ;
}

template bool DictReader::Detach( Dictionary::iterator, Dictionary& ) ;
template bool DictReader::Detach( Dictionary::iterator, Array& ) ;
template bool DictReader::Detach( Dictionary::iterator, Name& ) ;
template bool DictReader::Detach( Dictionary::iterator, std::string& ) ;
template bool DictReader::Detach( Dictionary::iterator, bool& ) ;
template bool DictReader::Detach( Dictionary::iterator, Stream& ) ;
template bool DictReader::Detach( Dictionary::iterator, Ref& ) ;
template bool DictReader::Detach( Dictionary::iterator, Object& ) ;

namespace
{
	template <typename T>
	bool DetachTo(
		File 					*file,
		Dictionary&				dict,
		Dictionary::iterator	i,
		T&						result )
	{
		if ( i != dict.end( ) )
		{
			result = ( i->second.Is<Ref>() ) ?
				file->ReadObj( i->second ).To<T>() :
				i->second.To<T>() ;
		
			dict.erase( i ) ;
			return true ;
		}
		return false ;
	}
}

template <>
bool DictReader::Detach<int>( Dictionary::iterator i, int& result )
{
	return DetachTo( m_file, m_dict, i, result ) ;
}

template <>
bool DictReader::Detach<double>( Dictionary::iterator i, double& result )
{
	return DetachTo( m_file, m_dict, i, result ) ;
}

template <>
bool DictReader::Detach<DictReader>(
	Dictionary::iterator	i,
	DictReader& 			result )
{
	Dictionary dict ;
	if ( Detach( i, dict ) )
	{
		result.m_dict.swap( dict ) ;
		result.m_file = m_file ;
		return true ;
	}
	else
		return false ;
}

template <>
bool DictReader::Detach<ArrayReader>(
	Dictionary::iterator	i,
	ArrayReader&			result)
{
	Array array ;
	if ( Detach( i, array ) )
	{
		result->swap( array ) ;
		result.SetFile( m_file ) ;
		return true ;
	}
	else
		return false ;
}

template <>
DictReader DictReader::DeRefObj( const Object& obj )
{
	Dictionary dict = DeRefObj<Dictionary>( obj ) ;
	return DictReader( dict, m_file ) ;
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

File* DictReader::GetFile( ) const
{
	return m_file ;
}

void DictReader::SetFile( File *file )
{
	m_file = file ;
}

} // end of namespace
