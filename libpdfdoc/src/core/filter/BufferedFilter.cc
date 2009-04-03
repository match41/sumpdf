/***************************************************************************\
 *   Copyright (C) 2009 by Nestal Wan                                      *
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
\***************************************************************************/

/*!
	\file	BufferedFilter.cc
	\brief	implementation the BufferedFilter class
	\date	Thu Apr 2 2009
	\author	Nestal Wan
*/

#include "BufferedFilter.hh"

namespace pdf {

BufferedFilter::BufferedFilter( const std::string& str )
	: m_buf( str ),
	  m_filter( m_buf.rdbuf() )
{
}

std::size_t BufferedFilter::Read( unsigned char *data, std::size_t size )
{
	return m_filter.Read( data, size ) ;
}

std::size_t BufferedFilter::Write( const unsigned char *data, std::size_t size )
{
	return m_filter.Write( data, size ) ;
}

void BufferedFilter::Reset( )
{
	m_buf.clear( ) ;
	m_filter.Reset( ) ;
}

} // end of namespace
