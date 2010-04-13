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

/**	\file	RealPath.cc
	\brief	implementation of the RealPath class
	\date	Apr 3, 2010
	\author	Nestal Wan
*/

#include "RealPath.hh"

#include "graphics/GraphicsVisitor.hh"

#include "core/Token.hh"

#include "page/ContentOp.hh"

#include "util/Debug.hh"
#include "util/Util.hh"

#include <map>

namespace pdf {

struct RealPath::HandlerMap
{
	/// command handler
	typedef void (RealPath::*Handler)( ContentOp& , const ResourcesDict* ) ;
	typedef std::map<Token, Handler>	Map ;

	static const Map::value_type	m_val[] ;
	static const Map				m_map ;

	typedef std::map<Token, PathSegment::Op>	SegOpMap ;

	static const SegOpMap::value_type	m_seg_val[] ;
	static const SegOpMap				m_seg_map ;
} ;

const RealPath::HandlerMap::Map::value_type	RealPath::HandlerMap::m_val[] =
{
	// path construction commands
	std::make_pair( "m",	&RealPath::OnPositionCommands ),
	std::make_pair( "l",	&RealPath::OnPositionCommands ),
	std::make_pair( "c",	&RealPath::OnPositionCommands ),
	std::make_pair( "v",	&RealPath::OnPositionCommands ),
	std::make_pair( "y",	&RealPath::OnPositionCommands ),
	std::make_pair( "h",	&RealPath::OnPositionCommands ),
	std::make_pair( "re",	&RealPath::Onre ),
	
	// path showing commands
} ;

const RealPath::HandlerMap::Map RealPath::HandlerMap::m_map(
	Begin( RealPath::HandlerMap::m_val ),
	End( RealPath::HandlerMap::m_val ) ) ;

const RealPath::HandlerMap::SegOpMap::value_type
	RealPath::HandlerMap::m_seg_val[] =
{
	std::make_pair( "m",	PathSegment::move ),
	std::make_pair( "l",	PathSegment::line ),
	std::make_pair( "c",	PathSegment::cubic123 ),
	std::make_pair( "v",	PathSegment::cubic23 ),
	std::make_pair( "y",	PathSegment::cubic13 ),
	std::make_pair( "h",	PathSegment::close ),
} ;

const RealPath::HandlerMap::SegOpMap RealPath::HandlerMap::m_seg_map(
	Begin( RealPath::HandlerMap::m_seg_val ),
	End( RealPath::HandlerMap::m_seg_val ) ) ;

/**	constructor
	
*/
RealPath::RealPath( const GraphicsState& gs, const Matrix& ctm )
	: m_state( gs )
	, m_ctm( ctm )
{
}

// Graphics virtual functions
void RealPath::OnCommand( ContentOp& op, const ResourcesDict *res )
{
	HandlerMap::Map::const_iterator i = HandlerMap::m_map.find( op.Operator() );
	if ( i != HandlerMap::m_map.end() )
		(this->*(i->second))( op, res ) ;
}

void RealPath::Print( std::ostream& os, ResourcesDict *res ) const
{
}

void RealPath::Visit( GraphicsVisitor *visitor )
{
	visitor->VisitPath( this ) ;
}

GraphicsState RealPath::GetState( ) const
{
	return m_state ;
}

// Path virtual functions
std::size_t RealPath::Count( ) const
{
	PDF_ASSERT( m_ops.size() == m_pt_index.size() ) ;
	return m_ops.size() ;
}

PathSegment RealPath::Segment( std::size_t index ) const
{
	PDF_ASSERT( m_ops.size() == m_pt_index.size() ) ;
	PDF_ASSERT( index < m_ops.size()  ) ;
	PDF_ASSERT( m_pt_index[index] < m_points.size() ||
		PathSegment::ArgCount(m_ops[index]) == 0 ) ;
	
	return PathSegment( m_ops[index],
		PathSegment::ArgCount(m_ops[index]) == 0 ? 0 :
		&m_points[m_pt_index[index]] ) ;
}

// position command handlers
void RealPath::OnPositionCommands( ContentOp& op, const ResourcesDict *res )
{
	HandlerMap::SegOpMap::const_iterator i = HandlerMap::m_seg_map.find( op.Operator() ) ;
	if ( i != HandlerMap::m_seg_map.end() )
	{
		std::size_t count = PathSegment::ArgCount( i->second ) ;
		if ( op.Count() >= count )
		{
			double temp[PathSegment::m_max_arg_count] = {} ;
			
			// we have a function named Count() as well, so need to
			// specify it's the namespace one
			PDF_ASSERT( pdf::Count(temp) >= count ) ;
			std::copy( op.begin(), op.begin() + count, temp ) ;
			
			// OK. still no throw here. commit
			m_pt_index.push_back( m_points.size() ) ;
			m_points.insert( m_points.end(), temp, temp + count ) ;
			m_ops.push_back( i->second ) ;
		}
	}
}

void RealPath::Onre( ContentOp& op, const ResourcesDict *res )
{
}

Matrix RealPath::Transform( ) const
{
	return m_ctm ;
}

} // end of namespace
