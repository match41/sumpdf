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
	\file	PngImage.hh
	\brief	definition the PngImage class
	\date	Thu Feb 26 2009
	\author	Nestal Wan
*/

#ifndef __IMG_PNG_IMAGE_HEADER_INCLUDED__
#define __IMG_PNG_IMAGE_HEADER_INCLUDED__

#include "Image.hh"

// libpng header. As client will not include this file directly, it is OK
// to include it here.
#include <png.h>

#include <cstdio>
#include <string>
#include <vector>

namespace img {

/*!	\brief	brief description
	
	this class represents
*/
class PngImage : public Image
{
public :
	PngImage( const std::string& filename ) ;
	~PngImage( ) ;
	
	//! width of the image in pixels
	std::size_t Width( ) const ;
	
	//! height of the image in pixels
	std::size_t Height( ) const ;

	//! number of bits per pixel of the image
	std::size_t Depth( ) const ;

	//! pixel format
	PixFormat Format( ) const ;
	
	std::size_t RowSize( ) const ;
	
	//! read one row of pixel
	bool ReadRow( unsigned char *pixel ) ;

private :
	void SetTransform( ) ;

	bool IsWholeImageLoaded( ) const ;
	
	void ReadWholeImage( ) ;

private :
	std::FILE	*m_fp ;
	png_structp	m_png ;
	png_infop	m_info ;

	//!	image data. should be empty for non-progressive image.
	std::vector<png_bytep>	m_rows ;
	
	std::vector<png_bytep>::const_iterator m_current_row ;
} ;

} // end of namespace

#endif
