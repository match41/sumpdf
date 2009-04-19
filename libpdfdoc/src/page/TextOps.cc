/***************************************************************************\
 *   Copyright (C) 2009 by Nestal Wan                                      *
 *   me@nestal.net                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
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

/*!
	\file	TextOps.cc
	\brief	implementation the TextOps class
	\date	Sun Apr 19 2009
	\author	Nestal Wan
*/

#include "TextOps.hh"

namespace pdf {

std::ostream& operator<<( std::ostream& os, const TextState& ts )
{
	const char *map[] =
	{
		"Tc", "Tw", "Tz", "TL", "Tr", "Ts"
	} ;
	
	assert( ts.type >= TextState::char_space ) ;
	assert( ts.type <= TextState::text_rise ) ;
	
	os << map[ts.type - TextState::char_space] << ' ' << ts.arg << ' ' ;
}

} // end of namespace
