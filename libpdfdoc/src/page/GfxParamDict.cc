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

/**	\file	GfxParamDict.cc
	\brief	implementation of the GfxParamDict class
	\date	Feb 25, 2010
	\author	Nestal Wan
*/

#include "GfxParamDict.hh"

#include "file/DictReader.hh"
#include "file/File.hh"

#include "util/Debug.hh"
#include "util/Exception.hh"

namespace pdf {

/**	constructor
	
*/
GfxParamDict::GfxParamDict( const GraphicsState& state )
: m_state( state )
{
}

void GfxParamDict::Read( DictReader& dict )
{
	Name type ;
	if ( dict.Detach( "Type", type ) && type != Name("ExtGState") )
		throw ParseError( "invalid type for ExtGState" ) ;
	
	double line_width ;
	if ( dict.Detach( "LW", line_width ) )
		m_state.LineWidth( line_width ) ;
}

Ref GfxParamDict::Write( File *file ) const
{
	Dictionary dict ;
	dict["LW"] = m_state.LineWidth() ;
	return file->WriteObj( dict ) ;
}

} // end of namespace
