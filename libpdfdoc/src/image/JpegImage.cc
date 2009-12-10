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
	\file	JpegImage.cc
	\brief	implementation the JpegImage class
	\date	Sat Feb 28 2009
	\author	Nestal Wan
*/

#include "JpegImage.hh"

#include <stdexcept>
#include <cerrno>
#include <cstring>

#include <iostream>

namespace img {

void JpegImage::my_error_exit( j_common_ptr cinfo )
{
	(*cinfo->err->output_message)( cinfo ) ;

	// Return control to the setjmp point
	throw std::runtime_error( "error!" ) ;
}

JpegImage::JpegImage( const std::string& filename )
	: m_fp( std::fopen( filename.c_str( ), "rb" ) )
{
	// try to open the file
	if ( m_fp == 0 )
		throw std::runtime_error( "cannot open file " + filename + ": " +
		                          strerror( errno ) ) ;

	// initialize decompression structures
	m_cinfo.err			= jpeg_std_error( &m_jerr ) ;
	m_jerr.error_exit	= my_error_exit;

	::jpeg_create_decompress( &m_cinfo ) ;

	::jpeg_stdio_src( &m_cinfo, m_fp ) ;
	::jpeg_read_header( &m_cinfo, true ) ;

	::jpeg_start_decompress( &m_cinfo ) ;
}

JpegImage::~JpegImage( )
{
	if ( m_cinfo.output_scanline >= m_cinfo.output_height )
		::jpeg_finish_decompress( &m_cinfo ) ;

	::jpeg_destroy_decompress( &m_cinfo ) ;
	std::fclose( m_fp ) ;
}

//! size of one row of pixel
std::size_t JpegImage::RowSize( ) const
{
	return m_cinfo.output_width * m_cinfo.output_components ;
}

//! width of the image in pixels
std::size_t JpegImage::Width( ) const
{
	return m_cinfo.output_width ;
}

//! height of the image in pixels
std::size_t JpegImage::Height( ) const
{
	return m_cinfo.output_height ;
}

//! number of bits per pixel of the image
std::size_t JpegImage::Depth( ) const
{
	return 8 ;
}

JpegImage::PixFormat JpegImage::Format( ) const
{
	return RGB ;
}

bool JpegImage::ReadRow( unsigned char *pixel )
{
	// decompress the scan lines one by one
	if ( m_cinfo.output_scanline < m_cinfo.output_height )
	{
		JSAMPLE *buf[] = { reinterpret_cast<JSAMPLE*>( pixel ) } ;
		::jpeg_read_scanlines( &m_cinfo, buf, 1 ) ;
		return true ;
	}
	else
		return false ;
}

} // end of namespace
