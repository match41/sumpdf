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
	\file	TokenSrc.hh
	\brief	definition the TokenSrc class
	\date	Wed Mar 4 2009
	\author	Nestal Wan
*/

#ifndef __PDF_TOKEN_SRC_HEADER_INCLUDED__
#define __PDF_TOKEN_SRC_HEADER_INCLUDED__

#include "Token.hh"
#include <deque>
#include <ios>

namespace pdf {

/*!	\brief	source of tokens
	\internal

	This class represents a place where tokens can be read from. It also has
	a cache so unused tokens can be put back. Further more, you can read bytes
	one-by-one from the TokenSrc also. The TokenSrc has a cache to store tokens
	that has been put back.
*/
class TokenSrc
{
public :
	TokenSrc( std::istream& file ) ;
	
	TokenSrc& ReadToken( Token& token ) ;
	void PutBack( const Token& token ) ;
	
	operator void*( ) const ;
	
	std::istream& Stream() ;
	
	/*!	peek tokens without removing them. This function will copy \a count
		tokens from the TokenSrc to \a outit . The copied tokens will \em not
		be removed from the token source. It is equal to calling ReadToken()
		\a count times and then call PutBack() \a count times.
		\param	outit	output iterator for storing output. can be a
						std::back_inserter or iterators to containers.
		\param	count	number of tokens to be peek
		\return	\a outit after incrementing it by number-of-token-extracted
				times.
	*/
	template <typename OutIt>
	OutIt Peek( OutIt outit, std::size_t count = 1 )
	{
		// read tokens to cache
		if ( Cache( count ) )
		{
			// copy token from cache to output
			for ( std::size_t i = 0 ; i < count ; i++ )
			{
				*outit = m_cache[count-i-1] ;
				outit++ ;
			}
		}
		return outit ;
	}
	
	void Ignore( std::size_t count = 1 ) ;
	
	void SetState( std::ios::iostate state ) ;
	void ResetState( ) ;
	
	bool HasCache( ) const ;
	
	TokenSrc& GetChar( char& ch ) ;

private :
	TokenSrc& ExtractToken( Token& token ) ;
	bool Cache( std::size_t count ) ;

private :
	std::istream&		m_file ;
	std::deque<Token>	m_cache ;

	std::size_t			m_pos ;
} ;

TokenSrc& operator>>( TokenSrc& src, Token& token ) ;

} // end of namespace

#endif
