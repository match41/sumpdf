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
	\file	DeflateFilter.hh
	\brief	definition the DeflateFilter class
	\date	Wed Mar 4 2009
	\author	Nestal Wan
*/

#ifndef __PDF_DEFLATE_FILTER_HEADER_INCLUDED__
#define __PDF_DEFLATE_FILTER_HEADER_INCLUDED__

#include "StreamFilter.hh"

#include "util/Exception.hh"

#include <vector>
#include <memory>

#include <zlib.h>

namespace pdf {

/*!	\brief		the "FlateDecode" filter for streams
	\internal

	This class represents the "FlateDecode" filter (i.e. zlib) for compressing
	data. It is the most commonly used filter. It is used to save space by a
	lossless compression. Usually it is used by page content stream to store
	large amount of page data.
	
	\ingroup	filter
*/
class DeflateFilter : public StreamFilter
{
public :
	explicit DeflateFilter( std::auto_ptr<StreamFilter>	src ) ;
	~DeflateFilter( ) ;

	std::size_t Read( unsigned char *data, std::size_t size ) ;
	std::size_t Write( const unsigned char *data, std::size_t size ) ;

	void Rewind( ) ;
	std::size_t Length( ) const ;
	Object NameChain( ) const ;
	DeflateFilter* Clone( ) const ;
	StreamFilter* GetInner( ) ;
	
	void Flush( ) ;
	
	class Error : public Exception
	{
	public :
		Error( const char *func, const char *msg ) ;
	} ;

	Name RawFormat( ) const ;

private :
	struct ZData ;

private :
	struct ZData
	{
		z_stream					z ;
		std::vector<unsigned char>	buf ;
	} m_comp, m_decomp ;

	bool	m_is_need_flush ;

	const std::auto_ptr<StreamFilter>	m_src ;
	
	static const std::size_t	m_buf_size = 80 ;
} ;

} // end of namespace

#endif
