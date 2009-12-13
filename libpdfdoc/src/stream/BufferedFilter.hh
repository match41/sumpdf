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
	\file	BufferedFilter.hh
	\brief	definition the BufferedFilter class
	\date	Thu Apr 2 2009
	\author	Nestal Wan
*/

#ifndef __PDF_BUFFERED_FILTER_HEADER_INCLUDED__
#define __PDF_BUFFERED_FILTER_HEADER_INCLUDED__

#include "StreamFilter.hh"

#include <vector>

namespace pdf {

/*!	\brief	stream filter with whole data in memory
	\ingroup	filter

	The BufferedFilter is a StreamFilter with all its data in memory. It stores
	a chunk of memory and an current read pointer.
*/
class BufferedFilter : public StreamFilter
{
public :
	BufferedFilter( ) ;

	template <typename InputIt>
	BufferedFilter( InputIt first, InputIt last )
		: m_buf( first, last ),
		  m_offset( 0 )
	{
	}

	BufferedFilter( std::vector<unsigned char>& buf ) ;

	std::size_t Read( unsigned char *data, std::size_t size ) ;
	std::size_t Write( const unsigned char *data, std::size_t size ) ;
	void Reset( ) ;
	std::size_t Length( ) const ;
	Object GetFilterName( ) const ;
	
private :
	std::vector<unsigned char>	m_buf ;		//!< chunk of data in memory
	std::size_t					m_offset ;	//!< current read position
} ;

} // end of namespace

#endif
