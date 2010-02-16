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
	\file	Types.hh
	\brief	Definition the sfnt types.
	\date	Sat Mar 7 2009
	\author	Nestal Wan
	
	This header file contains the definition of the sfnt types. It is defined
	in http://developer.apple.com/textfonts/TTRefMan/RM06/Chap6.html#Types
*/

#ifndef __FONT_TYPES_HEADER_INCLUDED__
#define __FONT_TYPES_HEADER_INCLUDED__

#include <boost/cstdint.hpp>

namespace pdf { namespace font {

typedef boost::uint8_t	uint8_t ;
typedef	boost::int8_t	int8_t ;
typedef boost::uint16_t	uint16_t ;
typedef	boost::int16_t	int16_t ;
typedef boost::uint32_t	uint32_t ;
typedef	boost::int32_t	int32_t ;
typedef boost::uint64_t	uint64_t ;
typedef	boost::int64_t	int64_t ;

typedef boost::int16_t	fraction_t ;
typedef boost::int16_t	fixed_t ;
typedef boost::int16_t	fword_t ;
typedef boost::uint16_t	ufword_t ;
typedef boost::int16_t	f2dot14_t ;
typedef boost::int64_t	date_time_t ;

} } // end of namespace

#endif
