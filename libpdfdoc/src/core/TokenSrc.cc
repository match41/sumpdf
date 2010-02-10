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
	\file	TokenSrc.cc
	\brief	implementation the TokenSrc class
	\date	Wed Mar 4 2009
	\author	Nestal Wan
*/

#include "TokenSrc.hh"

#include "util/SymbolInfo.hh"
#include "util/Debug.hh"

#include <boost/bind.hpp>

#include <istream>
#include <iostream>
#include <cassert>
#include <algorithm>

namespace pdf {

TokenSrc::TokenSrc( std::istream& file )
	: m_file( file ),
	  m_pos( 0 )
{
}

TokenSrc& TokenSrc::ExtractToken( Token& token )
{
	m_file >> token ;
	return *this ;
}

TokenSrc& TokenSrc::ReadToken( Token& token )
{
	if ( m_cache.empty( ) )
	{
		ExtractToken( token ) ;
	}
	else	
	{
		token.Swap( m_cache.back() ) ;
		m_cache.pop_back( ) ;
		
		if ( m_pos > 0 )
		{
			std::string t = token.Get( ) ;
			t.erase( 0, m_pos ) ;
			m_pos = 0 ;
			
			token.Swap( t ) ;
		}
	}

	return *this ;
}

/*!	put back tokens to the TokenSrc. Tokens are put back in a first-in-last-out
	order.
*/
void TokenSrc::PutBack( const Token& token )
{
	m_cache.push_back( token ) ;
}

TokenSrc::operator void*( ) const
{
	return m_file ;
}

TokenSrc& operator>>( TokenSrc& src, Token& token )
{
	return src.ReadToken( token ) ;
}

std::istream& TokenSrc::Stream( )
{
	return m_file ;
}

void TokenSrc::SetState( std::ios::iostate state )
{
	m_file.setstate( state ) ;
}

/**	\brief	Reset the state of the TokenSrc.

	After calling this function the state will be reset. Call this function
	after failing to read a token before resume from reading.
*/
void TokenSrc::ResetState( )
{
	m_file.clear( ) ;
	assert( m_file ) ;
}

/**	\brief	Check if the TokenSrc has cached tokens.

	\return	true if there are cached tokens, otherwise false.
*/
bool TokenSrc::HasCache( ) const
{
	return !m_cache.empty( ) ;
}

/**	\brief	Read one character from the TokenSrc.

	If there are cached tokens, one character will be stripped from it.
	Otherwise one character will be read from the underlying stream.
	\param	ch	reference to the newly read character.
	\return	*this 
*/
TokenSrc& TokenSrc::GetChar( char& ch )
{
	if ( m_cache.empty() )
		m_file.get( ch ) ;
	else
	{
		const std::string& s = m_cache.back().Get() ;
		
		PDF_ASSERT( m_pos < s.size( ) ) ;
		ch = s[m_pos] ;
		
		// read the last character of the token. now the whole token
		// is extracted and we should remove it from our cache.
		if ( ++m_pos == s.size( ) )
		{
			m_cache.pop_back( ) ;	// remember, s is now invalid
			m_pos = 0 ;
		}
	}
	
	return *this ;
}

/*!	\brief	ensure the cache has a number of tokens

	This function will ensure the cache has \a count number of tokens. If the
	cache already has so, it will do nothing. Otherwise, it will read from
	the stream for tokens and store them to the cache.
	\param	count	the number of tokens
	\return	true if the required number of token is cached. otherwise false.
*/
bool TokenSrc::Cache( std::size_t count )
{
	while ( m_cache.size() < count )
	{
		pdf::Token t ;
		if ( ExtractToken( t ) )
			m_cache.push_front( t ) ;
		else
			break ;
	}
	
	return m_file && m_cache.size( ) >= count ;
}

/*!	\brief	extract and discard tokens

	This function will extract and discard \a count tokens. It will start with
	cached tokens of course. Then continue to extract tokens from streams to
	meet the requirement.
	\param	count	number of token to extract and discard
*/
void TokenSrc::Ignore( std::size_t count )
{
	while ( !m_cache.empty( ) && count > 0 )
	{
		m_cache.pop_back( ) ;
		count-- ;
	}

	Token t ;
	while ( count > 0 && m_file )
	{
		ExtractToken( t ) ;
		count-- ;
	}
}

} // end of namespace
