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
	\file	TrueTypeTable.hh
	\brief	definition the TrueTypeTable class
	\date	Sat Mar 7 2009
	\author	Nestal Wan
*/

#ifndef __FONT_BYTE_STREAM_HEADER_INCLUDED__
#define __FONT_BYTE_STREAM_HEADER_INCLUDED__

#include <iosfwd>

#include "Types.hh"

namespace font {

/*!	\brief	brief description
	
	this class represents
*/
class TrueTypeTable
{
public :
	TrueTypeTable( std::streambuf *file ) ;
	TrueTypeTable( std::streambuf *file, std::streamoff offset,
	               std::size_t length ) ;

	template <typename T>
	friend TrueTypeTable& operator>>( TrueTypeTable& bs, T& val ) ;

	template <typename T>
	friend TrueTypeTable& operator<<( TrueTypeTable& bs, T val ) ;

	void EndStream( ) ;
	std::size_t Size( ) const ;
	uint32_t Checksum( ) const ;

	std::size_t Write( const unsigned char *data, std::size_t size ) ;
	std::size_t Copy( std::streambuf *src, std::size_t size ) ;

	void PutByte( unsigned char byte ) ;

private :
	unsigned char AddChecksum( unsigned char c ) ;

private :
	std::streambuf	*m_file ;
	uint32_t		m_checksum ;
	std::size_t		m_length ;
	std::streamoff	m_offset ;

	// temperory variable for calculating checksum
	uint32_t		m_temp_long ;
} ;

} // end of namespace

#endif
