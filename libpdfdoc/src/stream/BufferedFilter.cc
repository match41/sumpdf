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

#include <algorithm>
#include <cassert>
#include <cstring>

namespace pdf {

BufferedFilter::BufferedFilter( std::vector<unsigned char>& buf )
	: m_offset( 0 )
{
	m_buf.swap( buf ) ;
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
	return 0 ;
}

void BufferedFilter::Reset( )
{
	m_offset = 0 ;
}

} // end of namespace
