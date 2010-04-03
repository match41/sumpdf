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

/**	\file	RealPath.hh
    \brief	definition the RealPath class
    \date	Apr 3, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_REALPATH_HH_EADER_INCLUDED__
#define __PDF_REALPATH_HH_EADER_INCLUDED__

#include "graphics/Path.hh"

#include "graphics/GraphicsState.hh"

#include <vector>

namespace pdf {

///	brief description
/**	\internal
	The RealPath class represents
*/
class RealPath : public Path
{
public :
	explicit RealPath( const GraphicsState& gs ) ;
	
	// Graphics virtual functions
	void OnCommand( ContentOp& op, const ResourcesDict *res ) ;
	void Print( std::ostream& os, ResourcesDict *res ) const ;
	void Visit( GraphicsVisitor *visitor ) ;
	GraphicsState GetState( ) const ;
	
	// Path virtual functions
	std::size_t SegmentCount( ) const ;
	Segment GetSegment( std::size_t index ) const ;

private :
	std::vector<double>		m_points ;
	std::vector<Segment>	m_segs ;
	
	GraphicsState			m_state ;
} ;

} // end of namespace

#endif // REALPATH_HH_
