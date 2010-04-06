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

#include <map>

namespace pdf {

struct RealPath::HandlerMap
{
	/// command handler
	typedef void (RealPath::*Handler)( ContentOp& , const ResourcesDict* ) ;
	typedef std::map<Token, Handler>	Map ;

	static const Map::value_type	m_val[] ;
	static const Map				m_map ;
} ;

const RealPath::HandlerMap::Map::value_type	RealPath::HandlerMap::m_val[] =
{
	// path construction commands
	std::make_pair( "m",	&RealPath::Onm ),
	std::make_pair( "l",	&RealPath::Onl ),
	std::make_pair( "c",	&RealPath::Onc ),
	std::make_pair( "v",	&RealPath::Onv ),
	std::make_pair( "y",	&RealPath::Ony ),
	std::make_pair( "h",	&RealPath::Onh ),
	std::make_pair( "re",	&RealPath::Onre ),
	
	// path showing commands
} ;

/**	constructor
	
*/
RealPath::RealPath( const GraphicsState& gs )
	: m_state( gs )
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
	return m_segs.size() ;
}

PathSegment RealPath::Segment( std::size_t index ) const
{
	return m_segs[index] ;
}

// position command handlers
void RealPath::Onm( ContentOp& op, const ResourcesDict *res )
{
}

void RealPath::Onl( ContentOp& op, const ResourcesDict *res )
{
}

void RealPath::Onc( ContentOp& op, const ResourcesDict *res )
{
}

void RealPath::Onv( ContentOp& op, const ResourcesDict *res )
{
}

void RealPath::Ony( ContentOp& op, const ResourcesDict *res )
{
}

void RealPath::Onh( ContentOp& op, const ResourcesDict *res )
{
}

void RealPath::Onre( ContentOp& op, const ResourcesDict *res )
{
}

} // end of namespace
