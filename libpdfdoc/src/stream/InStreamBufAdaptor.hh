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
	\file	InStreamBufAdaptor.hh
	\brief	definition the StreamBufAdaptor class
	\date	Wed Mar 4 2009
	\author	Nestal Wan
*/

#ifndef __PDF_STREAM_BUF_ADAPTOR_HEADER_INCLUDED__
#define __PDF_STREAM_BUF_ADAPTOR_HEADER_INCLUDED__

#include <streambuf>

namespace pdf {

class StreamFilter ;

/*!	\brief	an adaptor from StreamFilter to streambuf
	
	This class transform a StreamFilter to an std::streambuf.
*/
class InStreamBufAdaptor : public std::streambuf
{
public :
	explicit InStreamBufAdaptor( StreamFilter *str = 0 ) ;
	InStreamBufAdaptor( const InStreamBufAdaptor& rhs ) ;

	void Set( StreamFilter *str ) ;

protected :
	int underflow( ) ;

private :
    bool BufferIn( ) ;

private :
	static const std::streamsize	m_buf_size	= 80 ;
	static const std::streamsize    m_pb_size	= 4 ;

	char 			m_buf[m_buf_size] ;
	StreamFilter	*m_str ;
} ;

} // end of namespace

#endif
