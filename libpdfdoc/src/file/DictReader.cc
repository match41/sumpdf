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

#include "util/Debug.hh"
#include "util/Rect.hh"

#include <boost/bind.hpp>

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
	if ( obj.Is<Ref>() && file != 0 )
		m_dict = file->ReadObj( obj.As<Ref>() ).As<Dictionary>() ;
	else
		m_dict.swap( obj.As<Dictionary>() ) ;
}

void DictReader::Swap( DictReader& dict )
{
	m_dict.swap( dict.m_dict ) ;
	std::swap( m_file, dict.m_file ) ;
}

template <typename ObjType>
bool DictReader::Detach( const Name& name, ObjType& result )
{
	Dictionary::iterator i = m_dict.find( name ) ;
	if ( i != m_dict.end() )
	{
		SwapAt( i, result ) ;
		m_dict.erase( i ) ;
		return true ;
	}
	else
	{
		// the PDF specification indicates that if an object is not
		// found in a dictionary, it is treated as an null object.
		// so it is reasonable to reset an object to its default value,
		// which should have the same meaning as null object.
		result = ObjType() ;
		return false ;
	}
}

template bool DictReader::Detach( const Name&, Dictionary& ) ;
template bool DictReader::Detach( const Name&, Array& ) ;
template bool DictReader::Detach( const Name&, std::string& ) ;
template bool DictReader::Detach( const Name&, bool& ) ;
template bool DictReader::Detach( const Name&, Stream& ) ;
template bool DictReader::Detach( const Name&, Ref& ) ;
template bool DictReader::Detach( const Name&, Object& ) ;
template bool DictReader::Detach( const Name&, Name& ) ;
template bool DictReader::Detach( const Name&, int& ) ;
template bool DictReader::Detach( const Name&, double& ) ;
template bool DictReader::Detach( const Name&, DictReader& ) ;
template bool DictReader::Detach( const Name&, ArrayReader& ) ;
template bool DictReader::Detach( const Name&, Rect& ) ;
template bool DictReader::Detach( const Name&, std::size_t& ) ;

// uncomment that when needed
//template bool DictReader::Detach( const Name&, std::vector<Dictionary>& ) ;
//template bool DictReader::Detach( const Name&, std::vector<Array>& ) ;
template bool DictReader::Detach( const Name&, std::vector<std::string>& ) ;
//template bool DictReader::Detach( const Name&, std::vector<Stream>& ) ;
//template bool DictReader::Detach( const Name&, std::vector<Ref>& ) ;
//template bool DictReader::Detach( const Name&, std::vector<Object>& ) ;
//template bool DictReader::Detach( const Name&, std::vector<Name>& ) ;
//template bool DictReader::Detach( const Name&, std::vector<int>& ) ;
template bool DictReader::Detach( const Name&, std::vector<double>& ) ;
//template bool DictReader::Detach( const Name&, std::vector<DictReader>& ) ;
//template bool DictReader::Detach( const Name&, std::vector<ArrayReader>& ) ;
//template bool DictReader::Detach( const Name&, std::vector<Rect>& ) ;

template <typename T>
bool DictReader::SwapAt( Dictionary::iterator i, T& result )
{
	PDF_ASSERT( i != m_dict.end( ) ) ;
	
	Object& obj = i->second ;
	
	bool is_ref = obj.Is<Ref>() ; 
	if ( is_ref && m_file != 0 )
		m_file->ReadType( obj, result ) ;
	else
		std::swap( obj.As<T>(), result ) ;
	return is_ref ;
}

template bool DictReader::SwapAt( Dictionary::iterator, Dictionary& ) ;
template bool DictReader::SwapAt( Dictionary::iterator, Array& ) ;
template bool DictReader::SwapAt( Dictionary::iterator, std::string& ) ;
template bool DictReader::SwapAt( Dictionary::iterator, bool& ) ;
template bool DictReader::SwapAt( Dictionary::iterator, Stream& ) ;
template bool DictReader::SwapAt( Dictionary::iterator, Ref& ) ;
template bool DictReader::SwapAt( Dictionary::iterator, Object& ) ;
template bool DictReader::SwapAt( Dictionary::iterator, Name& ) ;

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
			result = ( i->second.Is<Ref>() && file != 0 ) ?
				file->ReadObj( i->second ).To<T>() :
				i->second.To<T>() ;
			return true ;
		}
		return false ;
	}
}

template <>
bool DictReader::SwapAt<int>( Dictionary::iterator i, int& result )
{
	return DetachTo( m_file, m_dict, i, result ) ;
}

template <>
bool DictReader::SwapAt<double>( Dictionary::iterator i, double& result )
{
	return DetachTo( m_file, m_dict, i, result ) ;
}

template <>
bool DictReader::SwapAt<std::size_t>( Dictionary::iterator i, std::size_t& result )
{
	return DetachTo( m_file, m_dict, i, result ) ;
}

template <>
bool DictReader::SwapAt<DictReader>( Dictionary::iterator i, DictReader& result )
{
	PDF_ASSERT( i != m_dict.end( ) ) ;
	
	Dictionary dict ;
	bool rtn = SwapAt( i, dict ) ;
	result.m_dict.swap( dict ) ;
	result.m_file = m_file ;
	return rtn ;
}

template <>
bool DictReader::SwapAt<ArrayReader>( Dictionary::iterator i, ArrayReader& result )
{
	PDF_ASSERT( i != m_dict.end( ) ) ;

	Array array ;
	bool rtn = SwapAt( i, array ) ;
	result->swap( array ) ;
	result.SetFile( m_file ) ;
	return rtn ;
}

template <>
bool DictReader::SwapAt<Rect>( Dictionary::iterator i, Rect& result )
{
	PDF_ASSERT( i != m_dict.end( ) ) ;

	Array array ;
	bool rtn = SwapAt( i, array ) ;
	result.Assign( array.begin(), array.end() ) ;
	return rtn ;
}

template <typename T>
bool DictReader::SwapAt( Dictionary::iterator i, std::vector<T>& result )
{
	PDF_ASSERT( i != m_dict.end( ) ) ;

	ArrayReader	array ;
	bool rtn = SwapAt( i, array ) ;
	
	result.resize( array->size() ) ;
	for ( std::size_t i = 0 ; i < array->size() ; ++i )
		array.Detach( i, result[i] ) ;
	
	return rtn ;
}

template bool DictReader::SwapAt( Dictionary::iterator, std::vector<double>& ) ;
template bool DictReader::SwapAt( Dictionary::iterator, std::vector<int>& ) ;
template bool DictReader::SwapAt( Dictionary::iterator, std::vector<Name>& ) ;
template bool DictReader::SwapAt(
	Dictionary::iterator,
	std::vector<Dictionary>& ) ;
template bool DictReader::SwapAt( Dictionary::iterator, std::vector<Stream>& ) ;
template bool DictReader::SwapAt(
	Dictionary::iterator,
	std::vector<std::string>& );
template bool DictReader::SwapAt( Dictionary::iterator, std::vector<Ref>& ) ;
template bool DictReader::SwapAt( Dictionary::iterator, std::vector<Object>& ) ;
template bool DictReader::SwapAt(
	Dictionary::iterator,
	std::vector<DictReader>& ) ;

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

Dictionary& DictReader::Get()
{
	return m_dict ;
}

const Dictionary& DictReader::Get() const
{
	return m_dict ;
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
