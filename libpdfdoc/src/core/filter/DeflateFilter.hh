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
	\file	DeflateFilter.hh
	\brief	definition the DeflateFilter class
	\date	Wed Mar 4 2009
	\author	Nestal Wan
*/

#ifndef __PDF_DEFLATE_FILTER_HEADER_INCLUDED__
#define __PDF_DEFLATE_FILTER_HEADER_INCLUDED__

#include "StreamFilter.hh"

#include <vector>

#include <zlib.h>

namespace pdf {

/*!	\brief	brief description
	\ingroup	filter

	this class represents
*/
class DeflateFilter : public StreamFilter
{
public :
	DeflateFilter( StreamFilter *src ) ;
	~DeflateFilter( ) ;

	std::size_t Read( unsigned char *data, std::size_t size ) ;
	std::size_t Write( const unsigned char *data, std::size_t size ) ;

	void Reset( ) ;

private :
	void ReadFromSrc( ) ;

private :
	z_stream		m_zstr ;
	StreamFilter	*m_src ;
	
	static const std::size_t	m_buf_size = 80 ;
	std::vector<unsigned char> m_buf ;
} ;

} // end of namespace

#endif
