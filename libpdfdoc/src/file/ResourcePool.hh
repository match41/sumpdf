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

#include "RefObjMap.hh"
#include "core/ObjWrapper.hh"

namespace pdf {

class BaseFont ;
class Object ;
class PageNode ;
class RealResources ;
class RefCounter ;

class ElementPool
{
public :
	template <typename Maker>
	typename Maker::result_type Load( const Ref& key, Maker maker )
	{
		typedef Maker::result_type ElementPtr ;
	
		RefCounter *tmp = m_pool.Find( key ) ;
		if ( tmp == 0 )
		{
			ElementPtr t = maker() ;
			
			// never add Ref() to the map.
			// Ref() is reserved for the case in which we don't want to
			// share the element.
			if ( key != Ref() )
				m_pool.Add( key, t ) ;
			
			return t ;
		}
		
		// throw exception if type mismatch
		else
			return dynamic_cast<ElementPtr>( tmp ) ;
	}
	
	void Add( const Ref& key, RefCounter *element )
	{
		m_pool.Add( key, element ) ;
	}

	template <typename Element>
	Element* Find( const Ref& key ) const
	{
		RefCounter *tmp = m_pool.Find( key ) ;
		
		// throw exception if type mismatch
		return tmp != 0 ? &dynamic_cast<Element&>( *tmp ) : 0 ;
	}

	Ref Find( RefCounter *element )
	{
		return m_pool.Find( element ) ;
	}

private :
	RefObjMap<RefCounter>	m_pool ;
} ;

} // end of namespace

#endif // RESOURCEPOOL_H_
