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
	\file	PaintOp.cc
	\brief	implementation the PaintOp class
	\date	Mon Mar 2 2009
	\author	Nestal Wan
*/

#include "PaintOp.hh"

#include "core/Object.hh"
#include "util/Util.hh"

#include <boost/format.hpp>

#include <string>
#include <map>
#include <iostream>

namespace pdf {

PaintOp::DecodeError::DecodeError( const char *type )
	: Exception( boost::format( "invalid %1% object" ) % Demangle(type) )
{
}

/*!	lookup table for the decoding functions. This table maps operation
	strings to their corresponding decoding functions, which will decode
	the arguments and save them in the operation structures.
*/
const std::pair<const std::string, PaintOp::FuncPtr> PaintOp::m_table[] =
{
	std::make_pair( "BT",	&PaintOp::Decode<BeginText::_> ),
	std::make_pair( "ET",	&PaintOp::Decode<EndText::_> ),

	// text state
	std::make_pair( "Tc",   &PaintOp::Decode<CharSpace::_> ),
	std::make_pair( "Tw",   &PaintOp::Decode<WordSpace::_> ),
	std::make_pair( "Tz",   &PaintOp::Decode<TextScale::_> ),
	std::make_pair( "TL",   &PaintOp::Decode<TextLeading::_> ),
	std::make_pair( "Tf",   &PaintOp::Decode<TextFont::_> ),
	std::make_pair( "Tr",   &PaintOp::Decode<TextRender::_> ),
	std::make_pair( "Ts",   &PaintOp::Decode<TextRise::_> ),
	
	// text positioning operators
	std::make_pair( "Td",	&PaintOp::Decode<TextPosition::_> ),
	std::make_pair( "Tm",	&PaintOp::Decode<TextMatrix::_> ),
	
} ;

const PaintOp::FuncMap PaintOp::m_func_map( Begin( PaintOp::m_table ),
                                            End(   PaintOp::m_table ) ) ;

PaintOp::PaintOp( const std::string& ops, const Object *args,
                                          std::size_t count )
{
std::cout << " got cmd: " << ops << ' ' << count << std::endl ;
	FuncMap::const_iterator i = m_func_map.find( ops ) ;
	if ( i != m_func_map.end() )
		(this->*i->second)( args, count ) ;
}

std::ostream& operator<<( std::ostream& os, const PaintOp& op )
{
    return os ;
}

template <typename Op, std::size_t N>
class DecodeTuple
{
public :
	Op operator()( const Object *args, std::size_t count ) const ;
} ;

template <typename Op>
class DecodeTuple<Op,0>
{
public :
	Op operator()( const Object *, std::size_t ) const
	{
		return Op( ) ;
	}
} ;

template <typename Op>
class DecodeTuple<Op,1>
{
public :
	Op operator()( const Object *args, std::size_t count ) const
	{
		typename boost::tuples::element<0, Op>::type arg(args[0]) ;
		return Op( arg ) ;
	}
} ;

template <typename Op>
class DecodeTuple<Op,2>
{
public :
	Op operator()( const Object *args, std::size_t count ) const
	{
		return Op( args[0], args[1] ) ;
	}
} ;

template <typename Op>
class DecodeTuple<Op,6>
{
public :
	Op operator()( const Object *args, std::size_t count ) const
	{
		return Op( args[0], args[1], args[2], args[3], args[4], args[5] ) ;
	}
} ;

template <typename Op>
void PaintOp::Decode( const Object *args, std::size_t count )
{
	if ( count != static_cast<std::size_t>(boost::tuples::length<Op>::value ))
		throw DecodeError( typeid(Op).name() ) ;

	m_ops = DecodeTuple<Op,
		boost::tuples::length<Op>::value>()( args, count ) ;
}


} // end of namespace
