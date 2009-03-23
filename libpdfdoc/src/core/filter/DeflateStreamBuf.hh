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
	\file	DeflateStreamBuf.hh
	\brief	definition the DeflateStreamBuf class
	\date	Sun Mar 22 2009
	\author	Nestal Wan
*/

#ifndef __PDF_DEFLATE_STREAM_BUF_HEADER_INCLUDED__
#define __PDF_DEFLATE_STREAM_BUF_HEADER_INCLUDED__

#include <streambuf>
#include <zlib.h>

namespace pdf {

/*!	\brief	brief description
	
	this class represents
*/
class DeflateStreamBuf : public std::streambuf
{
public :
	DeflateStreamBuf( std::streambuf *str ) ;

protected :
	int_type underflow( ) ;
	int_type pbackfail( int_type c ) ;

private :
	int_type BufferIn( ) ;

private :
	static const std::streamsize	m_buf_size	= 16 ;
	static const std::streamsize m_pb_size	= 4 ;

	std::streambuf	*m_str ;
	char_type		m_buf[m_buf_size], m_comp[80] ;
	z_stream		m_zstr ;
} ;

} // end of namespace

#endif
