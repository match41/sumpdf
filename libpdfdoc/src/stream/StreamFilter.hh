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
	\file	StreamFilter.hh
	\brief	definition the StreamFilter class
	\date	Wed Mar 4 2009
	\author	Nestal Wan
*/

#ifndef __PDF_STREAM_FILTER_HEADER_INCLUDED__
#define __PDF_STREAM_FILTER_HEADER_INCLUDED__

#include <cstddef>
#include <iosfwd>

namespace pdf {

class Name ;
class Object ;

/*!	\addtogroup	filter Filter
	\brief	Filter classes for Stream.
	\internal
	
	The filter classes are used to decode/encode data to/from a PDF stream. A
	PDF document uses stream objects to represent page content, font program and
	other things. It defines some flexible facilities to store byte streams
	efficiently.
*/

/*!	\brief		Filter interface for PDF Stream.
	\ingroup	filter
	\internal

	This is the base class (interface) for all PDF stream filters. It provides
	virtual functions for basic operations on the data.
*/
class StreamFilter
{
public :
	virtual ~StreamFilter( ) ;
	
	/**	\brief	Read data from stream.
	
		This function reads data from a stream.
		\param	data	pointer to stored the data read from stream. It must
						be large enough to hold \a size bytes.
		\param	size	number of bytes the caller wants to read.
		\return	number of bytes actually read from stream. If it is zero, then
				it means the end of stream is reached and no more data can
				be extracted.
		\throw	StreamError	when an error occurs.
	*/
	virtual std::size_t Read( unsigned char *data, std::size_t size ) = 0 ;
	virtual std::size_t Write( const unsigned char *data, std::size_t size )=0;
	virtual void Flush( ) = 0 ;
	virtual void Rewind( ) = 0 ;
	virtual std::size_t Length( ) const = 0 ;
	virtual Object NameChain( ) const = 0 ;
	virtual StreamFilter* Clone( ) const = 0 ;
	virtual StreamFilter* GetInner( ) = 0 ;
	
	/// Raw format of the data.
	/**	If the 
	*/
	virtual Name RawFormat( ) const = 0 ;
} ;

} // end of namespace

#endif
