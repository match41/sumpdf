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
	\file	Image.hh
	\brief	definition the Image class
	\date	Thu Feb 26 2009
	\author	Nestal Wan
*/

#ifndef __IMG_IMAGE_HEADER_INCLUDED__
#define __IMG_IMAGE_HEADER_INCLUDED__

#include <cstddef>

namespace img {

/*!	\brief	brief description
	
	this class represents
*/
class Image
{
public :
	enum PixFormat { RGB, RGBA, BGR, BGRA, grey } ;

public :
	virtual ~Image( ) ;

	//! width of the image in pixels
	virtual std::size_t Width( ) const = 0 ;
	
	//! height of the image in pixels
	virtual std::size_t Height( ) const = 0 ;

	//! number of bits per pixel of the image
	virtual std::size_t Depth( ) const = 0 ;

	//! pixel format
	virtual PixFormat Format( ) const = 0 ;
	
	//! size of one row of pixel
	virtual std::size_t RowSize( ) const = 0 ;

	//! read one row of pixel. the buffer must be at least RowSize().
	virtual bool ReadRow( unsigned char *pixel ) = 0 ;
} ;

} // end of namespace

#endif
