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

/**	\file	StateParamDict.cc
	\brief	implementation of the StateParamDict class
	\date	Feb 28, 2010
	\author	Nestal Wan
*/

#include "ExtGState.hh"

#include "file/DictReader.hh"
#include "graphics/GraphicsState.hh"

namespace pdf {

/**	constructor
	
*/
ExtGState::ExtGState( )
{
}

void ExtGState::Read( DictReader& dict )
{
	double val = 0.0 ;
	if ( dict.Detach( "LW", val ) )
		m_doubles.insert( std::make_pair( line_width, val ) ) ; 

	if ( dict.Detach( "LC", val ) )
		m_doubles.insert( std::make_pair( line_cap, val ) ) ; 

	if ( dict.Detach( "LJ", val ) )
		m_doubles.insert( std::make_pair( line_join, val ) ) ; 
}

Ref ExtGState::Write( File *file ) const
{
	Dictionary dict ;
	std::map<Field, double>::const_iterator di = m_doubles.find( line_width ) ;
	if ( di != m_doubles.end() )
		dict["LW"] = di->second ;
	
	di = m_doubles.find( line_cap ) ;
	if ( di != m_doubles.end() )
		dict["LC"] = di->second ;

	di = m_doubles.find( line_join ) ;
	if ( di != m_doubles.end() )
		dict["LJ"] = di->second ;

	return Ref() ;
}

void ExtGState::Apply( GraphicsState& gs ) const
{
	std::map<Field, double>::const_iterator di = m_doubles.find( line_width ) ;
	if ( di != m_doubles.end() )
		gs.LineWidth( di->second ) ;
	
}

} // end of namespace
