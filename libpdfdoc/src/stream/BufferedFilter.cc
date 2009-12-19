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
	\file	BufferedFilter.cc
	\brief	implementation the BufferedFilter class
	\date	Thu Apr 2 2009
	\author	Nestal Wan
*/

#include "BufferedFilter.hh"

#include "core/Object.hh"

#include <algorithm>
#include <cassert>
#include <cstring>

namespace pdf {

/*!	default constructor will construct an empty buffer for writing.
*/
BufferedFilter::BufferedFilter( )
	: m_offset( 0 )
{
}

/*!	grand theft constructor. This constructor will steal the data
	from the argument vector \a buf . After calling this function, \a buf will
	be empty and the data originally in \a buf will be in the BufferedFilter
	objet.
	\param	buf		data to be put in the BufferedFilter. it will be empty
					after calling this function.
	\post	\a buf will be empty
*/
BufferedFilter::BufferedFilter( std::vector<unsigned char>& buf )
	: m_offset( 0 )
{
	m_buf.swap( buf ) ;
}

BufferedFilter::BufferedFilter( const char *str )
	: m_offset( 0 ), m_buf( str, str + std::strlen( str ) )
{
}

StreamFilter* BufferedFilter::Clone( ) const
{
	return new BufferedFilter( *this ) ;
}

std::size_t BufferedFilter::Read( unsigned char *data, std::size_t size )
{
	assert( m_offset <= m_buf.size() ) ;

	std::size_t count = std::min( size, m_buf.size() - m_offset ) ;
	if ( count > 0 )
		std::memcpy( data, &m_buf[m_offset], count ) ;
	
	m_offset += count ;		
	return count ;
}

std::size_t BufferedFilter::Write( const unsigned char *data, std::size_t size )
{
	m_buf.insert( m_buf.end(), data, data + size ) ;
	return size ;
}

void BufferedFilter::Reset( )
{
	m_offset = 0 ;
}

/**	This function returns the actual size of the buffer. The bytes already
	consumed by calling Read() will still be counted.
*/
std::size_t BufferedFilter::Length( ) const
{
	return m_buf.size( ) ;
}

Object BufferedFilter::GetFilterName( ) const
{
	return Object( ) ;
}

} // end of namespace
