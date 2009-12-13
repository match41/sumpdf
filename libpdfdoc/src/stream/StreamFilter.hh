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
	\file	StreamFilter.hh
	\brief	definition the StreamFilter class
	\date	Wed Mar 4 2009
	\author	Nestal Wan
*/

#ifndef __PDF_STREAM_FILTER_HEADER_INCLUDED__
#define __PDF_STREAM_FILTER_HEADER_INCLUDED__

#include <cstddef>

namespace pdf {

class Object ;

/*!	\addtogroup	filter Filter
	\brief	Filter classes for Stream.
	
	The filter classes are used to decode/encode data to/from a PDF stream.
*/

/*!	\brief		Filter interface for PDF Stream.
	\ingroup	filter
	
	This is the base class (interface) for all PDF stream filters. It provides
	virtual functions for basic operations on the data.
*/
class StreamFilter
{
public :
	virtual ~StreamFilter( ) ;
	
	virtual std::size_t Read( unsigned char *data, std::size_t size ) = 0 ;
	virtual std::size_t Write( const unsigned char *data, std::size_t size )=0;
	virtual void Reset( ) = 0 ;
	virtual std::size_t Length( ) const = 0 ;
	virtual Object GetFilterName( ) const = 0 ;
} ;

} // end of namespace

#endif
