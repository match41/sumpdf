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
	\file	RawFilter.hh
	\brief	definition the RawFilter class
	\date	Wed Mar 4 2009
	\author	Nestal Wan
*/

#ifndef __PDF_RAW_FILTER_HEADER_INCLUDED__
#define __PDF_RAW_FILTER_HEADER_INCLUDED__

#include "StreamFilter.hh"

#include <iosfwd>
#include <fstream>
#include <limits>

namespace pdf {

/*!	\brief		filter class to read data from file.
	\ingroup	filter

	This class is not actually a filter. It will read data directly from
	the streambuf specified in the constructor.
*/
class RawFilter : public StreamFilter
{
public :
	RawFilter( std::streambuf *file, std::streamoff start = 0,
	           std::size_t length = std::numeric_limits<std::size_t>::max() ) ;

	std::size_t Read( unsigned char *data, std::size_t size ) ;
	std::size_t Write( const unsigned char *data, std::size_t size ) ;

	void Reset( ) ;
	std::size_t Length( ) const ;
	Object GetFilterName( ) const ;

private :
	std::streambuf			*m_file ;		//!< the file to be read from
	const std::streamoff	m_start ;		//!< starting position
	std::streamoff			m_current ;		//!< current file position
	const std::size_t		m_length ;		//!< length of the data
} ;

} // end of namespace

#endif
