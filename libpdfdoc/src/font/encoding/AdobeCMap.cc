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

/**	\file	AdobeCMap.cc
	\brief	implementation of the AdobeCMap class
	\date	Apr 21, 2010
	\author	Nestal Wan
*/

#include "AdobeCMap.hh"

#include "core/Object.hh"

#include <iostream>
#include <sstream>

namespace pdf {

/**	constructor
	
*/
AdobeCMap::AdobeCMap( std::istream& is )
{
	std::string line ;
	while ( std::getline( is, line ) )
	{
		std::istringstream oss( line ) ;
		
		std::size_t count = 0 ;
		std::string op ;
		if ( oss >> count >> op && op == "beginbfrange" && count > 0 )
			ReadBFRange( is, count ) ;
	}
}

void AdobeCMap::ReadBFRange( std::istream& is, std::size_t count )
{
	for ( std::size_t i = 0 ; i < count ; ++i )
	{
		std::string line ;
		while ( std::getline( is, line ) )
		{
			std::istringstream oss( line ) ;
/*			
			Object begin, end, unicode ;
			if ( oss >> begin >> end >> unicode )
			{
			}
*/
		}
	}
}

} // end of namespace
