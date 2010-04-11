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

/**	\file	Endian.cc
    \brief	definition the Endian class
    \date	Apr 10, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_ENDIAN_CC_EADER_INCLUDED__
#define __PDF_ENDIAN_CC_EADER_INCLUDED__

#include "Endian.hh"
#include "Types.hh"

// boost headers
#include <boost/detail/endian.hpp>

#include <cstring>

namespace pdf {

#ifdef __GNUC__

template <>
u32 SwapByte( u32 t )
{
	return __builtin_bswap32( t ) ;
}

// for others
#else

template <>
u32 SwapByte( u32 t )
{
	return
		((t & 0xff)		<< 24UL ) |
		((t & 0xff00)	<< 16UL ) |
		((t & 0xff0000)	<< 8UL ) |
		(t >> 8) ;
}

#endif

template <>
u16 SwapByte( u16 t )
{
	return ((t & 0xff) << 8 ) | (t >> 8) ;
}

template <typename T>
void WriteBigEndian( T value, unsigned char *ptr )
{
#ifdef BOOST_LITTLE_ENDIAN
	value = SwapByte( value ) ;
#endif
	std::memcpy( ptr, &value, sizeof(value) ) ;
}

template void WriteBigEndian( u32 value, unsigned char *ptr ) ;
template void WriteBigEndian( u16 value, unsigned char *ptr ) ;

} // end of namespace

#endif // ENDIAN_CC_
