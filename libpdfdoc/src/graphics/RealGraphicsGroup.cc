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

/**	\file	RealGraphicsGroup.cc
	\brief	implementation of the RealGraphicsGroup class
	\date	May 23, 2010
	\author	Nestal Wan
*/

#include "RealGraphicsGroup.hh"
#include "graphics/ExtGraphicsLink.hh"

namespace pdf {

/**	constructor
	
*/
RealGraphicsGroup::RealGraphicsGroup( Stream&, File * )
{
}

std::size_t RealGraphicsGroup::Count( ) const
{
	return 0 ;
}

const Graphics* RealGraphicsGroup::At( std::size_t idx ) const
{
	return 0 ;
}

Graphics* RealGraphicsGroup::CreateRenderedObject(
	const GraphicsState&	gs,
	const Matrix&			ctm ) const
{
	return new ExtGraphicsLink<GraphicsGroup>( gs, ctm, this ) ;
}

} // end of namespace
