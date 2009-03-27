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
#include "util/Util.hh"

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

TokenSrc& operator>>( TokenSrc& is, Ref& obj )
{
	// ID generation "R"
	Token t[3] ;
	
	// peek 3 tokens. and check if they are in the reference format
	if ( is.Peek( t, Count(t) ) == End(t) &&
		 t[0].IsInt( ) && t[1].IsInt() && t[2].Get( ) == "R" )
	{
		// get the object ID and generation
		obj.m_obj_id		= t[0].As<int>() ;
		obj.m_generation	= t[1].As<int>() ;
		
		// discard the three tokens as we consumed it
		is.Ignore( Count(t) ) ;
	}
	
	// set stream state to indicate failure
	else
		is.SetState( std::ios::failbit ) ;

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
