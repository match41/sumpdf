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

/**	\file	PathSegment.cc
	\brief	implementation of the PatchSegment class
	\date	Apr 6, 2010
	\author	Nestal Wan
*/

#include "graphics/PathSegment.hh"

#include "util/Debug.hh"

#include <ostream>
#include <iterator>
#include <algorithm>

namespace pdf {

/**	constructor
	
*/
PathSegment::PathSegment( Op op, const double *points )
	: m_points( points )
	, m_op( op )
{
	PDF_ASSERT( m_points != 0 || m_op == close ) ;
	PDF_ASSERT( m_op >= move && m_op <= close ) ;
}

const double* PathSegment::Points( ) const
{
	PDF_ASSERT( m_points != 0 || m_op == close ) ;
	return m_points ;
}

std::size_t PathSegment::Count( ) const
{
	return ArgCount( m_op ) ;
}

std::size_t PathSegment::ArgCount( Op op )
{
	PDF_ASSERT( op >= move && op <= close ) ;
	
	// 	enum Op { move, line, cubic123, cubic23, cubic13, close } ;
	static const std::size_t table[] =
	{
		2, 2, 6, 4, 4, 0
	} ;
	return table[op] ;
}

PathSegment::Op PathSegment::GetOp( ) const
{
	PDF_ASSERT( m_op >= move && m_op <= close ) ;
	return m_op ;
}

PathSegment::iterator PathSegment::begin() const
{
	return m_points ;
}

PathSegment::iterator PathSegment::end() const
{
	return m_points + Count() ;
}

double PathSegment::operator[]( std::size_t i ) const
{
	PDF_ASSERT( i < Count() ) ;
	return m_points[i] ;
}

std::ostream& operator<<( std::ostream& os, const PathSegment& seg )
{
	PathSegment::Op op = seg.GetOp() ;
	PDF_ASSERT( op >= PathSegment::move && op <= PathSegment::close ) ;

	// enum Op { move, line, cubic123, cubic23, cubic13, close } ;
	static const char *op_name[] =
	{
		"m", "l", "c", "v", "y", "h"
	} ;
	std::copy( seg.Points(), seg.Points() + seg.Count(),
		std::ostream_iterator<double>( os, " " ) ) ;
	os << op_name[op] << "\n" ;
	return os ;
}

} // end of namespace
