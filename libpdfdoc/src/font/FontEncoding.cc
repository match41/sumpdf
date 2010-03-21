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

/**	\file	FontEncoding.cc
	\brief	implementation of the FontEncoding class
	\date	Mar 21, 2010
	\author	Nestal Wan
*/

#include "FontEncoding.hh"

#include "core/Array.hh"

#include "file/ArrayReader.hh"
#include "file/DictReader.hh"

#include "font/CodeMap.hh"

#include <iostream>

namespace pdf {

/**	constructor
	
*/
FontEncoding::FontEncoding( DictReader& self )
{
std::cout << *self << std::endl ;

	int current = 0 ;

	ArrayReader diff ;
	if ( self.Detach( "Differences", diff ) )
	{
		for ( Array::iterator i = diff->begin() ; i != diff->end() ; ++i )
		{
			if ( i->Is<int>() )
				current = diff.At<int>( i-diff->begin() ) ;
			
			else if ( i->Is<Name>() )
			{
				wchar_t ch = NameToUnicode( i->As<Name>().Str().c_str() ) ;
				m_charmap.insert( std::make_pair(
					static_cast<unsigned short>( current ), ch ) ) ;
			
				current++ ;
			}
		}
	}
}

wchar_t FontEncoding::LookUp( unsigned short char_code ) const
{
	CharMap::const_iterator i = m_charmap.find( char_code ) ;
	return i != m_charmap.end() ? i->second : 0 ; 
}

} // end of namespace
