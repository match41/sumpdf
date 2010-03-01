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

#include "StateParamDict.hh"

#include "file/DictReader.hh"

namespace pdf {

/**	constructor
	
*/
StateParamDict::StateParamDict( )
{
}

void StateParamDict::Read( DictReader& dict )
{
	double val = 0.0 ;
	if ( dict.Detach( "LW", val ) )
		m_doubles.insert( std::make_pair( line_width, val ) ) ; 

	if ( dict.Detach( "LC", val ) )
		m_doubles.insert( std::make_pair( line_cap, val ) ) ; 

	if ( dict.Detach( "LJ", val ) )
		m_doubles.insert( std::make_pair( line_join, val ) ) ; 
}

Ref StateParamDict::Write( File *file ) const
{
	return Ref() ;
}

} // end of namespace
