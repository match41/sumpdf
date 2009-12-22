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

/**
	\file	ResourcePool.hh
	\brief	definition the ResourcePool class
	\date	Dec 23, 2009
	\author	nestal
*/


#ifndef __PDF_RESOURCEPOOL_HH_EADER_INCLUDED__
#define __PDF_RESOURCEPOOL_HH_EADER_INCLUDED__

#include "core/Ref.hh"

#include <cassert>
#include <map>

namespace pdf {

template <typename T>
class ResourcePool
{
public :
	ResourcePool( )
	{
	}
	
	T* Find( const Ref& link )
	{
		T *t = TryFind( link ) ;
		return t != 0 ? t : new T ;
	}

	template <typename P1>
	T* Find( const Ref& link, P1 p1 )
	{
		T *t = TryFind( link ) ;
		return t != 0 ? t : new T(p1) ;
	}
	
	template <typename P1, typename P2>
	T* Find( const Ref& link, P1 p1, P2 p2 )
	{
		T *t = TryFind( link ) ;
		return t != 0 ? t : new T(p1, p2) ;
	}
	
	template <typename P1, typename P2, typename P3>
	T* Find( const Ref& link, P1 p1, P2 p2, P3 p3 )
	{
		T *t = TryFind( link ) ;
		return t != 0 ? t : new T(p1, p2, p3) ;
	}
	
	void Add( const Ref& link, T *res )
	{
		assert( m_pool.find(link) == m_pool.end() ) ;
		m_pool.insert( std::make_pair( link, res ) ) ;
	}

private :
	T* TryFind( const Ref& link )
	{
		typename MapType::iterator i = m_pool.find( link ) ;
		if ( i != m_pool.end() )
		{
			assert( i->second != 0 ) ;
		
			// assume T is a RefCountObj
			i->second->AddRef( ) ;
			return i->second ;
		}
		else
			return 0 ;
	}

private :
	typedef	std::map<Ref, T*> MapType ; 
	MapType	m_pool ;
} ;

} // end of namespace

#endif // RESOURCEPOOL_HH_
