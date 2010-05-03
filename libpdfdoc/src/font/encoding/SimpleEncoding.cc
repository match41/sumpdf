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

/**	\file	SimpleEncoding.cc
	\brief	implementation of the FontEncoding class
	\date	Mar 21, 2010
	\author	Nestal Wan
*/

#include "SimpleEncoding.hh"

#include "CodeMap.hh"

#include "core/Array.hh"
#include "core/Ref.hh"

#include "file/ArrayReader.hh"
#include "file/DictReader.hh"

#include "util/Debug.hh"

namespace pdf {

/**	constructor
	
*/
SimpleEncoding::SimpleEncoding( DictReader& self )
{
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
				wchar_t ch = 0 ;
				if ( NameToUnicode( i->As<Name>().Str().c_str(), ch ) )
					Add( static_cast<unsigned short>( current ), ch ) ;
			
				current++ ;
			}
		}
	}
	
	self.Detach( "BaseEncoding", m_base ) ;
}


} // end of namespace
