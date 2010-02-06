/***************************************************************************\
 *   Copyright (C) 2009 by Nestal Wan                                      *
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

/**
	\file	ResourcePool.hh
	\brief	definition the ResourcePool class
	\date	Dec 26, 2009
	\author	nestal
*/


#ifndef __PDF_RESOURCEPOOL_HEADER_INCLUDED__
#define __PDF_RESOURCEPOOL_HEADER_INCLUDED__

//#include "RefObjMap.hh"

#include "core/Ref.hh"
#include "util/Debug.hh"

#include <boost/bimap.hpp>
#include <boost/bimap/set_of.hpp>

namespace pdf {

class RefCounter ;

class ElementPool
{
public :
	template <typename Element>
	bool Acquire( const Ref& link, Element* &element )
	{
		MapType::left_iterator i = m_pool.left.find( link ) ;
		if ( i != m_pool.left.end() )
		{
			if ( element != 0 )
				element->Release( ) ;
			
			element = &dynamic_cast<Element&>(*i->second) ;
			element->AddRef() ;

			return true ;
		}
		else
			return false ;
	}
	
	void Add( const Ref& key, RefCounter *element )
	{
		if ( key != Ref() )
			m_pool.insert( MapEntry( key, element ) ) ;
	}

	template <typename Element>
	Element* Find( const Ref& key )
	{
		MapType::left_iterator i = m_pool.left.find( key ) ;
		
		// throw exception if type mismatch
		return i != m_pool.left.end() ? &dynamic_cast<Element&>(*i->second) : 0;
	}

	Ref Find( RefCounter *element )
	{
		MapType::right_iterator i = m_pool.right.find( element ) ;
		return i != m_pool.right.end() ? i->second : Ref() ;
	}

	bool Has( const Ref& key ) const
	{
		return m_pool.left.find( key ) != m_pool.left.end( ) ;
	}

private :
//	RefObjMap<RefCounter>	m_pool ;

	typedef	boost::bimap<
		boost::bimaps::set_of<Ref>,
		boost::bimaps::set_of<RefCounter*>
	> MapType ; 
	
	typedef MapType::value_type MapEntry ;
	
	MapType	m_pool ;
} ;

} // end of namespace

#endif // RESOURCEPOOL_H_
