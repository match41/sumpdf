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

// boost headers
#include <boost/detail/endian.hpp>
#include <boost/cstdint.hpp>

#include <cstring>
#include <algorithm>

#ifdef _MSC_VER
#include <intrin.h>
#endif

namespace pdf {

using namespace boost ;

#ifdef __GNUC__

template <>
uint32_t SwapByte( uint32_t t )
{
	return __builtin_bswap32( t ) ;
}

#elif _MSC_VER

template <>
uint32_t SwapByte( uint32_t t )
{
	return _byteswap_ulong( t ) ;
}

// for others
#else

template <>
uint32_t SwapByte( uint32_t t )
{
	uchar *p = reinterpret_cast<uchar*>( &t ) ;
	std::swap( p[0], p[3] ) ;
	std::swap( p[1], p[2] ) ;
	return t ;
}

#endif

template <>
uint16_t SwapByte( uint16_t t )
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

template void WriteBigEndian( uint32_t value, unsigned char *ptr ) ;
template void WriteBigEndian( uint16_t value, unsigned char *ptr ) ;

template <typename T>
T ReadBigEndian( const unsigned char *ptr )
{
	T value ;
	std::memcpy( &value, ptr, sizeof(value) ) ;

	return ToBigEndian( value ) ;
}

template <typename T>
T ToBigEndian( T value )
{
#ifdef BOOST_LITTLE_ENDIAN
	value = SwapByte( value ) ;
#endif
	return value ;
}

template uint16_t ToBigEndian( uint16_t ) ;
template uint32_t ToBigEndian( uint32_t ) ;

template <typename T>
T ToLittleEndian( T value )
{
#ifdef BOOST_BIG_ENDIAN
	value = SwapByte( value ) ;
#endif
	return value ;
}

template uint16_t ToLittleEndian( uint16_t ) ;
template uint32_t ToLittleEndian( uint32_t ) ;

} // end of namespace

#endif // ENDIAN_CC_
