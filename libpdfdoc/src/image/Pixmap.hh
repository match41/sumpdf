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
	\file	Image.hh
	\brief	definition the Image class
	\date	Thu Feb 26 2009
	\author	Nestal Wan
*/

#ifndef __IMG_PIXMAP_HEADER_INCLUDED__
#define __IMG_PIXMAP_HEADER_INCLUDED__

#include <cstddef>

namespace img {

/**	\brief	An abstraction of an array of pixels
	
	This interface represents an array of pixels. It is used by libpdfdoc as
	an independent way to transfer and store image data. It cannot be directly
	added to a page in a document, but it can be used with Doc::AddImage()
	to produce an Image object, which can be added to a page.
	
	Clients of libpdfdoc can inherit this interface can provide there own image
	class. Out of the box libpdfdoc supports PNG and JPEG optionally. Clients
	can derive this class and use their own toolkit to implement it. For
	example, Sumpdf uses the Qt image classes to support more image formats.  
*/
class Pixmap
{
public :
	enum PixFormat { RGB, RGBA, BGR, BGRA, grey } ;

public :
	virtual ~Pixmap( ) ;

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
