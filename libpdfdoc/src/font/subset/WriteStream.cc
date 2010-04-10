/***************************************************************************\
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
\***************************************************************************/

/**	\file	WriteStream.cc
	\brief	implementation of the WriteStream class
	\date	Apr 10, 2010
	\author	Nestal Wan
*/

#include "WriteStream.hh"

#include "Endian.hh"
#include "Types.hh"

// boost headers
#include <boost/detail/endian.hpp>

namespace pdf {

/**	constructor
	
*/
WriteStream::WriteStream( std::streambuf *buf )
	: m_buf( buf )
{
}

template <typename T>
WriteStream& WriteStream::operator<<( T v )
{
#ifdef BOOST_LITTLE_ENDIAN
	v = SwapByte( v ) ;
#endif
	m_buf->sputn( reinterpret_cast<const char*>(&v), sizeof(v) ) ;

	return *this ;
}

template WriteStream& WriteStream::operator<<( u32 v ) ;
template WriteStream& WriteStream::operator<<( u16 v ) ;

} // end of namespace
