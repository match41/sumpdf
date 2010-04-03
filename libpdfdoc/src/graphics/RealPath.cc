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

namespace pdf {

/**	constructor
	
*/
RealPath::RealPath( const GraphicsState& gs )
	: m_state( gs )
{
}

// Graphics virtual functions
void RealPath::OnCommand( ContentOp& op, const ResourcesDict *res )
{
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
std::size_t RealPath::SegmentCount( ) const
{
	return m_segs.size() ;
}

RealPath::Segment RealPath::GetSegment( std::size_t index ) const
{
	return m_segs[index] ;
}

} // end of namespace
