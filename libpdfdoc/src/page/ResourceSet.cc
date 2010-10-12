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

/**	\file	ResourceSet.cc
	\brief	implementation of the ResourcesSet class
	\date	May 21, 2010
	\author	Nestal Wan
*/

#include "ResourceSet.hh"

#include "file/DictReader.hh"

#include "util/Debug.hh"

#include <boost/bind.hpp>

#include <sstream>

namespace pdf {

/**	constructor
	
*/
template <typename T>
ResourceSet<T>::ResourceSet( const std::string& prefix )
	: m_prefix( prefix )
{
}

template <typename T>
ResourceSet<T>::~ResourceSet( )
{
	Clear( ) ;
}

template <typename T>
void ResourceSet<T>::Clear( )
{
	using namespace boost ;
	std::for_each( m_map.left.begin(), m_map.left.end(),
		bind( &T::Release, bind( &Map::left_value_type::second, _1 ) ) ) ;
	m_map.clear( ) ;
}

template <typename T>
Name ResourceSet<T>::Find( const T *t ) const
{
	typename Map::right_const_iterator i = m_map.right.find(const_cast<T*>(t));
	return i != m_map.right.end() ? i->second : Name() ;
}

template <typename T>
T* ResourceSet<T>::Find( const Name& name ) const
{
	typename Map::left_const_iterator i = m_map.left.find( name ) ;
	return i != m_map.left.end() ? const_cast<T*>( i->second ) : 0 ;
}

template <typename T>
Name ResourceSet<T>::Add( T *t )
{
	PDF_ASSERT( t != 0 ) ;

	// first, see if the font is already added
	using namespace boost ;
	typename Map::right_iterator it = m_map.right.find( t ) ;
	if ( it != m_map.right.end( ) )
		return it->second ;

	std::size_t idx = m_map.size( ) ;

	// create a new name
	Name name ;
	do
	{
		std::ostringstream oss ;
		oss << m_prefix << idx++ ;
		name = Name( oss.str() ) ;

	} while ( m_map.left.find( name ) != m_map.left.end( ) ) ;

	m_map.insert( typename Map::value_type( name, t ) ) ;
	t->AddRef() ;
	
	return name ;
}

template <typename T>
typename ResourceSet<T>::iterator ResourceSet<T>::begin( ) const
{
	return m_map.left.begin() ;
}

template <typename T>
typename ResourceSet<T>::iterator ResourceSet<T>::end( ) const
{
	return m_map.left.end() ;
}

} // end of namespace

// instantiations

#include "font/BaseFont.hh"
#include "graphics/RealColorSpace.hh"
#include "graphics/XObject.hh"
#include "ExtGState.hh" 
namespace pdf
{
	template class ResourceSet<BaseFont> ;
	template class ResourceSet<RealColorSpace> ;
	template class ResourceSet<XObject> ;
	template class ResourceSet<ExtGState> ;
}
