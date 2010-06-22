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
#include <iostream>

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
	std::make_pair( "S",	&RealPath::OnS ),
	std::make_pair( "s",	&RealPath::Ons ),
	std::make_pair( "f",	&RealPath::Onf ),
	std::make_pair( "F",	&RealPath::Onf ),
	std::make_pair( "f*",	&RealPath::OnfStar ),
	std::make_pair( "B",	&RealPath::OnB ),
	std::make_pair( "B*",	&RealPath::OnBStar ),
	std::make_pair( "b",	&RealPath::Onb ),
	std::make_pair( "b*",	&RealPath::OnbStar ),
	std::make_pair( "n",	&RealPath::Onn ),
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
	: m_stroke( true )
	, m_fill( false )
	, m_fill_mode( winding )
	, m_state( gs )
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

void RealPath::Print(
	std::ostream&	os,
	ResourcesDict	*res,
	GraphicsState&	gs ) const
{
	PDF_ASSERT( m_ops.size() == m_pt_index.size() ) ;

//	if ( m_state != gs )
		m_state.Print( os, res, gs ) ;

	std::size_t pt_idx = 0 ;
	for ( std::vector<PathSegment::Op>::const_iterator i = m_ops.begin() ;
		i != m_ops.end() ; ++i )
	{
		PDF_ASSERT( pt_idx == m_pt_index[i-m_ops.begin()] ) ;
		PDF_ASSERT( pt_idx + PathSegment::ArgCount(*i) <= m_points.size() ) ;
		
		PathSegment ps( *i, &m_points[pt_idx] ) ;
		os << ps ;

		pt_idx += ps.Count() ;
	}
	
	if ( m_stroke && !m_fill )
		os << "S\n" ;
	else if ( !m_stroke	&& m_fill && m_fill_mode == winding )
		os << "f\n" ;
	else if ( !m_stroke	&& m_fill && m_fill_mode == odd_even )
		os << "f*\n" ;
	else if ( m_stroke	&& m_fill && m_fill_mode == winding )
		os << "B\n" ;
	else if ( m_stroke	&& m_fill && m_fill_mode == odd_even )
		os << "B*\n" ;
	else if ( !m_stroke	&& !m_fill )
		os << "n\n" ;
	else
		throw Exception( "??" ) ;
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
	if ( op.Count() >= 4 )
	{
		double x		= op[0] ;
		double y		= op[1] ;
		double width 	= op[2] ;
		double height	= op[3] ;

		// draw rectangle
		MoveTo( x,			y ) ;
		LineTo( x+width,	y ) ;
		LineTo( x+width,	y+height ) ;
		LineTo( x,			y+height ) ;
		CloseSubPath( ) ;
	}
}

void RealPath::AddSegment( const PathSegment& seg )
{
	PDF_ASSERT( m_pt_index.size() == m_ops.size() ) ;
	
	m_pt_index.push_back( m_points.size() ) ;
	m_points.insert( m_points.end(), seg.begin(), seg.end() ) ;
	m_ops.push_back( seg.GetOp() ) ;

	PDF_ASSERT( m_pt_index.size() == m_ops.size() ) ;
}

void RealPath::MoveTo( double x, double y )
{
	// move x, y
	double ops[] = { x, y } ;
	AddSegment( PathSegment( PathSegment::move, ops ) ) ; 
}

void RealPath::LineTo( double x, double y )
{
	// move x, y
	double ops[] = { x, y } ;
	AddSegment( PathSegment( PathSegment::line, ops ) ) ; 
}

void RealPath::CloseSubPath( )
{
	PDF_ASSERT( m_pt_index.size() == m_ops.size() ) ;
	m_pt_index.push_back( m_points.size() ) ;
	m_ops.push_back( PathSegment::close ) ;
	PDF_ASSERT( m_pt_index.size() == m_ops.size() ) ;
}

Matrix RealPath::Transform( ) const
{
	return m_ctm ;
}

void RealPath::Transform( const Matrix& mat )
{
	m_ctm = mat ;
}

void RealPath::OnS( ContentOp& op, const ResourcesDict *res )
{
	m_stroke	= true ;
	m_fill		= false ;
}

void RealPath::Ons( ContentOp& op, const ResourcesDict *res )
{
	CloseSubPath( ) ;
	OnS( op, res ) ;
}

void RealPath::Onf( ContentOp& , const ResourcesDict * )
{
	m_stroke	= false ;
	m_fill		= true ;
	m_fill_mode	= winding ;
}

void RealPath::OnfStar( ContentOp& , const ResourcesDict * )
{
	m_stroke	= false ;
	m_fill		= true ;
	m_fill_mode	= odd_even ;
}

void RealPath::OnB( ContentOp& , const ResourcesDict * )
{
	m_stroke	= true ;
	m_fill		= true ;
	m_fill_mode	= winding ;
}

void RealPath::OnBStar( ContentOp& , const ResourcesDict * )
{
	m_stroke	= true ;
	m_fill		= true ;
	m_fill_mode	= odd_even ;
}

void RealPath::Onb( ContentOp& op, const ResourcesDict *res )
{
	CloseSubPath( ) ;
	OnB( op, res ) ;
}

void RealPath::OnbStar( ContentOp& op, const ResourcesDict *res )
{
	CloseSubPath( ) ;
	Onb( op, res ) ;
}

void RealPath::Onn( ContentOp& , const ResourcesDict * )
{
	m_stroke	= false ;
	m_fill		= false ;
}

bool RealPath::IsFill( ) const
{
	return m_fill ;
}

bool RealPath::IsStroke( ) const
{
	return m_stroke ;
}

RealPath::FillMode RealPath::FillRule( ) const
{
	return m_fill_mode ;
}

bool RealPath::SetStyle( bool fill, bool stroke, FillMode fill_mode )
{
	bool changed = false ;
	if ( m_fill != fill )
	{
		m_fill = fill ;
		changed = true ;
	}
	if ( m_stroke != stroke )
	{
		m_stroke = stroke ;
		changed = true ;
	}
	if ( m_fill_mode != fill_mode )
	{
		m_fill_mode = fill_mode ;
		changed = true ;
	}
	return changed ;
}

} // end of namespace
