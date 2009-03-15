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
	\file	Ref.cc
	\brief	implementation the Ref class
	\date	Wed Mar 5 2008
	\author	Nestal Wan
*/

#include "Ref.hh"

#include "Token.hh"
#include "TokenSrc.hh"

#include <cassert>
#include <cstdlib>
#include <sstream>
#include <iostream>

namespace pdf {

Ref::Ref( )
	: m_obj_id( 0 ), m_generation( 0 )
{
}

Ref::Ref( std::size_t id, std::size_t gen )
	: m_obj_id( id ), m_generation( gen )
{
	// check for no overflow
	assert( id == m_obj_id ) ;
	assert( gen == m_generation ) ;
}

std::size_t Ref::ID( ) const
{
	return m_obj_id ;
}

std::size_t Ref::Generation( ) const
{
	return m_generation ;
}

std::ostream& operator<<( std::ostream& os, const Ref& obj )
{
	return os << std::dec << obj.m_obj_id << ' ' << obj.m_generation << " R" ;
}

std::istream& operator>>( std::istream& is, Ref& b )
{
	TokenSrc src( is ) ;
	return ( src >> b ).Stream( ) ;
}

//TokenSrc& Ref::Decode( TokenSrc& is, const Token& first )
TokenSrc& operator>>( TokenSrc& is, Ref& obj )
{
	Token id, gen, s ;
	if ( is >> id )
	{
		if ( is >> gen )
		{
			if ( is >> s )
			{
				if ( id.IsInt( ) && gen.IsInt() && s.Get( ) == "R" )
				{
					obj.m_obj_id		= id.As<int>() ;
					obj.m_generation	= gen.As<int>() ;
				}
				else
				{
					// decode failed. need to put back the tokens
					is.PutBack( s ) ;
					is.PutBack( gen ) ;
					is.PutBack( id ) ;
					
					// set stream state to indicate failure
					is.SetState( std::ios::failbit ) ;
				}
			}
			else
			{
				is.PutBack( gen ) ;
				is.PutBack( id ) ;
			}
		}
		else
		{
			is.PutBack( id ) ;
		}
	}
	return is ;
}

bool Ref::operator==( const Ref& obj ) const
{
	return m_obj_id == obj.m_obj_id && m_generation == obj.m_generation ;
}

bool Ref::operator!=( const Ref& obj ) const
{
	return !operator==( obj ) ;
}

bool operator<( const Ref& obj1, const Ref& obj2 )
{
	return obj1.ID() < obj2.ID() ;
}

bool operator>( const Ref& obj1, const Ref& obj2 )
{
	return obj1.ID() > obj2.ID() ;
}

} // end of namespace
