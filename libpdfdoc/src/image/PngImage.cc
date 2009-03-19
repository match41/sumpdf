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
	\file	PngImage.cc
	\brief	implementation the PngImage class
	\date	Thu Feb 26 2009
	\author	Nestal Wan
*/

#include "PngImage.hh"

#include <cstring>
#include <iostream>

namespace img {

PngImage::PngImage( const std::string& fname )
	: m_fp( std::fopen( fname.c_str( ), "rb" ) ),
	  m_png( png_create_read_struct( PNG_LIBPNG_VER_STRING, 0, 0, 0 ) ),
	  m_info( 0 ),
	  m_current_row( m_rows.end( ) )
{
	try
	{
		if ( m_png == 0 || m_fp == 0 )
			throw -1 ;
		
		m_info = ::png_create_info_struct( m_png ) ;
		
		
		if ( setjmp( png_jmpbuf( m_png ) ) )
			throw -1 ;
	
		png_init_io( m_png, m_fp ) ;
		png_read_info( m_png, m_info ) ;

		// set libpng transformation
		SetTransform( ) ;
		
		// for interlace images, we read the whole image to memory first
		if ( png_get_interlace_type( m_png, m_info ) != PNG_INTERLACE_NONE )
			ReadWholeImage( ) ;
	}
	catch ( ... )
	{
		if ( m_fp != 0 )
			std::fclose( m_fp ) ;
		
		if ( m_png != 0 )
			png_destroy_read_struct( &m_png, &m_info, png_infopp_NULL ) ;
	}
}

PngImage::~PngImage( )
{
	png_destroy_read_struct( &m_png, &m_info, png_infopp_NULL ) ;
	if ( m_fp != 0 )
		std::fclose( m_fp ) ;
}

void PngImage::SetTransform( )
{
	// tell libpng to strip 16 bit/color files down to 8 bits/color
	png_set_strip_16( m_png ) ;
	
	// Strip alpha bytes from the input data without combining with the
	// background (not recommended).
	png_set_strip_alpha( m_png ) ;
	
	// Extract multiple pixels with bit depths of 1, 2, and 4 from a single
	// byte into separate bytes (useful for paletted and grayscale images).
	png_set_packing( m_png ) ;
	
	// Change the order of packed pixels to least significant bit first
	// (not useful if you are using png_set_packing).
	png_set_packswap( m_png ) ;
	
	// Expand paletted colors into true RGB triplets
	png_uint_32 color_type = png_get_color_type( m_png, m_info ) ;
	if ( color_type == PNG_COLOR_TYPE_PALETTE )
		png_set_palette_to_rgb( m_png ) ;
	
	// Expand grayscale images to the full 8 bits from 1, 2, or 4 bits/pixel
	png_uint_32 bit_depth = png_get_bit_depth( m_png, m_info ) ;
	if ( color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8 )
		png_set_expand_gray_1_2_4_to_8( m_png );
	
	// Expand paletted or RGB images with transparency to full alpha
	// channels so the data will be available as RGBA quartets.
	if ( png_get_valid( m_png , m_info, PNG_INFO_tRNS ) )
		png_set_tRNS_to_alpha( m_png );
}

void PngImage::ReadWholeImage( )
{
	png_uint_32 height = png_get_image_width( m_png, m_info ) ;
	m_rows.resize( height ) ;
	for ( png_uint_32 row = 0; row < height; row++ )
		m_rows[row] = reinterpret_cast<png_bytep>(png_malloc( m_png,
		                                                      RowSize( ) ) ) ;

	png_read_image( m_png, &m_rows[0] ) ;
	m_current_row = m_rows.begin( ) ;
}

//! width of the image in pixels
std::size_t PngImage::Width( ) const
{
	return png_get_image_width( m_png, m_info ) ;
}

//! height of the image in pixels
std::size_t PngImage::Height( ) const
{
	return png_get_image_height( m_png, m_info ) ;
}

//! number of bits per pixel of the image
std::size_t PngImage::Depth( ) const
{
	return 8 ;
}

//! pixel format
PngImage::PixFormat PngImage::Format( ) const
{
	return RGB ;
}

//! read one row of pixel
bool PngImage::ReadRow( unsigned char *pixel )
{
	if ( m_rows.empty( ) )
		png_read_row( m_png, pixel, NULL ) ;
	else
	{
		if ( m_current_row == m_rows.end( ) )
			return false ;

		std::memcpy( pixel, *m_current_row, RowSize( ) ) ;
		++m_current_row ;
	}
	return true ;
}

std::size_t PngImage::RowSize( ) const
{
	return png_get_rowbytes( m_png, m_info ) ;
}

} // end of namespace
