/***************************************************************************\
 *   Copyright (C) 2009 by Nestal Wan                                      *
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
 \***************************************************************************/

/**
 \file	OutStreamBufAdaptor.hh
 \brief	definition the OutStreamBufAdaptor class
 \date	Dec 21, 2009
 \author	nestal
 */

#ifndef __PDF_OUTSTREAMBUFADAPTOR_HEADER_INCLUDED__
#define __PDF_OUTSTREAMBUFADAPTOR_HEADER_INCLUDED__

#include <streambuf>

namespace pdf {

class StreamFilter ;

class OutStreamBufAdaptor : public std::streambuf
{
public:
	explicit OutStreamBufAdaptor( StreamFilter *dest = 0 ) ;
	~OutStreamBufAdaptor( ) ;

	void Set( StreamFilter *dest ) ;

protected :
	std::streamsize xsputn( const char_type *s, std::streamsize n ) ;
	int_type overflow( int_type c = traits_type::eof() ) ;
	int sync( ) ;

private :
	static const std::size_t 	m_buf_size = 80 ;
	char_type					m_buf[m_buf_size] ;
	
	int BufferOut( ) ;
	
	// prohibit copying and assignment
	OutStreamBufAdaptor( const OutStreamBufAdaptor& ) ;
	OutStreamBufAdaptor& operator=( const OutStreamBufAdaptor& ) ;

private :
	StreamFilter *m_dest ;
} ;

} // end of namespace

#endif // OUTSTREAMBUFADAPTOR_HH_
