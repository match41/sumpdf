/***************************************************************************
 *   Copyright (C) 2006 by Nestal Wan                                      *
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
 ***************************************************************************/

/*!
	\file	ElementTracker.cc
	\brief	implementation the ElementTracker class
	\date	Sat Apr 19 2008
	\author	Nestal Wan
*/

#include "ElementTracker.hh"

#include "ElementList.hh"
#include "IElement.hh"

#include <boost/bind.hpp>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/indexed_by.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/tag.hpp>

namespace pdf {

ElementTracker::ElementTracker( )
{
}

ElementTracker::~ElementTracker( )
{
}

void ElementTracker::Traverse( IElement *e )
{
	Add( e ) ;
	
	ElementList child = e->GetChildren( ) ;
	std::for_each( child.begin( ), child.end( ),
	               boost::bind( &ElementTracker::Add, this, _1 ) ) ;
}

void ElementTracker::Add( IElement *e )
{
	Map& map = m_con.get<MapTag>( ) ;
	
	Map::iterator i = map.find( e ) ;
	if ( i != map.end( ) )
		map.erase( i ) ;

	map.insert( e ) ;
}

void ElementTracker::Get( ElementList& list ) const
{
	const Seq& seq = m_con.get<SeqTag>( ) ;
	list.assign( seq.begin( ), seq.end( ) ) ;
}

} // end of namespace
