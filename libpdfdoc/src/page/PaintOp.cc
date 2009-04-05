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

#include "util/Util.hh"

#include <string>
#include <map>
#include <iostream>

namespace pdf {

PaintOp::PaintOp( const std::string& ops, const Object *args,
                                          std::size_t count )
{
std::cout << " got cmd: " << ops << ' ' << count << std::endl ;

	typedef void (PaintOp::*FuncPtr)( const Object*, std::size_t ) ;
	
	static const std::pair<const std::string, FuncPtr> table[] =
	{
		std::make_pair( "BT",		&PaintOp::DecodeNoArg<BeginText> ),
		std::make_pair( "ET",		&PaintOp::DecodeNoArg<EndText> ),
	} ;
	typedef std::map<std::string, FuncPtr> FuncMap ;
	static const FuncMap map( Begin( table ), End( table ) ) ;

	FuncMap::const_iterator i = map.find( ops ) ;
	if ( i != map.end() )
		(this->*i->second)( args, count ) ;
}

template <typename Op>
void PaintOp::DecodeNoArg( const Object *args, std::size_t count )
{
	Op op = {} ;
	m_ops = op ;
}

} // end of namespace
