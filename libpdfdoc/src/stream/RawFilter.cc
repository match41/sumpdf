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
	\file	RawFilter.cc
	\brief	implementation the RawFilter class
	\date	Wed Mar 4 2009
	\author	Nestal Wan
*/

#include "RawFilter.hh"
#include "BufferedFilter.hh"
#include "StreamError.hh"

#include "core/Object.hh"

#include <cassert>
#include <streambuf>

namespace pdf {

/*!	\brief	Constructor.

	The only constructor to initialize the object.
	\param	file	a streambuf to be read from
	\param	start	the starting offset of the file to be read
	\param	length	maximum number of bytes that can be read
*/
RawFilter::RawFilter( std::streambuf *file, std::streamoff start, 
                      std::size_t length )
	: m_file( file ), m_start( start ), m_current( 0 ), m_length( length )
{
	assert( m_file != 0 ) ;
}

/**	\brief	Create a copy of the filter.
	
	This function will create another filter such that the same data can be
	read from the filter. It will not create a RawFilter, but a BufferedFilter
	instead. It will read all the data from file and store them in the
	newly created BufferedFilter.
	\return	a BufferedFilter with data read from the file.
	\throw	StreamError	when data cannot be read from file. 
*/
StreamFilter* RawFilter::Clone( ) const
{
	std::vector<unsigned char> buf( m_length ) ;
	m_file->pubseekpos( m_start, std::ios::in ) ;
	std::size_t c = m_file->sgetn( reinterpret_cast<char*>( &buf[0] ),
	                               m_length ) ;
	if ( c != m_length )
		throw StreamError( "cannot read from file" ) ;
	
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

void RawFilter::Rewind( )
{
	assert( m_file != 0 ) ;
	
	m_current = 0 ;
	m_file->pubseekpos( m_start, std::ios::in ) ;
}

std::size_t RawFilter::Length( ) const
{
	return m_length ;
}

Object RawFilter::GetFilterName( ) const
{
	return Object( ) ;
}

/**	like the BufferedFilter, the RawFilter is the inner-most filter as well.
	Therefore GetInner() will just return \c this.
*/
StreamFilter* RawFilter::GetInner( )
{
	return this ;
}

/**	this function will do nothing. I am afraid the fsync() system call will
	take too much time on ext3 file system.
*/
void RawFilter::Flush( )
{
}

} // end of namespace
