/***************************************************************************\
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
\***************************************************************************/

/**	\file	PredictFilter.cc
	\brief	implementation of the PredictFilter class
	\date	Jun 25, 2010
	\author	Nestal Wan
*/

#include "PredictFilter.hh"

#include "core/Object.hh"

#include "util/Debug.hh"
#include "util/Exception.hh"

#include <algorithm>
#include <cstring>

#include <iostream>
#include <iomanip>

namespace pdf {

/**	constructor
	
*/
PredictFilter::PredictFilter( std::auto_ptr<StreamFilter> src, std::size_t col )
	: m_src( src )
	, m_buffer( col, '\0' )
	, m_idx( col )
{
}

std::size_t PredictFilter::Read( unsigned char *data, std::size_t size )
{
	std::size_t count = 0 ;
	while ( count < size )
	{
		PDF_ASSERT( m_idx <= m_buffer.size() ) ;
	
		if ( m_idx == m_buffer.size() && !FillBuffer( ) )
			break ;
		
		if ( m_idx < m_buffer.size() )
		{
			std::size_t n = std::min( m_buffer.size() - m_idx, size - count ) ;
			std::memcpy( data + count, &m_buffer[m_idx], n ) ;

			count += n ;
			m_idx += n ;
		}
		else
			throw ParseError( "unexpected end of stream" ) ;
	}
	return count ;
}

std::size_t PredictFilter::Write( const unsigned char *data, std::size_t size )
{
	return 0 ;
}

void PredictFilter::Flush( )
{
	
}

void PredictFilter::Rewind( )
{
	m_src->Length( ) ;
}

std::size_t PredictFilter::Length( ) const
{
	return m_src->Length() ;
}

Object PredictFilter::NameChain( ) const
{
	return m_src->NameChain( ) ;
}

StreamFilter* PredictFilter::GetInner( )
{
	return m_src.get() ;
}

Name PredictFilter::RawFormat( ) const
{
	return m_src->RawFormat() ;
}

bool PredictFilter::FillBuffer( )
{
	PDF_ASSERT( m_idx == m_buffer.size() ) ;
	
	std::vector<unsigned char> prev( m_buffer ) ;
	
	// try to read one row of data
	std::vector<unsigned char> tmp( m_buffer.size() + 1 ) ;
	std::size_t r = m_src->Read( &tmp[0], tmp.size() ) ;
	if ( r != tmp.size() )
		return false ;
	
	for ( std::size_t i = 0 ; i < m_buffer.size() ; i++ )
	{
		// the first byte in tmp is the predictor 
		switch ( tmp[0] )
		{
			case 0:	m_buffer[i] = tmp[i+1] ;			break ;
			case 2:	m_buffer[i] = tmp[i+1] + prev[i] ;	break ;
			default:
				break ;
		}
	}
	
	m_idx = 0 ;
	return true ;
}

StreamFilter* PredictFilter::Clone( ) const
{
	return new PredictFilter( std::auto_ptr<StreamFilter>( m_src->Clone() ),
		m_buffer.size() ) ;
}

} // end of namespace
