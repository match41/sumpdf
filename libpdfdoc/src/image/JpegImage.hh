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
	\file	JpegImage.hh
	\brief	definition the JpegImage class
	\date	Sat Feb 28 2009
	\author	Nestal Wan
*/

#ifndef __IMG_JPEG_IMAGE_HEADER_INCLUDED__
#define __IMG_JPEG_IMAGE_HEADER_INCLUDED__

#include "Pixmap.hh"

#include <cstdio>		// must include it before jpeglib header
#include <jpeglib.h>

#include <string>

namespace img {

/*!	\brief	brief description
	
	this class represents
*/
class JpegImage : public Pixmap
{
public :
	JpegImage( const std::string& filename ) ;
	~JpegImage( ) ;

	//! width of the image in pixels
	virtual std::size_t Width( ) const ;
	
	//! height of the image in pixels
	virtual std::size_t Height( ) const ;

	//! number of bits per pixel of the image
	virtual std::size_t Depth( ) const ;

	//! pixel format
	virtual PixFormat Format( ) const ;
	
	//! size of one row of pixel
	virtual std::size_t RowSize( ) const ;

	//! read one row of pixel. the buffer must be at least RowSize().
	virtual bool ReadRow( unsigned char *pixel ) ;

public :
	static void my_error_exit( j_common_ptr cinfo ) ;

private :
	std::FILE	*m_fp ;
	
	struct jpeg_error_mgr 			m_jerr ;
	struct jpeg_decompress_struct	m_cinfo ;
} ;

} // end of namespace

#endif
