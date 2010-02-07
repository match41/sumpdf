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

/**	\file	ArrayReader.cc
	\brief	implementation of the ArrayReader class
	\date	Feb 7, 2010
	\author	Nestal Wan
*/

#include "ArrayReader.hh"

#include "DictReader.hh"

#include "stream/Stream.hh"

namespace pdf {

/**	constructor
	
*/
ArrayReader::ArrayReader( )
	: m_file( 0 )
{
}

ArrayReader::ArrayReader( Array& array, File *file )
	: m_file( file )
{
	m_array.swap( array ) ;
}

ArrayReader::ArrayReader( Object& obj, File *file )
	: m_file( file )
{
	if ( obj.Is<Ref>() )
		m_array = file->ReadObj( obj.As<Ref>() ).As<Array>() ;
	else
		m_array.swap( obj.As<Array>() ) ;
}

template <typename T>
bool ArrayReader::Detach( std::size_t idx, T& result )
{
	if ( idx < m_array.size() )
	{
		Object& obj = m_array[idx] ;
		if ( obj.Is<Ref>() )
			result = m_file->ReadObj( obj.As<Ref>() ).As<T>() ;
		else
			std::swap( result, obj.As<T>() ) ;
		return true ;
	}
	else
		return false ;
}

template bool ArrayReader::Detach( std::size_t idx, Dictionary& result ) ;
template bool ArrayReader::Detach( std::size_t idx, Array& result ) ;
template bool ArrayReader::Detach( std::size_t idx, Name& result ) ;
template bool ArrayReader::Detach( std::size_t idx, std::string& result ) ;
template bool ArrayReader::Detach( std::size_t idx, bool& result ) ;
template bool ArrayReader::Detach( std::size_t idx, Stream& result ) ;
template bool ArrayReader::Detach( std::size_t idx, Ref& result ) ;
template bool ArrayReader::Detach( std::size_t idx, Object& result ) ;

namespace
{
	// like Detach(), but use To() instead of As()
	template <typename T>
	bool DetachTo( File *file, Array& array, std::size_t idx, T& result )
	{
		if ( idx < array.size() )
		{
			result = ( array[idx].Is<Ref>() )
				? file->ReadObj( array[idx] ).To<T>()
				: array[idx].To<T>() ;
			
			return true ;
		}
		return false ;
	}
}

template <>
bool ArrayReader::Detach<int>( std::size_t idx, int& result )
{
	return DetachTo( m_file, m_array, idx, result ) ;
}

template <>
bool ArrayReader::Detach<double>( std::size_t idx, double& result )
{
	return DetachTo( m_file, m_array, idx, result ) ;
}

template <>
bool ArrayReader::Detach<DictReader>( std::size_t idx, DictReader& result )
{
	Dictionary dict ;
	if ( Detach( idx, dict ) )
	{
		result->swap( dict ) ;
		result.SetFile( m_file ) ;
		return true ;
	}
	else
		return false ;
}

template <>
bool ArrayReader::Detach<ArrayReader>( std::size_t idx, ArrayReader& result )
{
	Array array ;
	if ( Detach( idx, array ) )
	{
		result.m_array.swap( array ) ;
		result.m_file = m_file ;
		return true ;
	}
	else
		return false ;
}

Array* ArrayReader::operator->()
{
	return &m_array ;
}

const Array* ArrayReader::operator->() const
{
	return &m_array ;
}

Array& ArrayReader::operator*()
{
	return m_array ;
}

const Array& ArrayReader::operator*() const
{
	return m_array ;
}

Object& ArrayReader::operator[]( std::size_t idx )
{
	return m_array[idx] ;
}

const Object& ArrayReader::operator[]( std::size_t idx ) const
{
	return m_array[idx] ;
}

File* ArrayReader::GetFile( ) const
{
	return m_file ;
}

void ArrayReader::SetFile( File *file )
{
	m_file = file ;
}

} // end of namespace
