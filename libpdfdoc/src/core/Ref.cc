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

/*!	\brief	default constructor
	\internal

	Initialize ID as 0 and generation as 0.
*/
Ref::Ref( )
	: m_obj_id( 0 ), m_generation( 0 )
{
}

/*!	\brief	constructor
	\internal

	Initialize ID and generation as \a id and \a gen.
*/
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

std::size_t Ref::Gen( ) const
{
	return m_generation ;
}

std::ostream& operator<<( std::ostream& os, const Ref& obj )
{
	return os << std::dec << obj.m_obj_id << ' ' << obj.m_generation << " R" ;
}

/*!	\brief	extraction operator from std::istream
	\internal

	This function extracts a Ref object \a b from an std::istream \a is . It
	will call operator>>( TokenSrc&, Ref& ) internally to do the job.
	\param	is	std::istream to be extracted from
	\param	obj	extracted reference object
	\return	the input std::istream reference, i.e. \a is
*/
std::istream& operator>>( std::istream& is, Ref& b )
{
	TokenSrc src( is ) ;
	return ( src >> b ).Stream( ) ;
}

/*!	\brief	extraction operator from TokenSrc
	\internal

	This function extracts a Ref object \a obj from a TokenSrc \a is . It will
	call TokenSrc::Peek() for 3 tokens and verify if the first two are integer
	and the third one is "R". If yes, it is a valid indirect object and it will
	be extracted successfully. Otherwise, all three tokens are unchanged and
	will still be in the TokenSrc.
	\param	is	TokenSrc to be extracted from
	\param	obj	the extracted reference object
	\return	the input TokenSrc reference, i.e. \a is
*/
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
	return obj1.ID() != obj2.ID() ? obj1.ID()  < obj2.ID()
	                              : obj1.Gen() < obj2.Gen() ;
}

bool operator>( const Ref& obj1, const Ref& obj2 )
{
	return obj1.ID() != obj2.ID() ? obj1.ID()  > obj2.ID()
	                              : obj1.Gen() > obj2.Gen() ;
}

} // end of namespace
