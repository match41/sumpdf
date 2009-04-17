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
	: Exception( boost::format( "invalid %1% object" ) % type )
{
}

const std::pair<const std::string, PaintOp::FuncPtr> PaintOp::m_table[] =
{
	std::make_pair( "BT",	&PaintOp::DecodeNoArg<BeginText> ),
	std::make_pair( "ET",	&PaintOp::DecodeNoArg<EndText> ),
	
	// text state
	std::make_pair( "Tc",   &PaintOp::DecodeTextState<TextState::char_space> ),
	std::make_pair( "Tw",   &PaintOp::DecodeTextState<TextState::word_space> ),
	std::make_pair( "Tz",   &PaintOp::DecodeTextState<TextState::scale> ),
	std::make_pair( "TL",   &PaintOp::DecodeTextState<TextState::leading> ),
	std::make_pair( "Tf",   &PaintOp::DecodeTextState<TextState::font_size> ),
	std::make_pair( "Tr",   &PaintOp::DecodeTextState<TextState::render_mode> ),
	std::make_pair( "Ts",   &PaintOp::DecodeTextState<TextState::text_rise> ),
	
	// text positioning operators
	std::make_pair( "Td",	&PaintOp::DecodeTwoArgs<TextPosition,
	                                                double, double> ),
	std::make_pair( "Tm",	&PaintOp::Decode6Args<TextMatrix, double> ),
	
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

template <typename Op>
void PaintOp::DecodeNoArg( const Object *args, std::size_t count )
{
    if ( count != 0 )
        throw DecodeError( typeid(Op).name() ) ;
	    
	Op op = {} ;
	m_ops = op ;
}

template <typename Op, typename Arg1>
void PaintOp::DecodeOneArg( const Object *args, std::size_t count )
{
    if ( count != 1 )
        throw DecodeError( typeid(Op).name() ) ;

	Op op = { args[0].As<Arg1>() } ;
	m_ops = op ;
}

template <typename Op, typename Arg1, typename Arg2>
void PaintOp::DecodeTwoArgs( const Object *args, std::size_t count )
{
    if ( count != 2 )
        throw DecodeError( typeid(Op).name() ) ;
	
	Op op = { args[0].As<Arg1>(), args[1].As<Arg2>() } ;
	m_ops = op ;
}

template <typename Op, typename Arg>
void PaintOp::Decode6Args( const Object *args, std::size_t count )
{
    if ( count != 6 )
    	throw DecodeError( typeid(Op).name() ) ;

	Op op = { args[0].As<Arg>(), args[1].As<Arg>(), args[2].As<Arg>(),
				args[3].As<Arg>(), args[4].As<Arg>(), args[5].As<Arg>() } ;
	m_ops = op ;
}

template <typename Op, typename Arg1, Arg1 arg1, typename Arg2>
void PaintOp::DecodeTwoArgBind1st( const Object *args, std::size_t count )
{
    if ( count != 1 )
    	throw DecodeError( typeid(Op).name() ) ;
        
	Op op = { arg1, args[0].As<Arg2>() } ;
	m_ops = op ;
}

// help function for instantiating text state decoders
template <TextState::Type t>
void PaintOp::DecodeTextState( const Object *args, std::size_t count )
{
    DecodeTwoArgBind1st<TextState, TextState::Type, t, double>( args, count ) ;
}

std::ostream& operator<<( std::ostream& os, const PaintOp& op )
{
    return os ;
}

} // end of namespace
