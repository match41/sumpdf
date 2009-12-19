/***************************************************************************
 *   Copyright (C) 2006 by Nestal Wan                                      *
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
 ***************************************************************************/

/*!
	\file	RawFilter.cc
	\brief	implementation the RawFilter class
	\date	Wed Mar 4 2009
	\author	Nestal Wan
*/

#include "RawFilter.hh"
#include "BufferedFilter.hh"

#include "core/Object.hh"

#include <cassert>
#include <streambuf>

namespace pdf {

/*!	\brief	constructor

	The only constructor to initialize the object.
	\param	file	a streambuf to be read from
	\param	start	the starting offset of the file to be read
*/
RawFilter::RawFilter( std::streambuf *file, std::streamoff start, 
                      std::size_t length )
	: m_file( file ), m_start( start ), m_current( 0 ), m_length( length )
{
	assert( m_file != 0 ) ;
}

StreamFilter* RawFilter::Clone( ) const
{
	std::vector<unsigned char> buf( m_length ) ;
	m_file->pubseekpos( m_start, std::ios::in ) ;
	std::size_t c = m_file->sgetn( reinterpret_cast<char*>( &buf[0] ),
	                               m_length ) ;
	// TODO: throw better things
	if ( c != m_length )
		throw -1 ;
	
	return new BufferedFilter( buf ) ;
}

/*!	\brief	reading from file
	
	This function will read a number of bytes from the underlying file.
	Before reading, it will seek to the current position to prevend the
	position being changed by others.
	\param[out]	data	pointer to buffer to store data read from file
	\param[in]	size	size of the buffer pointed by "data" in bytes
	\return				the number of bytes read from the file. 0 if
						end-of-file occurs.
*/
std::size_t RawFilter::Read( unsigned char *data, std::size_t size )
{
	assert( m_file != 0 ) ;
	m_file->pubseekpos( m_start + m_current, std::ios::in ) ;
	
	std::size_t count = std::min<std::size_t>( m_length - m_current, size ) ;
	
	std::size_t c = m_file->sgetn( reinterpret_cast<char*>( data ), count ) ;
	m_current += c ;
	
	return c ;
}

std::size_t RawFilter::Write( const unsigned char *data, std::size_t size )
{
	assert( m_file != 0 ) ;
	m_file->pubseekpos( m_start + m_current, std::ios::out ) ;
	
	std::size_t count = std::min<std::size_t>( m_length - m_current, size ) ;
	
	std::size_t c = m_file->sputn(reinterpret_cast<const char*>( data ), count);
	m_current += c ;
	
	return c ;
}

void RawFilter::Reset( )
{
	assert( m_file != 0 ) ;
	
	m_current = 0 ;
	m_file->pubseekpos( m_start, std::ios::in ) ;
}

std::size_t RawFilter::Length( ) const
{
	return m_length - m_current ;
}

Object RawFilter::GetFilterName( ) const
{
	return Object( ) ;
}

StreamFilter* RawFilter::GetInner( )
{
	return this ;
}

} // end of namespace
