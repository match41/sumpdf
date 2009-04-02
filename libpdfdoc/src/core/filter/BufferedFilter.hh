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

#include "RawFilter.hh"

#include <sstream>

namespace pdf {

/*!	\brief	brief description
	
	this class represents
*/
class BufferedFilter : public StreamFilter
{
public :
	explicit BufferedFilter( const std::string& str = std::string() ) ;

	template <typename InputIt>
	BufferedFilter( InputIt first, InputIt last )
		: m_buf( std::string( first, last ) ),
		  m_filter( m_buf.rdbuf() )
	{
	}
	
	std::size_t Read( unsigned char *data, std::size_t size ) ;
	std::size_t Write( const unsigned char *data, std::size_t size ) ;
	void Reset( ) ;

private :
	std::stringstream	m_buf ;
	RawFilter			m_filter ;
} ;

} // end of namespace

#endif
